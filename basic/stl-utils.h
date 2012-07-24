#ifndef __STL_UTILS__
#define __STL_UTILS__

#include "stl-basic.h"
#include <stdarg.h>

#define contains(X, x) ((X).find(x) != (X).end())

inline void improve(DoubleInt &x, const DoubleInt &y) {
  if(y.first > x.first) x = y; // Bigger is better.
}

template<class Compare> inline void improve(DoubleInt &x, const DoubleInt &y, Compare compare) {
  if(compare(y.first, x.first)) x = y;
}

// Free up the memory in a vector or hash_map.
template<class T> void destroy(T &obj) {
  T empty_obj;
  obj.swap(empty_obj);
}

template<class T> int index_of(const vector<T> &vec, const T &x, int i0 = 0) {
  for(int i = i0; i < len(vec); i++)
    if(vec[i] == x) return i;
  return -1;
}

template<class T> int count_of(const vector<T> &vec, const T &x) {
  int n = 0;
  forvec(_, const T &, y, vec)
    if(x == y) n++;
  return n;
}

// Get vec[i], but if i is out of range, expand the vector and fill
// everything with x.
template<class T> T &expand_get(vector<T> &vec, int i, const T &x) {
  int n = len(vec);
  if(i >= n) {
    vec.resize(i+1);
    for(int ii = n; ii <= i; ii++) vec[ii] = x;
  }
  return vec[i];
}
template<class T> T &expand_get(vector< vector<T> > &mat, int i, int j, const T &x) {
  int n = len(mat);
  if(i >= n) mat.resize(i+1);
  return expand_get(mat[i], j, x);
}
template<class T> T &expand_get(vector< vector< vector<T> > > &mat, int i, int j, int k, const T &x) {
  int n = len(mat);
  if(i >= n) mat.resize(i+1);
  return expand_get(mat[i], j, k, x);
}

// Assuming this vector/matrix will not grow any more,
// we can safely call compact to reduce the memory usage.
// This is only effective after deletions.
// This isn't necessary if we haven't actually touched
// the memory past size (i.e., we didn't have a bigger
// structure).
template<class T> void vector_compact(vector<T> &vec) {
  vector<T> new_vec(len(vec));
  new_vec = vec;
  vec.swap(new_vec);
}
template<class T> void matrix_compact(vector< vector<T> > &mat) {
  vector< vector<T> > new_mat(len(mat));
  foridx(i, len(mat)) compact(mat[i]);
  new_mat = mat;
  mat.swap(new_mat);
}

// Append to a vector and return the value type.
template<class T> inline T &push_back(vector<T> &vec, const T &x = T()) {
  vec.push_back(x);
  return vec[len(vec)-1];
}

template<class T> inline void matrix_resize(vector< vector<T> > &mat, int nr, int nc) {
  mat.resize(nr);
  foridx(r, nr) mat[r].resize(nc);
}

template<class T> inline void matrix_resize(vector< vector< vector<T> > > &mat, int n1, int n2, int n3) {
  mat.resize(n1);
  foridx(i, n1) {
    mat[i].resize(n2);
    foridx(j, n2)
      mat[i][j].resize(n3);
  }
}

template<class T> inline vector< vector<T> > new_matrix(int nr, int nc, T v) {
  vector< vector<T> > mat;
  mat.resize(nr);
  foridx(r, nr) {
    mat[r].resize(nc);
    foridx(c, nc)
      mat[r][c] = v;
  }
  return mat;
}

template<class T> inline void matrix_fill(vector< vector<T> > &mat, T v) {
  foridx(i, len(mat)) vector_fill(mat[i], v);
}

template<class T> inline void vector_fill(vector<T> &vec, T v) {
  foridx(i, len(vec)) vec[i] = v;
}

template<class T> inline T vector_sum(const vector<T> &vec) {
  T sum = 0;
  foridx(i, len(vec)) sum += vec[i];
  return sum;
}

// Returns the index of the minimum element in vec.
template<class T> inline int vector_index_min(const vector<T> &vec) {
  T min = vec[0];
  int best_i = 0;
  foridx(i, len(vec)) {
    if(vec[i] < min) {
      min = vec[i];
      best_i = i;
    }
  }
  return best_i;
}

template<class T> inline int vector_min(const vector<T> &vec) {
  return vec[vector_index_min(vec)];
}

// Returns the index of the maximum element in vec.
template<class T> inline int vector_index_max(const vector<T> &vec) {
  T max = vec[0];
  int best_i = 0;
  foridx(i, len(vec)) {
    if(vec[i] > max) {
      max = vec[i];
      best_i = i;
    }
  }
  return best_i;
}

template<class T> inline int vector_max(const vector<T> &vec) {
  return vec[vector_index_max(vec)];
}

// Returns the index of the maximum element in vec.
template<class T> inline IntPair matrix_index_max(const vector< vector<T> > &mat) {
  T max = mat[0][0];
  IntPair best_ij = IntPair(0, 0);
  foridx(i, len(mat)) {
    foridx(j, len(mat[i])) {
      if(mat[i][j] > max) {
        max = mat[i][j];
        best_ij = IntPair(i, j);
      }
    }
  }
  return best_ij;
}

// Returns the sum of the elements in column c.
template<class T> inline T matrix_col_sum(const vector< vector<T> > &mat, int c) {
  T sum = 0;
  foridx(r, len(mat)) sum += mat[r][c];
  return sum;
}

template<class T1, class T2> ostream &operator<<(ostream &out, const pair<T1, T2> &p) {
  return out << p.first << ' ' << p.second;
}

template<class T> ostream &operator<<(ostream &out, const vector<T> &vec) {
  foridx(i, len(vec)) {
    if(i > 0) out << ' ';
    out << vec[i];
  }
  return out;
}

template<class T> ostream &operator<<(ostream &out, const vector< vector<T> > &mat) {
  foridx(r, len(mat)) out << mat[r] << endl;
  return out;
}

template<class T> vector<T> subvector(const vector<T> &vec, int i, int j = -1) {
  int N = len(vec);
  if(j < 0) j += N;
  if(j < i) j = i;

  // Probably some fancy STL way to do this.
  vector<T> subvec(j-i);
  foridx(k, j-i) subvec[k] = vec[i+k];
  return subvec;
}

template<class T> vector<T> to_vector(T arr[], int n) {
  vector<T> vec(n);
  foridx(i, n) vec[i] = arr[i];
  return vec;
}

inline IntVec to_vector(int n, ...) {
  va_list ap;
  IntVec vec;
  va_start(ap, n);
  foridx(i, n) vec.push_back(va_arg(ap, int));
  va_end(ap);
  return vec;
}

inline DoubleVec to_fvector(int n, ...) {
  va_list ap;
  DoubleVec vec;
  va_start(ap, n);
  foridx(i, n) vec.push_back(va_arg(ap, double));
  va_end(ap);
  return vec;
}

template<class T> inline void operator+=(vector<T> &vec1, const vector<T> &vec2) {
  foridx(i, len(vec1)) vec1[i] += vec2[i];
}

#endif
