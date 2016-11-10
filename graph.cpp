/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: graph.cpp
  */

#include "graph.h"
#include <cstring>

// Graph constructor

Graph::Graph(int numberOfGlobalNodes, int numberOfGlobalEdges, int rank, int numberOfProcesses):
  numberOfGlobalNodes(numberOfGlobalNodes), 
  numberOfGlobalEdges(numberOfGlobalEdges),
  nodeOffset(0), 
  numberOfLocalNodes(0), 
  adjMatrix(nullptr), 
  rank(rank), 
  numberOfProcesses(numberOfProcesses) {}

// Set number of global nodes of graph
void Graph::setNumberOfGlobalNodes(int numberOfNodes) { numberOfGlobalNodes = numberOfNodes;}

/** 
 * calculates numLocalNodes & nodeOffset
 */
void Graph::chunkGraph() {
  numberOfLocalNodes = numberOfGlobalNodes / numberOfProcesses;
  int leftover = numberOfGlobalNodes % numberOfProcesses;

  if (rank < leftover) {
    numberOfLocalNodes += 1;
    nodeOffset = rank * numberOfLocalNodes;
  }
  else {
    nodeOffset = rank * numberOfLocalNodes + leftover;
  }
}

void Graph::allocateAdjMatrix() {
  adjMatrixSize = numberOfLocalNodes * numberOfGlobalNodes;
  adjMatrix = new bool[adjMatrixSize];
  memset((void *) adjMatrix, 0, adjMatrixSize);
}

