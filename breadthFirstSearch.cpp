/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: breadthFirstSearch.cpp
  */ 

#include "breadthFirstSearch.h"

/**
 * Performs BFS on the input graph from the input search key
 */
void breadthFirstSearch(long long key, const Graph &graph, long long *globalParent) {
  
  int numberOfGlobalNodes = graph.getNumberOfGlobalNodes();
  int numberOfLocalNodes = graph.getNumberOfLocalNodes();
  int nodeOffset = graph.getNodeOffset();
  bool *adjMatrix = graph.getAdjMatrix();

  // Set global parent to -1's
  memset((void *) globalParent, -1, sizeof(long long) * numberOfGlobalNodes);

  // Allocate memory for visited flags 
  int numberOfVisited = numberOfGlobalNodes + 1;

  bool *visited = new bool[numberOfVisited];
  memset((void *) visited, 0, sizeof(bool) * numberOfVisited);

  // Mark starting node index as visited
  visited[key] = true;

  // Allocate parent on each node
  long long *localParent = new long long[numberOfGlobalNodes];
  memcpy(localParent, globalParent, sizeof(long long) * numberOfGlobalNodes);

  // Indicates if this vertex has already been visited and processed
  bool *done = new bool[numberOfLocalNodes];
  memset((void *) done, 0, sizeof(bool) * numberOfLocalNodes);

  // start timer
  PROFILER_START_EVENT("BFS");

  int numberOfProcessors = 8;
  int chunkSize = numberOfLocalNodes/numberOfProcessors;

  // step through bfs until complete
  do {
    // unset work flag
    visited[numberOfGlobalNodes] = 0;

    // executed step
    #pragma omp parallel for schedule(static, chunkSize)

    for(int index = 0 ; index < numberOfLocalNodes ; index++){
      int globalIndex = index + nodeOffset;  
      // if my node is visited but not completed
      if (visited[globalIndex] && !done[index]) {
        // visited all my children
        for (int nodeId = 0; nodeId < numberOfGlobalNodes; nodeId++) {
          // if there is an edge && child hasn't been visited
          if (adjMatrix[index*numberOfGlobalNodes + nodeId] && !visited[nodeId]) {
            // visit this node
            visited[nodeId] = true;
            // mark me as it's parent
            localParent[nodeId] = globalIndex;
          }
        }
        // mark this node as done
        done[index] = true;
        // indicate work was done
        visited[numberOfGlobalNodes] = true;

      }
    }

    // update global visited information
    MPI::COMM_WORLD.Allreduce(MPI::IN_PLACE, 
                              visited,
                              numberOfVisited, 
                              MPI::BOOL,
                              MPI::LOR);
  
} while(visited[numberOfVisited - 1]);

  // copy parent back from each node
  memcpy(globalParent, localParent, sizeof(long long) * numberOfGlobalNodes);

  // reduce max to get parent info on master
  if (graph.getRank() == 0) {
    MPI::COMM_WORLD.Reduce(MPI::IN_PLACE,  
                           globalParent, 
                           numberOfGlobalNodes, 
                           MPI::LONG_LONG, 
                           MPI::MAX,
                           0); 
  }
  else {
    MPI::COMM_WORLD.Reduce(globalParent,
                           globalParent,
                           numberOfGlobalNodes,
                           MPI::LONG_LONG,
                           MPI::MAX,
                           0);
  }
  
  PROFILER_STOP_EVENT("BFS");
} 
