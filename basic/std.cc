#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "std.h"
#include "str.h"
#include "timer.h"

// Return the current date/time.
string now() {
  time_t t = time(NULL);
  return substr(ctime(&t), 0, -1);
}

string hostname() {
  char buf[1024];
  gethostname(buf, sizeof(buf));
  return buf;
}

// Return the amount of memory (kB) used by this process
int mem_usage() {
  ifstream in("/proc/self/status");
  if(!in) return 0;
  char buf[1024];
  static const char *key = "VmRSS";

  while(in.getline(buf, sizeof(buf))) {
    if(strncmp(buf, key, strlen(key)) != 0) continue;
    char *s = strchr(buf, ':');
    if(!s) return 0;
    int x;
    sscanf(s+1, "%d", &x);
    return x;
  }
  return -1;
}

// Return whether the file exists.
bool file_exists(const char *file) {
  return access(file, F_OK) == 0;
}

// Create an empty file.  Return success.
bool create_file(const char *file) {
  ofstream out(file);
  if(!out) return false;
  out.close();
  return true;
}

time_t file_modified_time(const char *file) {
  struct stat stat_buf;
  if(stat(file, &stat_buf) != 0)
    return 0;
  return stat_buf.st_mtime;
}

// Return the cpu speed in MHz.
int cpu_speed_mhz() {
  ifstream in("/proc/cpuinfo");
  if(!in) return 0;
  char buf[1024];
  static const char *key = "cpu MHz";

  while(in.getline(buf, sizeof(buf))) {
    if(strncmp(buf, key, strlen(key)) != 0) continue;
    char *s = strchr(buf, ':');
    if(!s) return 0;
    double x;
    sscanf(s+1, "%lf", &x);
    return (int)x;
  }
  return 0;
}

// "file" -> "file"
// "dir/file" -> "file"
string strip_dir(string s) {
  return substr(s, s.rfind('/')+1);
}

// "file" -> "file"
// "dir/file" -> "dir"
string get_dir(string s) {
  int i = s.rfind('/');
  return i == -1 ? "." : substr(s, 0, s.rfind('/'));
}

// "base" -> "base"
// "base.ext" -> "base"
string file_base(string s) {
  int i = s.rfind('.');
  return i == -1 ? s : substr(s, 0, i);
}

bool get_files_in_dir(string dirname, bool fullpath, vector<string> &files) {
  DIR *dir = opendir(dirname.c_str());
  if(!dir) return false;
  while(true) {
    dirent *ent = readdir(dir);
    if(!ent) break;
    // For some reason, sometimes files show up as d_type == DT_UNKNOWN, I
    // think due to AFS issues
    //cout << "FFF " << ent->d_name << ' ' << (int)ent->d_type << endl;
    if(ent->d_type != DT_DIR) {
      files.push_back((fullpath ? dirname+"/" : string()) + ent->d_name);
    }
  }
  closedir(dir);
  return true;
}
