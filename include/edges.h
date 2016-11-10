/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: edges.h
  */ 

#ifndef EDGES_H
#define EDGES_H

#include <cstdlib>
#include <ctime>

using namespace std;

class Edges {
 
 public:

  // Edges Constructor with empty numberOfEdges
  Edges();

  // Edges Constructor with numberOfEdges
  Edges(int numberOfEdges);

  // Generate edges 
  void generate(int scale, int seed = 0);
  void Kronecker(int scale, double A, double B, double C, int i);

  // Destructor
  ~Edges() {
    if(edges != nullptr) 
      delete[] edges; 
  }

  // Get property of Edges 
  int getSize() const { return this->size; }

  long long* getEdges() const { return this->edges; }

  Edges& operator=(Edges &&edges) = delete;
  Edges& operator=(const Edges &edges) = delete;
  Edges(Edges &&edges) = delete;
  Edges(const Edges &edges) = delete;
  
 private:
  long long *edges;
  int size;

};

#endif // EDGES_H