#ifndef __STATS_H__
#define __STATS_H__

#include "std.h"
#include "stl-basic.h"
#define DBL_MAX 1e300
#define DBL_MIN (-1e300)

struct StatFig {
  StatFig() { clear(); }
  StatFig(double sum, int n) : sum(sum), n(n) { }
  virtual ~StatFig() { }

  static double F1(const StatFig &fig1, const StatFig &fig2) {
    if(fig1.n == 0 || fig2.n == 0) return 0;
    return 2*fig1.val()*fig2.val() / (fig1.val()+fig2.val());
  }

  void add()         { add(1); }
  virtual void add(double v) { sum += v; n++; }
  virtual void clear() { sum = n = 0; }
  int size() const { return n; }
  double val() const { return sum / n; }
  double mean() const { return sum / n; }
  double sum;
  int n;
};

inline ostream &operator<<(ostream &out, const StatFig &fig) {
  return out << fig.sum << '/' << fig.n << '=' << fig.val();
}

////////////////////////////////////////////////////////////
// Stores the min and the amx

struct BigStatFig : public StatFig {
  BigStatFig() { clear(); }
  void add(double v) { if(v < min) min = v; if(v > max) max = v; StatFig::add(v); }
  void clear() { min = DBL_MAX; max = DBL_MIN; StatFig::clear(); }
  double min, max;
};

inline ostream &operator<<(ostream &out, const BigStatFig &fig) {
  return out << fig.n << ':' << fig.min << "/<< "  << fig.val() << " >>/" << fig.max;
}

////////////////////////////////////////////////////////////
// Stores the standard deviation (and all points)

struct FullStatFig : public BigStatFig {
  FullStatFig() { clear(); }
  virtual ~FullStatFig() { }
  void add(double v) { data.push_back(v); BigStatFig::add(v); }
  void clear() { data.clear(); BigStatFig::clear(); }

  double variance() const {
    double var = 0, mean = val();
    forvec(_, double, v, data) var += sq(v-mean);
    var /= n;
    return var;
  }
  double stddev() const { return sqrt(variance()); }

  DoubleVec data;
};

inline ostream &operator<<(ostream &out, const FullStatFig &fig) {
  return out << (BigStatFig)fig << '~' << fig.stddev();
}

#endif
