/*
 * Implementation of Lloyd's algorithm
 * for k-means clustering.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <stdbool.h>

#include "lloyds.h"

/*
 * Squared euclidean distance.
 */
double distance(double **x, double **y, size_t n)
{
    double dist;
    unsigned i;

    dist = 0;
    for (i = 0; i < n; i++)
        dist += pow((*x)[i] - (*y)[i], 2);

    return dist;
}


/*
 * Initialize centroids in C
 * by choosing k random rows from X
 */
void init_centroids(double **C, unsigned k,
        double **X, size_t m, size_t n)
{
    unsigned i, j, r;

    for (i = 0, r = rand() % m; i < k; i++, r = rand() % m)
        for (j = 0; j < n; j++)
            C[i][j] = X[r][j];
}


/*
 * Initialize labels by finding centroid
 * in C with least distance for each X
 *
 * The label is an index into C
 */
void init_labels(double **C, unsigned k,
        double **X, size_t m, size_t n, unsigned *L, double *min_dist)
{
    unsigned i, j;
    double d;

    for (i = 0; i < m; i++) {
        for (j = 0; j < k; j++) {
            d = distance(&X[i], &C[j], n);
            if (d < min_dist[i]) {
                min_dist[i] = d;
                L[i] = j;
            }
        }
    }
}


/*
 * Update centroids by getting means of
 * all x with common centroids, and assigning
 * the new centroid to those means
 */
void update_centroids(double **C, unsigned k,
        double **X, size_t m, size_t n, unsigned *L)
{
    unsigned i, j, h;
    double *mean, l;

    for (j = 0; j < k; j++) {
        l = 0;
        mean = calloc(n, sizeof(double));

        for (i = 0; i < m; i++) {
            if (L[i] == j) {
                for (h = 0; h < n; h++) {
                    mean[h] += X[i][h];
                }
                l++;
            }
        }

        if (l) {
            for (i = 0; i < n; i++)
                mean[i] /= l;
        }

        for (i = 0; i < n; i++)
            C[j][i] = mean[i];
        free(mean);
    }
}


/*
 * Update labels for each x by finding the closest centroid.
 *
 * Returns true if any of the labels have changed.
 */
bool update_labels(double **C, unsigned k,
        double **X, size_t m, size_t n, unsigned *L, double *min_dist)
{
    unsigned i, j;
    double _min_dist, d;
    bool changed;

    for (i = 0; i < m; i++) {
        _min_dist = min_dist[i];
        min_dist[i] = DBL_MAX;
        for (j = 0; j < k; j++) {
            d = distance(&X[i], &C[j], n);
            if (d < min_dist[i]) {
                min_dist[i] = d;
                L[i] = j;

                if (min_dist[i] < _min_dist)
                    changed = true;
            }
        }
    }

    return changed;
}


/*
 * Calculate total distance between each x and its centroid.
 */
double calc_inertia(double **C,
        double **X, size_t m, size_t n, unsigned *L)
{
    unsigned i;
    double inertia;

    inertia = 0;
    for (i = 0; i < m; i++)
        inertia += distance(&C[L[i]], &X[i], n);

    return inertia;
}


/*
 * Lloyd's algorithm
 *
 * params:
 *
 * X        - input vectors
 * m        - # of rows in X
 * n        - # of cols in X
 * k        - number of clusters
 * n_iter   - number of centroid updates
 * C        - centroids container (return)
 * L        - map vector in X to centroid (return)
 *
 * return value:
 *
 * inertia  - rank centroids
 *
 * note:
 * C and L will be allocated in this function
 *
 */
double lloyds(double **X, size_t m, size_t n, unsigned k, unsigned n_iter,
       double ***C, unsigned** L)
{
    unsigned i, iter;
    double inertia, *min_dist;
    bool changed;

    *C = malloc(sizeof(double *) * k);
    for (i = 0; i < k; i++)
        (*C)[i] = malloc(sizeof(double) * n);

    *L = malloc(sizeof(unsigned) * m);

    min_dist = malloc(sizeof(double) * m);
    for (i = 0; i < m; i++)
        min_dist[i] = DBL_MAX;

    init_centroids(*C, k, X, m, n);
    init_labels(*C, k, X, m, n, *L, min_dist);

    iter = 0;
    changed = true;
    while (changed && iter < n_iter) {
        changed = false;
        update_centroids(*C, k, X, m, n, *L);
        changed = update_labels(*C, k, X, m, n, *L, min_dist);
        iter++;
    }

    inertia = calc_inertia(*C, X, m, n, *L);

    return inertia;
}
