/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: graph.h
  */ 

#ifndef GRAPH_H
#define GRAPH_H

#include <cstdlib>
#include <cstring>

/**
 * Stores graph information and data
 */
class Graph {

 public:
  
  // Graph constructor
  Graph(int numberOfGlobalNodes, int numberOfGlobalEdges, int rank, int numberOfProcesses);

  // Get all property of graph

  int getNumberOfGlobalNodes() const { return numberOfGlobalNodes; }
  int getNumberOfGlobalEdges() const { return numberOfGlobalEdges; }
  int getNumberOfLocalNodes() const { return numberOfLocalNodes; }
  int getNodeOffset() const { return nodeOffset; }

  int getRank() const { return rank; }
  int getNumberOfProcesses() const { return numberOfProcesses; }

  bool* getAdjMatrix() const { return adjMatrix; }
  size_t getAdjMatrixSize() const { return adjMatrixSize; }
  
  // Set number of global nodes of graph
  void setNumberOfGlobalNodes(int numberOfNodes);

  // Chunk graph : calculate number of local nodes and node offset
  void chunkGraph();

  void allocateAdjMatrix();

  /**
   * copy constructor and copy assigment operators
   */
  Graph& operator=(Graph &&graph) = delete;
  Graph& operator=(const Graph &graph) = delete;
  Graph(Graph &&graph) = delete;
  Graph(const Graph &graph) = delete;

 private:

  /**
   *global info
   */
  int numberOfGlobalNodes; // (2^scale)
  int numberOfGlobalEdges; // (edgefactor* numberOfGlobalNodes)
  int rank; // rank of this proc
  int numberOfProcesses; // number of procs

  /**
   * local info
   */
  int nodeOffset; // offset of the first node this rank is responsible for
  int numberOfLocalNodes; // number of nodes this rank is responsible for
  bool *adjMatrix; // ptr to adjacency matrix in device memory
  size_t adjMatrixSize;
  
};

#endif // GRAPH_H
