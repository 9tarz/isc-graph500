/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: profiler.h
  */ 

#ifndef PROFILER_H
#define PROFILER_H

#include "util.h"

#include <map>
#include <ctime>
#include <string>

using namespace std;

/**
 * class used to time functions
 */
class Profiler {

  public:

    static Profiler& getInstance() {
      static Profiler prof;

      return prof;
    }

    void startEvent(const string event) {
      // crete time stamp for this event
      times_[event] = clock();
    }

    void stopEvent(const string event) {
      // calculate time usage and print
      clock_t end = clock();
      if(times_.find(event) == times_.end()) {
        LOG_ERROR("Event \"" + event + "\" does not exist");
      }
      double elapsedTime = double(end - times_[event]) / CLOCKS_PER_SEC;
      
      // save time
      measureTime_ = elapsedTime;
    }

    double getTime() {
      return measureTime_;
    }

  private:
    // should never be instantiated
    Profiler() { 
    }

    ~Profiler() {}

    map<string, clock_t> times_;

    double measureTime_;
};

// profiler macros
#define PROFILER_START_EVENT(string) { Profiler::getInstance().startEvent(string); }

#define PROFILER_STOP_EVENT(string) { Profiler::getInstance().stopEvent(string); }

#endif // PROFILER_H
