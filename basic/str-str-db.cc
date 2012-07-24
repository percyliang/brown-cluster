#include "str-str-db.h"
#include "std.h"
#include "str.h"
#include "strdb.h"

StrStrDB::~StrStrDB() {
  destroy_strings(s2t);
}

// File format: lines of <t>\t<s>\t<...junk...>
void StrStrDB::read(const char *file) {
  track("StrStrDB::read()", file, true);
  char buf[1024];
  ifstream in(file);
  assert2(in, file);

  // Read the s2t for each word.
  max_t_len = 0;
  while(in.getline(buf, sizeof(buf))) {
    char *t = strtok(buf, "\t");
    char *s = strtok(NULL, "\t");
    assert(s && t);

    assert2(!contains(s2t, s), s << " appears twice");
    s2t[copy_str(s)] = copy_str(t);
    max_t_len = max(max_t_len, (int)strlen(t));
  }
  logs("Read " << len(s2t) << " strings");
  logs("Longest mapped string is " << max_t_len << " characters.");
}

const char *StrStrDB::operator[](const char *word) const {
  StrStrMap::const_iterator it = s2t.find(word);
  return it == s2t.end() ? "" : it->second;
}
