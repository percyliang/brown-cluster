#ifndef __STR_STR_DB_H__
#define __STR_STR_DB_H__

#include "stl-basic.h"

// Maps strings (s) to strings (t).
class StrStrDB {
public:
  ~StrStrDB();

  void read(const char *file);
  const char *operator[](const char *s) const;

  int max_t_len;
private:
  StrStrMap s2t;
};

#endif
