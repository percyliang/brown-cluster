#ifndef __PROB_UTILS__
#define __PROB_UTILS__

#include "stl-basic.h"

int rand_binomial(int n, double p);
int rand_index(const fvector &probs);
double rand_gaussian(double mean, double var);

inline double factorial(int n);
inline double choose(int n, int k);
double binomial_prob(int n, int k, double p);

void norm_distrib(fvector &vec);
void norm_distrib(fmatrix &mat, int c);
void rand_distrib(fvector &probs, int n);
IntVec rand_permutation(int n);

#endif
