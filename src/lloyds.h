#ifndef LLOYDS_H
#define LLOYDS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

double distance(double **x, double **y, size_t n);
void init_centroids(double ** C, unsigned k,
        double ** X, size_t m, size_t n);
void init_labels(double **C, unsigned k,
        double **X, size_t m, size_t n, unsigned *L, double *min_dist);
void update_centroids(double **C, unsigned k,
        double **X, size_t m, size_t n, unsigned *L);
bool update_labels(double **C, unsigned k,
        double **X, size_t m, size_t n, unsigned *L, double *min_dist);
double calc_inertia(double **C,
        double **X, size_t m, size_t n, unsigned *L);
double lloyds(double ** X, size_t m, size_t n, unsigned k, unsigned n_iter,
       double *** C, unsigned** L);

#endif
