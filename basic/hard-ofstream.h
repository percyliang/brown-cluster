#ifndef __HARD_OFSTREAM_H__
#define __HARD_OFSTREAM_H__

// On AFS, flushing a file writes it to the local disk but not AFS.
// Hard flushing ensures that the file will be written, by closing
// and re-opening the file.

#include <fstream>
#include <string>

using namespace std;

class hard_ofstream : public ofstream {
public:
  hard_ofstream() { }
  hard_ofstream(const char *file, ofstream::openmode mode = ofstream::trunc) { open(file, mode); }
  
  void open(const char *file, ofstream::openmode mode = ofstream::trunc) {
    ofstream::open(file, mode);
    this->file = file;
  }

  void hard_flush() {
    close();
    open(file.c_str(), ofstream::app);
  }

private:
  string file;
};

#endif
