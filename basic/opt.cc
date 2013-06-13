#include "opt.h"
#include "std.h"
#include "logging.h"
#include <getopt.h>

////////////////////////////////////////////////////////////////////////
// command-line arguments

void GetOpt::AddOpt(const string &name, bool has_arg) {
  opts.push_back(pair<string, bool>(name, has_arg));
}

void GetOpt::Parse(int argc, char *argv[]) {
  option *opt_list = new option[opts.size()+1];
  for(int i = 0; i <= (int)opts.size(); i++) {
    option *o = &opt_list[i];
    if(i < (int)opts.size()) {
      o->name = opts[i].first.c_str();
      o->has_arg = opts[i].second;
      //printf("N %s\n", o->name);
    }
    else {
      o->name = NULL;
      o->has_arg = 0;
    }
    o->flag = NULL;
    o->val = 0;
  }

  int i;

  values.clear();
  values.resize(opts.size());
  while(true) {
    int status = getopt_long(argc, argv, "", opt_list, &i);
    if(status == -1) break;
    assert(status == 0);
    //debug("%d %s -> %s\n", i, opt_list[i].name, optarg);
    // put a 1 to signify that the argument exists
    values[i] = optarg ? optarg : "1";
  }
  
  delete [] opt_list;
}

int GetOpt::Lookup(const string &name) const {
  for(int i = 0; i < (int)opts.size(); i++) {
    if(opts[i].first == name) return i;
  }
  return -1;
}

string GetOpt::Get(const string &name, const string &default_value) const {
  int i = Lookup(name);
  return i != -1 && !values[i].empty() ? values[i] : default_value;
}

string GetOpt::Get(const string &name) const {
  string x = Get(name, "");
  if(x.empty()) {
    fprintf(stderr, "Missing required parameter `%s'.\n", name.c_str());
    exit(1);
  }
  return x;
}

bool GetOpt::Exists(const string &name) const {
  return !Get(name, "").empty();
}

int GetOpt::GetInt(const string &name) const {
  int x;
  int r = sscanf(Get(name).c_str(), "%d", &x);
  assert(r == 1);
  return x;
}

int GetOpt::GetInt(const string &name, int default_value) const {
  return Exists(name) ? GetInt(name) : default_value;
}

double GetOpt::GetDouble(const string &name) const {
  double x;
  int r = sscanf(Get(name).c_str(), "%lf", &x);
  assert(r == 1);
  return x;
}

double GetOpt::GetDouble(const string &name, double default_value) const {
  return Exists(name) ? GetDouble(name) : default_value;
}

////////////////////////////////////////////////////////////

void process_opt(int argc, char *argv[]) {
  GetOpt opt;

  // set up GetOpt to parse
  for(int i = 0; i < (int)bool_opts.size(); i++) {
    opt.AddOpt(bool_opts[i].name, false);
    opt.AddOpt("no" + bool_opts[i].name, false);
  }
  for(int i = 0; i < (int)int_opts.size(); i++)
    opt.AddOpt(int_opts[i].name, true);
  for(int i = 0; i < (int)double_opts.size(); i++)
    opt.AddOpt(double_opts[i].name, true);
  for(int i = 0; i < (int)string_opts.size(); i++)
    opt.AddOpt(string_opts[i].name, true);
  opt.AddOpt("help", false);

  // parse
  opt.Parse(argc, argv);

  // print help if called for
  if(opt.Exists("help") || !opt.Exists("text")) {
    printf("usage: %s\n", argv[0]);
    for(int i = 0; i < (int)bool_opts.size(); i++) {
      const OptInfo<bool> &o = bool_opts[i];
      printf(" %c%-20s: %s", " *"[o.required], o.name.c_str(), o.msg.c_str());
      if(!o.required) printf(" [%s]", *(o.var) ? "true" : "false");
      printf("\n");
    }
    for(int i = 0; i < (int)int_opts.size(); i++) {
      const OptInfo<int> &o = int_opts[i];
      printf(" %c%-13s <int> : %s", " *"[o.required], o.name.c_str(), o.msg.c_str());
      if(!o.required) printf(" [%d]", *(o.var));
      printf("\n");
    }
    for(int i = 0; i < (int)double_opts.size(); i++) {
      const OptInfo<double> &o = double_opts[i];
      printf(" %c%-13s <dbl> : %s", " *"[o.required], o.name.c_str(), o.msg.c_str());
      if(!o.required) printf(" [%f]", *(o.var));
      printf("\n");
    }
    for(int i = 0; i < (int)string_opts.size(); i++) {
      const OptInfo<string> &o = string_opts[i];
      printf(" %c%-13s <str> : %s", " *"[o.required], o.name.c_str(), o.msg.c_str());
      if(!o.required) printf(" [%s]", (o.var)->c_str());
      printf("\n");
    }
    exit(1);
  }

  // retrieve data; store the variables
  for(int i = 0; i < (int)bool_opts.size(); i++) {
    const OptInfo<bool> &o = bool_opts[i];
    bool yes = opt.Exists(o.name);
    bool no = opt.Exists("no" + o.name);
    assert(!o.required || (yes || no));
    assert(!yes || !no);
    if(yes) *(o.var) = true;
    if(no) *(o.var) = false;
  }
  for(int i = 0; i < (int)int_opts.size(); i++) {
    const OptInfo<int> &o = int_opts[i];
    *(o.var) = o.required ? opt.GetInt(o.name) : opt.GetInt(o.name, *(o.var));
  }
  for(int i = 0; i < (int)double_opts.size(); i++) {
    const OptInfo<double> &o = double_opts[i];
    *(o.var) = o.required ? opt.GetDouble(o.name) : opt.GetDouble(o.name, *(o.var));
  }
  for(int i = 0; i < (int)string_opts.size(); i++) {
    const OptInfo<string> &o = string_opts[i];
    *(o.var) = o.required ? opt.Get(o.name) : opt.Get(o.name, *(o.var));
  }
}

void init_opt(int argc, char *argv[]) {
  process_opt(argc, argv);
  srand(rand_seed);
}

void print_opts() {
  track("print_opts()", "", true);
  forvec(_, const OptInfo<bool> &, o, bool_opts)
    logs(o.name << " = " << (*o.var ? "true" : "false"));
  forvec(_, const OptInfo<int> &, o, int_opts)
    logs(o.name << " = " << *o.var);
  forvec(_, const OptInfo<double> &, o, double_opts)
    logs(o.name << " = " << *o.var);
  forvec(_, const OptInfo<string> &, o, string_opts)
    logs(o.name << " = " << *o.var);
}

////////////////////////////////////////////////////////////
// Pre defined options.

// allow user to specify a comment always, so some arbitrary description
// of this program execution can be embedded in the command-line
