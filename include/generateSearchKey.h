/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: generateSearchKey.h
  */ 
  
#ifndef GENERATE_SEARCH_KEY_H
#define GENERATE_SEARCH_KEY_H

#include "edges.h"
#include "graph.h"

/**
 * generates a random search key in the graph
 */
long long generateSearchKey(const Graph &graph, const Edges &edges);

#endif // GENERATE_SEARCH_KEY_H
