#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>

using namespace std;

struct Timer {
  Timer() { }
  Timer(int ms) : ms(ms) { }

  //void start() { clock_gettime(0, &start_time); }
  void start() { gettimeofday(&start_time, NULL); }
  Timer &stop() {
    //clock_gettime(0, &end_time);
    gettimeofday(&end_time, NULL);
    ms = Timer::to_ms(end_time) - Timer::to_ms(start_time);
    return *this;
  }
  //static int to_ms(const timespec &tv) { return tv.tv_sec*1000 + tv.tv_nsec/1000000; }
  static int to_ms(const timeval &tv) { return tv.tv_sec*1000 + tv.tv_usec/1000; }

  //timespec start_time;
  //timespec end_time;
  timeval start_time;
  timeval end_time;
  int ms;
};

ostream &operator<<(ostream &out, const Timer &timer);

#endif
