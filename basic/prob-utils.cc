#include "prob-utils.h"

double rand_gaussian(double mean, double var) {
  // Use the Box-Muller Transformation
  // if x_1 and x_2 are independent uniform [0, 1],
  // then sqrt(-2 ln x_1) * cos(2*pi*x_2) is Gaussian with mean 0 and variance 1
  double x1 = rand_double(), x2 = rand_double();
  double z = sqrt(-2*log(x1))*cos(2*M_PI*x2);
  return z * sqrt(var) + mean; 
}

// The probability of heads is p.
// Throw n coin tosses.
// Return number of heads.
int rand_binomial(int n, double p) {
  int k = 0;
  while(n--) k += rand_double() < p;
  return k;
}

inline double factorial(int n) {
  double ans = 1;
  while(n > 1) ans *= n--;
  return ans;
}

inline double choose(int n, int k) {
  if(n-k < k) k = n-k;
  double ans = 1;
  for(int i = 0; i < k; i++) ans *= n-i;
  ans /= factorial(k);
  return ans;
}

double binomial_prob(int n, int k, double p) {
  return choose(n, k) * pow(p, k) * pow(1-p, n-k);
}

int rand_index(const fvector &probs) {
  double v = rand_double();
  double sum = 0;
  foridx(i, len(probs)) {
    sum += probs[i];
    if(v < sum) return i;
  }
  assert(0);
}

void norm_distrib(fvector &vec) {
  double sum = 0;
  foridx(i, len(vec)) sum += vec[i];
  foridx(i, len(vec)) vec[i] /= sum;
}

void norm_distrib(fmatrix &mat, int c) {
  double sum = 0;
  foridx(r, len(mat)) sum += mat[r][c];
  foridx(r, len(mat)) mat[r][c] /= sum;
}

void rand_distrib(fvector &probs, int n) {
  probs.resize(n);
  foridx(i, n) probs[i] = rand();
  norm_distrib(probs);
}

IntVec rand_permutation(int n) {
  IntVec perm(n);
  foridx(i, n) perm[i] = i;
  foridx(i, n) {
    int j = mrand(i, n);
    int t = perm[i]; perm[i] = perm[j]; perm[j] = t;
  }
  return perm;
}
