#ifndef __MEM_TRACKER_H__
#define __MEM_TRACKER_H__

#include "std.h"
#include "stl-basic.h"
#include "union-set.h"
#include "strdb.h"

// Currently, memory tracking is not accurate.
// Alway underestimates.

// Call this function.  Don't use anything else.
#define track_mem(x) mem_tracker.add(__STRING(x), x)

#define list_types(f) \
  f(IntVec) \
  f(IntMat) \
  f(IntIntMap) \
  f(IntDoubleMap) \
  f(IntIntPairMap) \
  f(IntPairDoubleMap) \
  f(IntSet) \
  f(DoubleVec) \
  f(DoubleVecVec) \
  f(StrVec) \
  f(StrIntMap) \
  f(UnionSet) \
  f(StrDB)

#define prefix_t(type) T_##type,
#define define_str(type) __STRING(type),
#define define_add(type) \
  void add(const char *name, const type &data) { \
    records.push_back(MemRecord(name, T_##type, &data)); \
  }
#define define_case(type) \
  case T_##type: return mem_usage(*((const type *)r.data));

enum MemType { T_RAWNUMBER, list_types(prefix_t) };

struct MemRecord {
  MemRecord(const char *name, int mem) :
    name(name), type(T_RAWNUMBER), data(NULL), mem(mem) { }
  MemRecord(const char *name, MemType type, const void *data) :
    name(name), type(type), data(data), mem(0) { }
  string name;
  MemType type;
  const void *data;
  int mem;
};

// Track amount of memory used.
class MemTracker {
public:
  static const char *type_names[];

  list_types(define_add)

  void add(const char *name, int mem) {
    records.push_back(MemRecord(name, mem));
  }

  int compute_mem_usage(const MemRecord &r);
  int compute_mem_usage();
  void report_mem_usage();

private:
  vector<MemRecord> records;
};

extern MemTracker mem_tracker;

////////////////////////////////////////////////////////////
// Various mem_usage() functions on various data types.

template<class T> int mem_usage(const vector< vector< vector< vector<T> > > > &mat) { // matrix
  int mem = 0;
  foridx(i, len(mat)) {
    foridx(j, len(mat[i])) {
      foridx(k, len(mat[i][j]))
        mem += len(mat[i][j][k]) * sizeof(T);
      mem += len(mat[i][j]) * sizeof(vector<T>);
    }
    mem += len(mat[i]) * sizeof(vector<T>);
  }
  mem += len(mat) * sizeof(vector<T>);
  return mem;
}

template<class T> int mem_usage(const vector< vector< vector<T> > > &mat) { // matrix
  int mem = 0;
  foridx(i, len(mat)) {
    foridx(j, len(mat[i]))
      mem += len(mat[i][j]) * sizeof(T);
    mem += len(mat[i]) * sizeof(vector<T>);
  }
  mem += len(mat) * sizeof(vector<T>);
  return mem;
}

template<class T> int mem_usage(const vector< vector<T> > &mat) { // matrix
  int mem = 0;
  foridx(i, len(mat))
    mem += len(mat[i]) * sizeof(T);
  mem += len(mat) * sizeof(vector<T>);
  return mem;
}

template<class T> int mem_usage(const vector<T> &vec) { // vector
  return len(vec) * sizeof(T);
}

template<class T> int mem_usage(const unordered_set<T> &set) { // hash_set
  return (int)set.bucket_count()*4 + len(set)*(sizeof(T)+sizeof(void *));
}

template<class Tx, class Ty, class Hf, class Eq> int mem_usage(const unordered_map<Tx, Ty, Hf, Eq> &map) { // hash_map
  return (int)map.bucket_count()*4 + len(map)*(sizeof(Tx)+sizeof(Ty)+sizeof(void *));
}

inline int mem_usage(const UnionSet &u) { // UnionSet
  return mem_usage(u.parent);
}

inline int mem_usage(const StrDB &db) { // StrDB
  int mem = mem_usage(db.s2i) + mem_usage(db.i2s);
  foridx(i, len(db))
    mem += (strlen(db[i])+1) * sizeof(char);
  return mem;
}

#endif
