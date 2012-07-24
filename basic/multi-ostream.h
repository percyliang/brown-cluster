#ifndef __MULTI_OSTREAM_H__
#define __MULTI_OSTREAM_H__

/*
 * Create a multi_ostream, and you can add many files or any ostream objects
 * to it.  The output sent to the multi_ostream will be redirected to the many
 * destinations.
 * Useful for logging to a file and stdout.
 */

#include <iostream>
#include <fstream>
#include <vector>

#include "hard-ofstream.h"

using namespace std;

struct ostream_info {
  ostream_info(ostream *out, bool own, bool hard) : out(out), own(own), hard(hard) { }
  ostream *out;
  bool own; // Whether we own the ostream and should destroy it at the end.
  bool hard; // Whether this is a hard_ofstream.

  void destroy() { if(own) delete out; }
};

class multi_buf : public streambuf {
public:
  multi_buf() : buf_i(0) { }
  ~multi_buf();

  void flush();
  void hard_flush();

  void add(ostream *out, bool own, bool hard);
  void remove_last() { flush(); infos.back().destroy(); infos.pop_back(); }

protected:
  virtual int overflow(int ch);

private:
  vector<ostream_info> infos;
  char buf[16384];
  int buf_i;
};

class multi_ostream : public basic_ostream<char, char_traits<char> > {
public:
  multi_ostream() : basic_ostream<char, char_traits<char> >(&sbuf) { }

  virtual ostream &flush();
  virtual ostream &hard_flush();

  void add(const char *file, bool hard = false) {
    ostream *out = hard ? new hard_ofstream(file) : new ofstream(file);
    sbuf.add(out, true, hard);
  }
  void add(ostream *out) { sbuf.add(out, false, false); }

  void remove_last() { sbuf.remove_last(); }

private:
  multi_buf sbuf;
};

#endif
