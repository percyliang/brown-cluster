#ifndef __MEM_H__
#define __MEM_H__

// Takes memory is in bytes and formats it nicely
struct Mem { Mem(int mem) : mem(mem) { } int mem; };
inline ostream &operator<<(ostream &out, const Mem &m) {
  unsigned int mem = m.mem;
  if(mem < 1024)           out << mem;
  else if(mem < 1024*1024) out << mem/1024 << 'K';
  else                     out << mem/(1024*1024) << 'M';
  return out;
}

#endif
