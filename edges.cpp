/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: edges.cpp
  */ 

#include "edges.h"
#include <cmath>

Edges::Edges() :size(0), edges(nullptr)  {}

Edges::Edges(int numberOfEdges) :size(numberOfEdges) {
  edges = new long long[numberOfEdges * 2](); 
}

void Edges::generate(int scale, int seed) {

  // seed prng
  if (!seed) { 
    srand(time(NULL)); 
  }
  else { 
    srand(seed);
  }

  // Set initiator probabilities
  double a = 0.57;
  double b = 0.19;
  double c = 0.19;

  // Random Set vertices for each edge
  for (int i = 0; i < size; ++i) {
    Kronecker(scale, a, b, c, i);
  }
}


void Edges::Kronecker(int scale, double A, double B, double C, int i) {
  long long start = 0;
  long long end = 0;

  // loop over each order of bit
  double ab = A + B;
  double cNorm = C / (1 - ab);
  double aNorm = A / ab;

  for(int ib = 0; ib < scale; ib++) {
    // compare with probabilities and set bits of indices
    int coeff = 1 << uint(ib);

    long long start2 = 0;
    long long end2 = 0;

    // permute vertex labels
    double rand1 = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

    if (rand1 > ab) {
      start2 = 1;
    }

    double rand2 = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

    if (rand2 > (cNorm * double(start2)) + (aNorm * fmod(start2 + 1.0, 2.0))) {
      end2 = 1;
    }

    start = start + coeff * start2;
    end = end + coeff * end2;
  }

  // Set a start vertex
  edges[i] = start;
  // Connect the start vertex to the end vertex
  edges[i + size] = end;
}