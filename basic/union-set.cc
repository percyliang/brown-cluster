#include "union-set.h"

void UnionSet::Init(int n) {
  parent.resize(n);
  for(int v = 0; v < n; v++)
    parent[v] = v;
}

// return whether u and v are in the same connected component;
// connect them if they aren't
bool UnionSet::Do(int u, int v, bool doit) {
  int ru = GetRoot(u);
  int rv = GetRoot(v);
  if(ru == rv) return true;
  if(doit) parent[ru] = rv;
  return false;
}

int UnionSet::GetRoot(int v) {
  int rv = v;
  while(parent[rv] != rv)
    rv = parent[rv];
  while(v != rv) {
    int pv = parent[v];
    parent[v] = rv;
    v = pv;
  }
  return rv;
}
