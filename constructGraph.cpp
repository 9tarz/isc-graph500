/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: constructGraph.cpp
  */ 

#include "constructGraph.h"

// Compute number of nodes from edge list
int findNumberOfNodes(Edges &edges) {
  int numberOfNodes = 0;
  for (int i = 0; i < edges.getSize()*2; i++) {
    if (edges.getEdges()[i] > numberOfNodes) {
      numberOfNodes = edges.getEdges()[i]; //update max node number
    }
  }
  numberOfNodes += 1;
  return numberOfNodes;
}

/**
 * Constructs the graph from the edge list
 */
void constructGraph(Edges &edges, Graph &graph) {
  
  // find number from vertices from edge list
  // note: this info is in graph object but this kernel isn't provided that info
  int numberOfNodes = findNumberOfNodes(edges); 
  
  // Store number of nodes in whole graph
  graph.setNumberOfGlobalNodes(numberOfNodes);

  // Break the graph into chunks for each process
  graph.chunkGraph();

  // Allocate memory for Adj Matrix
  graph.allocateAdjMatrix();

  PROFILER_START_EVENT("Construct Graph");
  
  // Build Adj Matrix
  buildAdjMatrix(graph.getAdjMatrix() , graph.getNumberOfGlobalNodes(), edges.getEdges(), edges.getSize(), graph.getNodeOffset(), graph.getNumberOfLocalNodes(), graph.getRank());

  PROFILER_STOP_EVENT("Construct Graph");
}


void buildAdjMatrix(bool *adjMatrix, int numberOfGlobalNodes, long long *edges, int numberOfGlobalEdges, int nodeOffset, int numberOfLocalNodes, int rank) {
  
  #pragma omp parallel for
  for (int index = 0; index < numberOfGlobalEdges; index++) {
    // get the two vertices to connect
    int vertA = edges[index];
    int vertB = edges[index + numberOfGlobalEdges];
    
    // remove self edges
    if (vertA == vertB) { continue; }

    // set edge in both direction
    if (vertA >= nodeOffset && vertA < (nodeOffset + numberOfLocalNodes)) {
      // vertA is row vertB is column
      adjMatrix[(vertA - nodeOffset) * numberOfGlobalNodes + vertB] = true;
    }
    if (vertB >= nodeOffset && vertB < (nodeOffset + numberOfLocalNodes)) {
      // vert b is row vertA is column
      adjMatrix[(vertB - nodeOffset) * numberOfGlobalNodes + vertA] = true;
    }
  }
}


