/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: main.cpp
  */ 

#include "edges.h"
#include "graph.h"
#include "constructGraph.h"
#include "generateSearchKey.h"
#include "breadthFirstSearch.h"
#include "validation.h"
#include "util.h"
#include "profiler.h"
#include "output.h"

#include <iostream>
#include <mpi.h>
#include <cmath>
#include <string>
#include <cstdlib>

#include <cstdio>
#include <utility>

int main(int argc, char **argv) {

  //Initialize MPI
  MPI::Init(argc, argv);
  int rank, numberOfProcesses;
  rank = MPI::COMM_WORLD.Get_rank(); //get rank
  numberOfProcesses = MPI::COMM_WORLD.Get_size(); //get num of processes

  if (argc != 3) { 
    cout << "[USAGE] ./main <scale> <edgefactor>" << endl;
    exit;
  }

  int scale = atoi(argv[1]); //get scale 
  int edgeFactor = atoi(argv[2]); //get edge factor
  
  int numberOfNodes = pow(2, scale);
  int numberOfEdges = edgeFactor * numberOfNodes;

  // Allocate memory for edge list
  Edges edges(numberOfEdges);

  // Generate random edges on root node
  if(rank == 0) { 
    edges.generate(scale, 10);  // with seed = 10
  }

  // Broadcast edge list to all nodes
  MPI::COMM_WORLD.Bcast(edges.getEdges(), edges.getSize()*2, MPI::LONG_LONG, 0);
    
  // Construct a graph object for this process
  Graph graph(0, numberOfEdges, rank, numberOfProcesses);

  double constructGraphTime;
  if (rank == 0) { 
    cout << "Constructing graph..." << endl; 
  }

  // Construct a graph from the edge list (kernel 1)
  constructGraph(edges, graph);

  if (rank == 0) { 
    cout << "Done." << endl; 
  }

  constructGraphTime = Profiler::getInstance().getTime();

  if (rank == 0) { 
    cout << endl << "Running 64 BFSs..." << endl; 
  }

  long long key = 0;

  // at most 64 times iterations
  int numIters = min(64, graph.getNumberOfGlobalNodes());

  // Allocate memory for global parent list
  long long *globalParent = new long long[graph.getNumberOfGlobalNodes()];

  double *bfsTime = new double[numIters];
  long long int *bfsNumberOfEdges = new long long int[numIters];
  double *bfsTEPS = new double[numIters];

  for (int iter = 0; iter < numIters; iter++) {
    if (rank == 0) {
      // Sample each iteration search key 
      key = generateSearchKey(graph, edges);
    }

    // Broadcast this key to all nodes
    MPI::COMM_WORLD.Bcast(&key, 1, MPI::LONG_LONG, 0);

    // Breadth First Search (kernel 2)
    breadthFirstSearch(key, graph, globalParent);

    // Validate parent list for BFS tree
    if (rank == 0)
    {
      validate(globalParent, key, edges, graph.getNumberOfGlobalNodes());
      double teps = getNumEdges(globalParent, graph.getNumberOfGlobalNodes()) / Profiler::getInstance().getTime();
      bfsNumberOfEdges[iter] = getNumEdges(globalParent, graph.getNumberOfGlobalNodes());
      bfsTime[iter] = Profiler::getInstance().getTime();
      bfsTEPS[iter] = teps;
    }
  }

  if (rank == 0) { 
    printOutput(scale, edgeFactor, numIters, constructGraphTime, bfsTime, bfsNumberOfEdges, bfsTEPS);
  }
  if (rank == 0) { 
    cout << "Done." << endl; 
  }

  MPI::Finalize();
} 