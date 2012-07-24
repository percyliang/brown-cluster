#ifndef __OPT_H__
#define __OPT_H__

#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

// First thing to call in main().
void init_opt(int argc, char *argv[]);

////////////////////////////////////////////////////////////////////////
// command-line arguments

class GetOpt {
public:
  GetOpt() { }

  void AddOpt(const string &name, bool has_arg);
  void Parse(int argc, char *argv[]);
  int Lookup(const string &name) const;

  bool Exists(const string &name) const;
  string Get(const string &name, const string &default_value) const;
  string Get(const string &name) const;
  int GetInt(const string &name) const;
  int GetInt(const string &name, int default_value) const;
  double GetDouble(const string &name) const;
  double GetDouble(const string &name, double default_value) const;

private:
  vector< pair<string, bool> > opts; 
  vector<string> values;
};

template<class T> struct OptInfo {
  OptInfo(const string &name, T *var, const string &msg, bool required)
    : name(name), var(var), msg(msg), required(required) { }

  string name;
  T *var; // location of the variable that stores this value
  string msg;
  bool required;
};

extern vector< OptInfo<bool> > bool_opts;
extern vector< OptInfo<int> > int_opts;
extern vector< OptInfo<double> > double_opts;
extern vector< OptInfo<string> > string_opts;

////////////////////////////////////////////////////////////

// two versions: in one, option is required
#define opt_define_bool_req(var, name, msg) \
  bool var = opt_define_bool_wrap(name, &var, false, msg, true)
#define opt_define_bool(var, name, val, msg) \
  bool var = opt_define_bool_wrap(name, &var, val, msg, false)
#define opt_define_int_req(var, name, msg) \
  int var = opt_define_int_wrap(name, &var, 0, msg, true)
#define opt_define_int(var, name, val, msg) \
  int var = opt_define_int_wrap(name, &var, val, msg, false)
#define opt_define_double_req(var, name, msg) \
  double var = opt_define_double_wrap(name, &var, 0.0, msg, true)
#define opt_define_double(var, name, val, msg) \
  double var = opt_define_double_wrap(name, &var, val, msg, false)
#define opt_define_string_req(var, name, msg) \
  string var = opt_define_string_wrap(name, &var, "", msg, true)
#define opt_define_string(var, name, val, msg) \
  string var = opt_define_string_wrap(name, &var, val, msg, false)

inline bool opt_define_bool_wrap(const string &name, bool *var, bool val, const string &msg, bool required) {
  bool_opts.push_back(OptInfo<bool>(name, var, msg, required));
  return val; 
}

inline int opt_define_int_wrap(const string &name, int *var, int val, const string &msg, bool required) {
  //printf("HELLO %s\n", name.c_str());
  int_opts.push_back(OptInfo<int>(name, var, msg, required));
  //printf("N %d\n", (int)int_opts.size());
  return val; 
}
inline double opt_define_double_wrap(const string &name, double *var, double val, const string &msg, bool required) {
  double_opts.push_back(OptInfo<double>(name, var, msg, required));
  return val;
}
inline string opt_define_string_wrap(const string &name, string *var, const string &val, const string &msg, bool required) {
  string_opts.push_back(OptInfo<string>(name, var, msg, required));
  return val; 
}

////////////////////////////////////////////////////////////

void print_opts();

extern int rand_seed;
extern string comment;
extern int initC;

#endif
