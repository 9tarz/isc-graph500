/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: constructGraph.h
  */ 

#ifndef CONSTRUCT_GRAPH_H
#define CONSTRUCT_GRAPH_H

#include <utility>
#include <cmath>

#include "edges.h"
#include "graph.h"
#include "profiler.h"
#include <omp.h>

using namespace std;

void constructGraph(Edges &edges, Graph &graph);
int findNumberOfNodes(Edges &edges);
void buildAdjMatrix(bool *adjMatrix, int numberOfGlobalNodes, long long *edges, int numberOfGlobalEdges, int nodeOffset, int numberOfLocalNodes, int rank);

#endif // CONSTRUCT_GRAPH_H