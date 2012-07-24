#ifndef __STR_H__
#define __STR_H__

#include "stl-basic.h"

string substr(const string &s, int i, int j);
string substr(const string &s, int i);

string str_printf(const char *fmt, ...);
char *copy_str(const char *s);
string int2str(int x);
string double2str(double x);

StringVec split(const char *str, const char *delims, bool keep_empty);
StrVec mutate_split(char *str, const char *delims);

char *trim(char *s);
string tolower(const char *s);

int index_of(const char *s, const char *t);

#endif
