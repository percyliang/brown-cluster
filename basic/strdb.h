#ifndef __STRDB_H__
#define __STRDB_H__

#include "std.h"
#include "stl-basic.h"
#include "stl-utils.h"
#include "logging.h"

void destroy_strings(StrVec &vec);
void destroy_strings(StrStrMap &map);

// Map between strings and integers.
// Strings must not have spaces in them.
// File format: strings, one per line.  Assume strings are distinct.
struct StrDB {
  StrDB() { }
  ~StrDB() { destroy_strings(); }

  int read(istream &in, int n, bool one_way);
  int read(const char *file, bool one_way);

  void write(ostream &out);
  void write(const char *file);

  int size() const   { return len(i2s); }
  void clear()       { destroy_strings(); i2s.clear(); s2i.clear(); }
  void destroy()     { destroy_strings(); ::destroy(i2s); ::destroy(s2i); }
  void destroy_s2i() { ::destroy(s2i); }
  void clear_keep_strings() { i2s.clear(); s2i.clear(); }

  const char *operator[](int i) const;
  int operator[](const char *s) const;
  int operator[](const char *s);
  int lookup(const char *s, bool incorp_new, int default_i);

  IntVec lookup(const StrVec &svec);

  bool exists(const char *s) const { return s2i.find(s) != s2i.end(); }

  // /usr/bin/top might not show the memory reduced.
  void destroy_strings() { ::destroy_strings(i2s); }

  StrVec i2s;
  StrIntMap s2i;
};

ostream &operator<<(ostream &out, const StrDB &db);

////////////////////////////////////////////////////////////

// Map between IntPairs and ints.
struct IntPairIntDB {
  IntPair operator[](int i) const { return i2p[i]; }
  int operator[](const IntPair &p) { return lookup(p, true, -1); }
  int lookup(const IntPair &p, bool incorp_new, int default_i);
  int size() const { return len(i2p); }

  int read(istream &in, int N);
  void write(ostream &out);

  IntPairIntMap p2i;
  IntPairVec i2p;
};

////////////////////////////////////////////////////////////

// Map between IntVecs and ints.
struct IntVecIntDB {
  const IntVec &operator[](int i) const { return i2v[i]; }
  int operator[](const IntVec &v) { return lookup(v, true, -1); }
  int lookup(const IntVec &v, bool incorp_new, int default_i);
  int size() const { return len(i2v); }

  IntVecIntMap v2i;
  IntVecVec i2v;
};

////////////////////////////////////////////////////////////

#if 0
// Map between IntArrays and ints.  Arrays terminate with -1.
struct IntArrayIntDB {
  int *operator[](int i) const { return i2a[i]; }
  int operator[](const IntArray &a) { return lookup(a, true, -1); }
  int lookup(const IntArray &a, bool incorp_new, int default_i);
  int size() const { return len(i2a); }

  int read(istream &in, int N);
  void write(ostream &out);

  hash_map<int *, int, intarray_hf, intarray_eq> p2i;
  vector<int *> i2a;
};
#endif

////////////////////////////////////////////////////////////

typedef void int_func(int a);
void read_text(const char *file, int_func *func, StrDB &db, bool read_cached, bool write_cached, bool incorp_new);

#endif
