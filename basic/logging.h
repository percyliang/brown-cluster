#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "std.h"
#include "mem.h"
#include "timer.h"
#include "indent.h"

////////////////////////////////////////////////////////////

// State associated with a run.
struct Run {
  Run() { init(); }
  bool print() const { return num_lines == next_line_to_print; }

  int num_omitted() { return num_lines - num_lines_printed; }
  bool new_line();

  void init();
  void finish();

  int num_lines;          // Number of lines that we've gone through so far in this run.
  int num_lines_printed;  // Number of lines actually printed.
  int next_line_to_print; // Next line to be printed (lines are 0-based).
  Timer timer;            // Keeps track of time spent on this run.
  bool print_all_lines;   // Whether or not to force the printing of each line.
};

////////////////////////////////////////////////////////////
// Global information about logging.

struct LogInfo {
  LogInfo();
  ~LogInfo();

  void init();
  void hard_flush() { out.flush(); }

  Run &parent_run() { return runs[ind_level-1]; }
  Run &this_run()   { return runs[ind_level]; }
  Run &child_run()  { return runs[ind_level+1]; }

  // Parameters.
  int max_ind_level; // Maximum indent level.
  int ms_per_line;   // Number of milliseconds between consecutive lines of output.
  string log_file;

  // State.
  ofstream out;
  int ind_level; // Current indent level.
  const char *buf; // The buffer to be flushed out the next time _logs is called.
  vector<Run> runs; // Indent level -> state
  Timer timer; // Timer that starts at the beginning of the program
};

extern LogInfo log_info;

////////////////////////////////////////////////////////////

#define lout (log_info.out)
#define here lout << "HERE " << __FILE__ << ':' << __LINE__ << endl
#define _ind_within        (log_info.ind_level   <= log_info.max_ind_level)
#define _parent_ind_within (log_info.ind_level-1 <= log_info.max_ind_level)
#define _logs(x) \
  do { lout << log_info.buf << Indent(log_info.ind_level) << x; log_info.buf = ""; } while(0)
#define logs(x) \
  do { \
    if(_ind_within && log_info.this_run().new_line()) { \
      _logs(x << endl); \
    } \
  } while(0)
// Output something if parent outputted something.
// Subtle note: parent must have been a track, not logs, so its run
// information has not been updated yet until it closes.
// Therefore, calling print() on it is valid.
#define logss(x) \
  do { \
    if(_parent_ind_within && log_info.parent_run().print()) { \
      log_info.this_run().new_line(); \
      _logs(x << endl); \
    } \
  } while(0)

#define LOGS(x) _logs(x << endl)

////////////////////////////////////////////////////////////
// For tracking functions or blocks.
struct LogTracker {
  LogTracker(const char *name) : b(true), output_stopped(false), name(name) { }
  void begin(bool print_all_lines);
  ~LogTracker();

  bool b; // Trick used in track_block to execute the for loop exactly once.
  bool output_stopped;
  const char *name;
  ostringstream descrip;
};

#define track(name, x, all) \
  LogTracker _lt(name); \
  (_ind_within && log_info.this_run().print() && _lt.descrip << x), _lt.begin(all)
#define track_block(name, x, all) \
  for(LogTracker _lt(name); \
      _lt.b && ((_ind_within && log_info.this_run().print() && _lt.descrip << x), _lt.begin(all), true); \
      _lt.b = false)

#define track_foridx(i, n, s, all) \
  foridx(i, n) track_block(s, i << '/' << n, all)
#define track_forvec(i, tx, x, vec, s, all) \
  forvec(i, tx, x, vec) track_block(s, i << '/' << len(vec), all)

#define init_log \
  log_info.init(); \
  track("main", to_vector(argv, argc), true); \
  logs(now() << " on " << hostname() << " (" << cpu_speed_mhz() << "MHz)");

#define prog_status \
  "PROG_STATUS: " << \
  "time = " << log_info.timer.stop() << \
  ", memory = " << Mem(mem_usage()*1024)

#endif
