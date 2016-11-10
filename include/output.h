/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: output.h
  */ 

#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <algorithm>

using namespace std;

void printOutput(int SCALE, int edgefactor, int NBFS, double kernel_1_time, double *kernel_2_time, long long int *kernel_2_nedge, double *kernel_2_TEPS);

#endif // OUTPUT
