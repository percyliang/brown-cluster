#include "mem-tracker.h"
#include "mem.h"

/*
 * Currently, memory tracking is not accurate.
 * Alway underestimates.
 */

////////////////////////////////////////////////////////////

int MemTracker::compute_mem_usage(const MemRecord &r) {
  switch(r.type) {
    list_types(define_case);
    default: assert(0);
  }
  return 0;
}

int MemTracker::compute_mem_usage() {
  int total_mem = 0;
  forvec(_, MemRecord &, r, records) {
    if(r.type != T_RAWNUMBER) r.mem = compute_mem_usage(r);
    total_mem += r.mem;
  }
  return total_mem;
}

static bool record_less_than(const MemRecord &r1, const MemRecord &r2) {
  return r1.mem > r2.mem;
}

void MemTracker::report_mem_usage() {
  track("report_mem_usage()", "", true);

  int total_mem = compute_mem_usage();

  sort(records.begin(), records.end(), record_less_than);

  forvec(_, const MemRecord &, r, records) {
    logs(type_names[r.type] << ' ' << r.name << ": " <<
         Mem(r.mem) << " (" << (double)r.mem/total_mem << ')');
  }
  logs("Total: " << Mem(total_mem));
}

////////////////////////////////////////////////////////////

MemTracker mem_tracker;

const char *MemTracker::type_names[] = {
  "?",
  list_types(define_str)
};
