/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: generateSearchKey.cpp
  */

#include "generateSearchKey.h"

long long generateSearchKey(const Graph &graph, const Edges &edges) {
  bool cont = true;
  int numberOfNodes = graph.getNumberOfGlobalNodes();
  long long key = 0;

  while (cont) {

    // generate random key
    key = rand() % numberOfNodes;

    // confirm this node has at least one edge
    for (int i = 0; i < edges.getSize(); ++i) {
      int vertA = edges.getEdges()[i];
      int vertB = edges.getEdges()[i + edges.getSize()];
      if ((vertA != vertB) && (vertA == key || vertB == key)) {
        cont = false;
        break;
      }
    }
  }

  return key;
}
