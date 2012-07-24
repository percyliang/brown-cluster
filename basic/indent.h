#ifndef __INDENT_H__
#define __INDENT_H__

#include <iostream>

using namespace std;

struct Indent {
  Indent(int level) : level(level) { }
  int level;
};

inline ostream &operator<<(ostream &out, const Indent &ind) {
  for(int i = 0; i < ind.level; i++) out << "  ";
  return out;
}

#endif
