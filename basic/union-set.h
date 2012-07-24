#ifndef __UNION_SET_H__
#define __UNION_SET_H__

#include <vector>

using namespace std;

struct UnionSet {
  UnionSet() { } 
  UnionSet(int n) { Init(n); }
  void Init(int n);

  bool Join(int u, int v) { return Do(u, v, true); }
  bool InSameSet(int u, int v) { return Do(u, v, false); }

  bool Do(int u, int v, bool doit);
  int GetRoot(int v);

  vector<int> parent;
};

#endif
