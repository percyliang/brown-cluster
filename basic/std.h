#ifndef __STD_H__
#define __STD_H__

#include <assert.h>
#include <math.h>
#include <stdlib.h>
//#include <values.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

////////////////////////////////////////////////////////////

#define len(vec) (int)(vec).size()
#define sq(x) ((x)*(x))

// For loop sugar.  This is such a hack!
#define foridx(i, n)                  for(int i = 0; i < n; i++)
#define forvec(i, tx, x, vec)         for(int i = 0, _##i = 0; i < len(vec); i++) \
                                      for(tx x = (vec)[i]; i == _##i; _##i++)
#define formap(tx, x, ty, y, t, map)  forstl(t, _##x##y, map) _mapvars(tx, x, ty, y)
#define forcmap(tx, x, ty, y, t, map) forcstl(t, _##x##y, map) _mapvars(tx, x, ty, y)
#define forstl(t, x, container)       for(t::iterator x = (container).begin(); x != (container).end(); x++)
#define forcstl(t, x, container)      for(t::const_iterator x = (container).begin(); x != (container).end(); x++)
#define _mapvars(tx, x, ty, y)        for(tx x = _##x##y->first, *_##x = &x; _##x; _##x = NULL) \
                                      for(ty y = _##x##y->second, *_##y = &y; _##y; _##y = NULL)

////////////////////////////////////////////////////////////
// Generate random numbers.

inline int mrand(int a)        { return rand() % a; }
inline int mrand(int a, int b) { return rand() % (b-a) + a; }
inline double rand_double() {
  static const int BASE = 100000;
  return (double)(rand()%BASE)/BASE;
}

////////////////////////////////////////////////////////////
// Floating point stuff.

const double TOL = 1e-10;

inline bool flt(double u, double v) { return u + TOL < v; } 
inline bool fgt(double u, double v) { return u - TOL > v; } 

// Comparing floating point numbers.
inline bool feq(double u, double v, double tol = TOL) { return fabs(u-v) < tol; }

template <class T> inline int sign(T u) {
  if(u < 0) return -1;
  if(u > 0) return 1;
  return 0;
}

#define assert_feq(u, v) do { _assert_feq(u, v, __FILE__, __LINE__); } while(0);
#define assert_feq2(u, v, tol) do { _assert_feq(u, v, tol, __FILE__, __LINE__); } while(0);
#define assert_fneq(u, v) do { _assert_fneq(u, v, __FILE__, __LINE__); } while(0);
inline void _assert_feq(double u, double v, const char *file, int line) {
  if(!feq(u, v)) { printf("At %s:%d, %f != %f\n", file, line, u, v); assert(0); }
}
inline void _assert_feq(double u, double v, double tol, const char *file, int line) {
  if(!feq(u, v, tol)) { printf("At %s:%d, %f != %f\n", file, line, u, v); assert(0); }
}
inline void _assert_fneq(double u, double v, const char *file, int line) {
  if(feq(u, v)) { printf("At %s:%d, %f == %f\n", file, line, u, v); assert(0); }
}
#define assert_eq(u, v) do { _assert_eq(u, v, __STRING(u), __STRING(v), __FILE__, __LINE__); } while(0)
template<class T> inline void _assert_eq(const T &u, const T &v, const char *us, const char *vs, const char *file, int line) {
  if(u != v) {
    cout << "At " << file << ':' << line << ", " <<
            us << '(' << u << ')' << " != " <<
            vs << '(' << v << ')' << endl;
    assert(0);
  }
}

#define assert2(x, reason) \
  do { \
    if(!(x)) { \
      cout << "\nFAILURE REASON: " << reason << endl; \
      assert(x); \
    } \
  } while(0)

string now();
string hostname();
int cpu_speed_mhz();
int mem_usage(); // in kB

bool create_file(const char *file);
bool file_exists(const char *file);
time_t file_modified_time(const char *file);

string strip_dir(string s);
string get_dir(string s);
string file_base(string s);
bool get_files_in_dir(string dirname, bool fullpath, vector<string> &files);

#endif
