#include "timer.h"

ostream &operator<<(ostream &out, const Timer &timer) {
  int ms = timer.ms;
  int m = ms / 60000; ms %= 60000;
  int h = m / 60; m %= 60;
  if(h > 0) out << h << 'h';
  if(h > 0 || m > 0) out << m << 'm';
  out << ms/1000.0 << 's';
  return out;
}
