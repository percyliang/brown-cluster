#ifndef __LISP_H__
#define __LISP_H__

#include <vector>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////

struct LispNode {
  void destroy();
  void print(int ind) const;

  string value;
  vector<LispNode *> children;
};

////////////////////////////////////////////////////////////

struct LispTree {
  LispTree() : root(NULL) { }
  ~LispTree();

  bool read_token(istream &in, string &s);
  LispNode *read_node(const vector<string> &tokens, int &i);
  void read(const char *file);
  void print() const;

  LispNode *root;
};

#endif
