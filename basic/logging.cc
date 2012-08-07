#include "logging.h"
#include "opt.h"
#include "mem.h"

// The logging output has a tree structure, where each node is a
// line of output, and the depth of a node is its indent level.
// A run is the sequence of children of some node.
// A subset of the lines in the run will get printed.

////////////////////////////////////////////////////////////

void Run::init() {
  num_lines = 0;
  num_lines_printed = 0;
  next_line_to_print = 0;
  print_all_lines = false;
  timer.start();
}

void Run::finish() {
  // Make it clear that this run is not printed.
  // Otherwise, logss might think its
  // parent was printed when it really wasn't.
  next_line_to_print = -1;
  timer.stop();
}

bool Run::new_line() {
  bool p = print();
  num_lines++;
  if(!p) return false;
  
  // We're going to print this line.  Now decide next line to print.
  int ms_per_line = log_info.ms_per_line;
  if(num_lines <= 2 ||   // Print first few lines anyway.
     ms_per_line == 0 || // Print everything.
     print_all_lines)    // Print every line in this run.
    next_line_to_print++;
  else {
    timer.stop();
    if(timer.ms == 0) // No time has elapsed.  
      next_line_to_print *= 2; // Exponentially increase time between lines.
    else
      next_line_to_print += max(int((double)num_lines * ms_per_line / timer.ms), 1);
  }

  num_lines_printed++;
  return true;
}

////////////////////////////////////////////////////////////
// Global information about logging.

LogInfo::LogInfo() {
  ms_per_line   = 0; //1000; // 1 second
  max_ind_level = 3;

  ind_level = 0;
  buf       = "";

  runs.resize(128);
  timer.start();
}

LogInfo::~LogInfo() {
  out.flush();
}

void LogInfo::init() {
  if (log_file.empty()) {
    out.open("/dev/stdout");
  } else {
    cout << "Logging to " << log_file << endl;
    out.open(log_file.c_str());
  }
}

LogInfo log_info;

////////////////////////////////////////////////////////////
// LogTracker:: For tracking functions or blocks.

void LogTracker::begin(bool print_all_lines) {
  if(_ind_within) {
    if(log_info.this_run().print()) {
      const string &s = descrip.str();

      _logs(name);
      if(s.size() > 0 && name[0])
        lout << ": ";
      lout << s;

      lout.flush();
      log_info.buf = " {\n"; // Open the block.

      log_info.child_run().init();
      log_info.child_run().print_all_lines = print_all_lines;
    }
    else {
      log_info.max_ind_level = -log_info.max_ind_level; // Prevent children from outputting.
      output_stopped = true;
    }
  }

  log_info.ind_level++;
}

LogTracker::~LogTracker() {
  log_info.ind_level--;

  if(output_stopped)
    log_info.max_ind_level = -log_info.max_ind_level; // Restore indent level.

  if(_ind_within) {
    if(log_info.this_run().new_line()) {
      // Finish up child level.
      log_info.ind_level++;
      int n = log_info.this_run().num_omitted();
      if(n > 0)
        _logs("... " << n << " lines omitted ...\n");
      log_info.ind_level--;
      log_info.child_run().finish();

      if(log_info.buf[0]) // Nothing was printed, because buf hasn't been emptied.
        log_info.buf = ""; // Just pretend we didn't open the block.
      else // Something indented was printed.
        _logs("}"); // Close the block.

      // Print time
      Timer &ct = log_info.child_run().timer;
      lout << " [" << ct;
      if(log_info.ind_level > 0) {
        Timer &tt = log_info.this_run().timer;
        tt.stop();
        lout << ", cumulative " << tt;
      }
      lout << "]\n";
    }
  }
}

// Options for logging.
int _log_info_max_ind_level = opt_define_int_wrap("max-ind-level", &log_info.max_ind_level, log_info.max_ind_level, "Maximum indent level for logging", false);
int _log_info_ms_per_line = opt_define_int_wrap("ms-per-line", &log_info.ms_per_line, log_info.ms_per_line, "Print a line out every this many milliseconds", false);
string _log_info_log_file = opt_define_string_wrap("log", &log_info.log_file, log_info.log_file, "File to write log to (\"\" for stdout)", false);
