#include "lisp.h"
#include "std.h"
#include "indent.h"

void LispNode::destroy() {
  forvec(_, LispNode *, node, children) {
    node->destroy();
    delete node;
  }
}

void LispNode::print(int ind) const {
  cout << Indent(ind) << (value.empty() ? "(empty)" : value) << endl;
  forvec(_, LispNode *, subnode, children)
    subnode->print(ind+1);
}

////////////////////////////////////////////////////////////

LispTree::~LispTree() {
  root->destroy();
  delete root;
}

bool is_paren(char c) {
  return c == '(' || c == ')' || c == '[' || c == ']';
}
bool is_paren(string s) {
  return s == "(" || s == ")" || s == "[" || s == "]";
}
bool is_left_paren(string s) {
  return s == "(" || s == "[";
}
bool is_right_paren(string s) {
  return s == ")" || s == "]";
}
string matching_right_paren(char c) {
  if(c == '(') return ")";
  if(c == '[') return "]";
  return "";
}

// Return first non-space character.
char skip_space(istream &in) {
  char c;
  while(true) {
    c = in.peek();
    if(!isspace(c)) break;
    in.get();
  }
  return c;
}

// Comments start with # and end with the line.
// There must be a space before the #.
char skip_comments(istream &in) {
  while(true) {
    char c = skip_space(in);
    if(c == '#')
      while((c = in.peek()) != '\n') in.get();
    else
      return c;
  }
}

bool LispTree::read_token(istream &in, string &s) {
  char c = skip_comments(in);

  if(is_paren(c)) {
    s = in.get();
    return true;
  }

  s = "";
  while(true) {
    c = in.peek();
    if(c == EOF) return false;
    if(isspace(c) || is_paren(c)) break;
    s += in.get();
  }

  return true;
}

LispNode *LispTree::read_node(const vector<string> &tokens, int &i) {
  LispNode *node = new LispNode();
  assert(i < len(tokens));

  string s = tokens[i++];
  if(is_left_paren(s)) {
    char left_paren = s[0];

    if(left_paren == '(') {
      assert(i < len(tokens) && !is_paren(tokens[i]));
      node->value = tokens[i++];
    }

    while(i < len(tokens) && !is_right_paren(tokens[i])) {
      node->children.push_back(read_node(tokens, i));
    }

    assert(i < len(tokens));
    s = tokens[i++];
    assert(s == matching_right_paren(left_paren));
  }
  else if(is_right_paren(s))
    assert(false);
  else
    node->value = s;

  return node;
}

void LispTree::read(const char *file) {
  ifstream in(file);
  vector<string> tokens;
  string token;
  while(read_token(in, token)) {
    tokens.push_back(token);
  }
  int i = 0;
  root = read_node(tokens, i);
  assert(i == len(tokens));
}

void LispTree::print() const {
  assert(root);
  root->print(0);
}
