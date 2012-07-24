#include "multi-ostream.h"

/*
 * Create a multi_ostream, and you can add many files or any ostream objects
 * to it.  The output sent to the multi_ostream will be redirected to the many
 * destinations.
 * Useful for logging to a file and stdout.
 */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

multi_buf::~multi_buf() {
  flush();
  for(int i = 0; i < (int)infos.size(); i++)
    infos[i].destroy();
}

void multi_buf::add(ostream *out, bool own, bool hard) {
  infos.push_back(ostream_info(out, own, hard));
}

void multi_buf::flush() {
  for(int i = 0; i < (int)infos.size(); i++) {
    ostream_info &info = infos[i];
    info.out->write(buf, buf_i); 
    info.out->flush();
  }
  buf_i = 0;
}

void multi_buf::hard_flush() {
  for(int i = 0; i < (int)infos.size(); i++) {
    ostream_info &info = infos[i];
    info.out->write(buf, buf_i); 
    if(info.hard)
      ((hard_ofstream *)info.out)->hard_flush();
    else
      info.out->flush();
  }
  buf_i = 0;
}

int multi_buf::overflow(int ch) {
  buf[buf_i++] = ch;
  if(buf_i == sizeof(buf) || ch == '\n') flush();
  return ch;
}

ostream &multi_ostream::flush() {
  sbuf.flush();
  return *this;
}

ostream &multi_ostream::hard_flush() {
  sbuf.hard_flush();
  return *this;
}
