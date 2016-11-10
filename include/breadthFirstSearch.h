/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: breadthFirstSearch.h
  */ 

#ifndef BREADTH_FIRST_SEARCH_H
#define BREADTH_FIRST_SEARCH_H

#include "graph.h"
#include <mpi.h>
#include "profiler.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <omp.h>
  
using std::ceil;

/**
 * performs breadth first search startin at the input key
 *
 * args:
 *   - key: search key to start the bfs at (node ID)
 *   - graph: graph object to search over
 *   - parent: array of len == graph.numGlobalNodes. Stores 
 * results of bfs
 */
void breadthFirstSearch(long long key, const Graph &graph, long long *globalParent);

#endif // BREADTH_FIRST_SEARCH
