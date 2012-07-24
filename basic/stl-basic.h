#ifndef __STL_BASIC_H__
#define __STL_BASIC_H__

#include "std.h"
#include "city.h"

////////////////////////////////////////////////////////////

typedef double real;
//typedef float real;

typedef pair<int, int> IntPair;
typedef pair<int, real> IntDouble;
typedef pair<real, int> DoubleInt;
typedef pair<real, real> DoublePair;
typedef vector<IntPair> IntPairVec;
typedef vector<DoubleInt> DoubleIntVec;
typedef vector<bool> BoolVec;
typedef vector<int> IntVec;
typedef vector<string> StringVec;
typedef vector<IntVec> IntMat;
typedef vector<IntVec> IntVecVec;
typedef vector<IntVecVec> IntVecVecVec;
typedef vector<IntVecVecVec> IntVecVecVecVec;
typedef vector<real> DoubleVec;
typedef vector<DoubleVec> DoubleVecVec;
typedef vector<DoubleVecVec> DoubleVecVecVec;
typedef vector<DoubleVecVecVec> DoubleVecVecVecVec;
typedef vector<IntDouble> IntDoubleVec;
typedef vector<IntDoubleVec> IntDoubleVecVec;
typedef vector<IntDoubleVecVec> IntDoubleVecVecVec;
typedef vector<IntDoubleVecVecVec> IntDoubleVecVecVecVec;

typedef IntVec ivector;
typedef DoubleVec fvector;
typedef DoubleVecVec fmatrix;

////////////////////////////////////////////////////////////

struct vector_eq {
  bool operator()(const IntVec &v1, const IntVec &v2) const {
    return v1 == v2;
  }
};
struct vector_hf {
  size_t operator()(const IntVec &v) const {
    return CityHash64(reinterpret_cast<const char*>(&v[0]), sizeof(int) * v.size());
#if 0
    int h = 0;
    foridx(i, len(v))
      h = (h<<4)^(h>>28)^v[i];
    return h;
#endif
  }
};

struct pair_eq {
  bool operator()(const IntPair &p1, const IntPair &p2) const {
    return p1 == p2;
  }
};
struct pair_hf {
  size_t operator()(const IntPair &p) const {
    return (p.first<<4)^(p.first>>28) ^ p.second;
  }
};

struct str_eq {
  bool operator()(const char *s1, const char *s2) const {
    return strcmp(s1, s2) == 0;
  }
};
struct str_hf {
  size_t operator()(const char *s) const {
    return CityHash64(s, strlen(s));
  }
};

struct string_eq {
  bool operator()(const string &s1, const string &s2) const {
    return s1 == s2;
  }
};
struct string_hf {
  size_t operator()(const string &s) const {
    return CityHash64(s.c_str(), s.size());
  }
};

////////////////////////////////////////////////////////////

typedef unordered_set<int> IntSet;
typedef unordered_set<IntPair, pair_hf, pair_eq> IntPairSet;
typedef unordered_set<IntVec, vector_hf, vector_eq> IntVecSet;
typedef unordered_map<IntVec, real, vector_hf, vector_eq> IntVecDoubleMap;
typedef unordered_map<IntVec, int, vector_hf, vector_eq> IntVecIntMap;
typedef unordered_map<int, int> IntIntMap;
typedef unordered_map<int, real> IntDoubleMap;
typedef unordered_map<int, IntPair> IntIntPairMap;
typedef unordered_map<int, IntVec> IntIntVecMap;
typedef unordered_map<int, IntIntMap> IntIntIntMapMap;
typedef unordered_map<IntPair, int, pair_hf, pair_eq> IntPairIntMap;
typedef unordered_map<IntPair, real, pair_hf, pair_eq> IntPairDoubleMap;
typedef unordered_map<IntPair, DoubleVec, pair_hf, pair_eq> IntPairDoubleVecMap;
typedef unordered_map<IntVec, IntVec, vector_hf, vector_eq> IntVecIntVecMap;
typedef unordered_map<IntVec, DoubleVec, vector_hf, vector_eq> IntVecDoubleVecMap;
typedef vector<IntIntMap> IntIntMapVec;

typedef vector<const char *> StrVec;
typedef unordered_map<const char *, int, str_hf, str_eq> StrIntMap;
typedef unordered_map<const char *, const char *, str_hf, str_eq> StrStrMap;

#endif
