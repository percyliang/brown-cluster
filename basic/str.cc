#include "stl-basic.h"
#include <stdarg.h>

string substr(const string &s, int i, int j) {
  if(i < 0) i += len(s);
  if(j < 0) j += len(s);
  i = max(i, 0);
  j = max(j, i);
  return s.substr(i, j-i);
}
string substr(const string &s, int i) {
  return substr(s, i, len(s));
}

string str_printf(const char *fmt, ...) {
  char buf[16384];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  return buf;
}

char *copy_str(const char *s) {
  char *t = new char[strlen(s)+1];
  strcpy(t, s);
  return t;
}

string int2str(int x) {
  return str_printf("%d", x);
}

string double2str(double x) {
  ostringstream os;
  os << x;
  return os.str();
}

StringVec split(const char *str, const char *delims, bool keep_empty) {
  StringVec vec; // Store the result.
  // Build quick lookup table.
  BoolVec is_delim(256);
  for(const char *p = delims; *p; p++) is_delim[*p] = true;
  is_delim['\0'] = true;

  const char *end = str;
  while(true) {
    if(is_delim[*end]) {
      if(keep_empty || end-str > 0) // Extract token.
        vec.push_back(string(str, end-str)); 
      str = end+1;
    }
    if(!*end) break;
    end++;
  }
  return vec;
}

StrVec mutate_split(char *str, const char *delims) {
  StrVec vec;
  for(char *p = strtok(str, delims); p; p = strtok(NULL, delims))
    vec.push_back(p);
  return vec;
}

// Remove leading and trailing white space.
char *trim(char *s) {
  // Removing leading spaces.
  while(*s && isspace(*s)) s++;

  // Remove trailing spaces.
  char *t;
  for(t = s+strlen(s)-1; t != s && isspace(*t); t--);
  t[1] = '\0';
  return s;
}

string tolower(const char *s) {
  string t = s;
  foridx(i, len(t)) t[i] = tolower(t[i]);
  return t;
}

// String matching with brute force.
int index_of(const char *s, const char *t) {
  int ns = strlen(s), nt = strlen(t);
  foridx(i, ns-nt+1)
    if(strncmp(s+i, t, nt) == 0) return i;
  return -1;
}
