/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: util.h
  */ 

#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>
#include <string>
#include <mpi.h>
#include <cstdint>
#include <iostream>

using namespace std;

/** 
 * stores error string to indicate what caused error
 */
class Error
{
public:
  // constructor
  // sets error string
 Error(const string &string) : error(string) {}

  // returns error string
  string GetError() { return this->error; }

private:
  // error string
  string error;
};

// generic exception wrapper
#define LOG_ERROR(msg) {int rank = MPI::COMM_WORLD.Get_rank();     \
    Error err(string(__FILE__) + "(" + to_string(__LINE__) +  \
              "): " + "rank(" + to_string(rank) + ") "+ msg); \
    throw err;}

// printing helper
#define LOG_INFO(msg) {cout << __FILE__ << "(" << to_string(__LINE__) \
                            << "): " << msg << endl;}


#endif // UTIL_H