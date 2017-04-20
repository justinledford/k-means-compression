/*
 * Utility functions for loading image data
 * into matrices, and writing matrices to pixmaps
 * and compressed binary formats.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "image.h"

/*
 *  Load a .rgb file (pixmap) into a m x n matrix
 *  where n = tile_w * tile_h * 3 (channels of each pixel)
 *  and m = w * h / (tile_w * tile_h)
 */
double **load_rgb(char *path, unsigned w, unsigned h,
        unsigned tile_w, unsigned tile_h)
{
    unsigned i, j, k, l, n, m, x, y, *offsets;
    unsigned char *buf;
    FILE* fp;
    double **X;

    if ((w % tile_w) != 0) {
        printf("load_img: w not divisible by tile width\n");
        exit(1);
    }

    if ((h % tile_h) != 0) {
        printf("load_img: h not divisible by tile height\n");
        exit(1);
    }

    /* vector size */
    n = tile_w * tile_h * 3;

    /* # of vectors */
    m = w * h / (tile_w * tile_h);

    X = malloc(sizeof(double *) * m);
    for (i = 0; i < m; i++)
        X[i] = malloc(sizeof(double) * n);

    buf = malloc(sizeof(double) * n);
    offsets = malloc(sizeof(unsigned) * tile_h * tile_w);

    fp = fopen(path, "r");

    if (fp == NULL) {
        printf("error: couldn't open file %s\n", path);
        exit(1);
    }

    // read bytes into vectors
    for (i = 0; i < m; i++) {

        // calculate offsets
        j = 0;
        x = i*tile_w;
        y = i/(w/tile_w) * w * (tile_h-1);
        for (k = 0; k < tile_h; k++) {
            for (l = 0; l < tile_w; l++) {
                offsets[j] = x + y + k*w + l;
                j++;
            }
        }

        for (j = 0; j < n/3; j++) {
            fseek(fp, offsets[j]*3, SEEK_SET);
            if (fread(buf+(j*3), 1, 3, fp) < 3) {
                printf("load_img: read less than %d bytes\n",
                        3);
                exit(1);
            }
        }

        for (j = 0; j < n; j++)
            X[i][j] = buf[j];
    }

    free(buf);
    free(offsets);
    return X;
}

/*
 *  Write a .rgb file (pixmap) from a m x n matrix
 *  where n = tile_w * tile_h * 3 (channels of each pixel)
 *  and m = w * h / (tile_w * tile_h)
 */
void write_rgb(char *path, double **X, unsigned w, unsigned h,
        unsigned tile_w, unsigned tile_h)
{
    unsigned i, j, m, n, k, l, x, y, *offsets;
    unsigned char *buf;
    FILE* fp;

    if ((w % tile_w) != 0) {
        printf("load_img: w not divisible by tile width\n");
        exit(1);
    }

    if ((h % tile_h) != 0) {
        printf("load_img: h not divisible by tile height\n");
        exit(1);
    }

    /* vector size */
    n = tile_w * tile_h * 3;

    /* # of vectors */
    m = w * h / (tile_w * tile_h);


    buf = malloc(sizeof(double) * n);
    offsets = malloc(sizeof(unsigned) * tile_h * tile_w);

    fp = fopen(path, "w");

    if (fp == NULL) {
        printf("error: couldn't open file %s\n", path);
        exit(1);
    }

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            buf[j] = (unsigned char)X[i][j];

        j = 0;
        x = i*tile_w;
        y = (i/(w/tile_w)) * w*(tile_h-1);
        for (k = 0; k < tile_h; k++) {
            for (l = 0; l < tile_w; l++) {
                offsets[j] = x + y + k*w + l;
                j++;
            }
        }

        for (j = 0; j < n/3; j++) {
            fseek(fp, offsets[j]*3, SEEK_SET);
            if (fwrite(buf+(j*3), 1, 3, fp) < 3) {
                printf("write_img: read less than %d bytes\n",
                        3);
                exit(1);
            }
        }
    }

    free(buf);
    free(offsets);
}

/*
 *  Write a .rgb file (pixmap) from a centroids
 *  matrix and a label vector
 *
 *  C - k x n matrix of centroids
 *  L - m x 1 vector containg the labels from the
 *      rows of a matrix
 */
void write_clustered_rgb(char *path, unsigned w, unsigned h,
        unsigned tile_w, unsigned tile_h,
        double **C, unsigned *L)
{
    unsigned i, j, m, n, k, l, x, y;
    unsigned char *buf;
    unsigned *offsets;
    FILE* fp;

    if ((w % tile_w) != 0) {
        printf("load_img: w not divisible by tile width\n");
        exit(1);
    }

    if ((h % tile_h) != 0) {
        printf("load_img: h not divisible by tile height\n");
        exit(1);
    }

    /* vector size */
    n = tile_w * tile_h * 3;

    /* # of vectors */
    m = w * h / (tile_w * tile_h);

    buf = malloc(sizeof(double) * n);
    offsets = malloc(sizeof(unsigned) * tile_h * tile_w);

    fp = fopen(path, "w");

    if (fp == NULL) {
        printf("error: couldn't open file %s\n", path);
        exit(1);
    }

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            buf[j] = (unsigned char)C[L[i]][j];

        j = 0;
        x = i*tile_w;
        y = (i/(w/tile_w)) * w*(tile_h-1);
        for (k = 0; k < tile_h; k++) {
            for (l = 0; l < tile_w; l++) {
                offsets[j] = x + y + k*w + l;
                j++;
            }
        }

        for (j = 0; j < n/3; j++) {
            fseek(fp, offsets[j]*3, SEEK_SET);
            if (fwrite(buf+(j*3), 1, 3, fp) < 3) {
                printf("write_img: read less than %d bytes\n",
                        3);
                exit(1);
            }
        }
    }

    free(buf);
    free(offsets);
}

/*
 * Write image to binary format:
 *
 *                            bytes offset
 *  magic number                0
 *  width (px)                  1
 *  height (px)                 3
 *  tile_width  (px)            5
 *  tile_height (px)            7
 *  # of tiles in codebook      9
 *  ID size (bits)              11
 *  tile size (bytes)           13
 *  tiles                       15
 *  identifiers                 15 + (n)*tile size
 *
 *  notes:
 *  ID size = log2 (# of tiles in codebook)
 *      -IDs are packed into 16 bit ints,
 *       so some padding may be present dependiing
 *       on the ID size
 *  tile size = tile_width * tile_height * 3
 *
 */
void write_compressed(char *path, unsigned w, unsigned h,
        unsigned tile_w, unsigned tile_h,
        double **C, unsigned *L, unsigned k)
{
    unsigned i, j, m, n;
    uint8_t buf8;
    uint16_t buf16;
    FILE* fp;

    if ((w % tile_w) != 0) {
        printf("load_img: w not divisible by tile width\n");
        exit(1);
    }

    if ((h % tile_h) != 0) {
        printf("load_img: h not divisible by tile height\n");
        exit(1);
    }

    /* vector size */
    n = tile_w * tile_h * 3;

    /* # of vectors */
    m = (w / tile_w) * (h / tile_h);

    fp = fopen(path, "w");

    if (fp == NULL) {
        printf("error: couldn't open file %s\n", path);
        exit(1);
    }

    /* magic number */
    char magic = 'J';
    fwrite(&magic, sizeof(char), 1, fp);

    /* width and height*/
    buf16 = (uint16_t)w;
    fwrite(&buf16, sizeof(uint16_t), 1, fp);
    buf16 = (uint16_t)h;
    fwrite(&buf16, sizeof(uint16_t), 1, fp);

    /* tile width and height*/
    buf16 = (uint16_t)tile_w;
    fwrite(&buf16, sizeof(uint16_t), 1, fp);
    buf16 = (uint16_t)tile_h;
    fwrite(&buf16, sizeof(uint16_t), 1, fp);

    /* # of tiles */
    buf16 = (uint16_t)k;
    fwrite(&buf16, sizeof(uint16_t), 1, fp);

    /* ID size */
    buf16 = (uint16_t)log2(k);
    fwrite(&buf16, sizeof(uint16_t), 1, fp);

    /* tile size */
    buf16 = (uint16_t)(tile_w * tile_h * 3);
    fwrite(&buf16, sizeof(uint16_t), 1, fp);

    /* write tiles */
    for (i = 0; i < k; i++) {
        for (j = 0; j < n; j++) {
            buf8 = (uint8_t)C[i][j];
            fwrite(&buf8, sizeof(uint8_t), 1, fp);
        }
    }

    /* write IDs */
    i = 0;
    while(i < m) {
        /* squueze bits together */
        for (j = 0, buf16 = 0; j < (16-log2(k)); j += log2(k)) {
            buf16 |= (L[i] << j);
            i++;
        }

        if (fwrite(&buf16, sizeof(uint16_t), 1, fp) < 1) {
            printf("write_compressed: error writing IDs\n");
            printf("%d\n", i);
            exit(1);
        }
    }

    fclose(fp);
}

/*
 *  Load image into matrices, vectors and properties
 *  from binary format:
 *
 *                            bytes offset
 *  magic number                0
 *  width (px)                  1
 *  height (px)                 3
 *  tile_width  (px)            5
 *  tile_height (px)            7
 *  # of tiles in codebook      9
 *  ID size (bits)              11
 *  tile size (bytes)           13
 *  tiles                       15
 *  identifiers                 15 + (n)*tile size
 *
 *  notes:
 *  ID size = log2 (# of tiles in codebook)
 *      -IDs are packed into 16 bit ints,
 *       so some padding may be present dependiing
 *       on the ID size
 *  tile size = tile_width * tile_height * 3
 *
 */
void load_compressed(char *path, unsigned *w, unsigned *h,
        unsigned *tile_w, unsigned *tile_h,
        double ***C, unsigned **L, unsigned *k)
{
    unsigned i, j, m, n, id_size;
    uint8_t buf8;
    uint16_t buf16;
    int mask;
    FILE* fp;

    fp = fopen(path, "r");

    if (fp == NULL) {
        printf("error: couldn't open file %s\n", path);
        exit(1);
    }

    /* magic number */
    fread(&buf8, sizeof(char), 1, fp);

    /* width and height*/
    fread(&buf16, sizeof(uint16_t), 1, fp);
    *w = buf16;
    fread(&buf16, sizeof(uint16_t), 1, fp);
    *h = buf16;

    /* tile width and height*/
    fread(&buf16, sizeof(uint16_t), 1, fp);
    *tile_w = buf16;
    fread(&buf16, sizeof(uint16_t), 1, fp);
    *tile_h = buf16;


    /* # of tiles */
    fread(&buf16, sizeof(uint16_t), 1, fp);
    *k = buf16;

    /* ID size */
    fread(&buf16, sizeof(uint16_t), 1, fp);
    id_size = buf16;

    /* tile size */
    fread(&buf16, sizeof(uint16_t), 1, fp);
    n = buf16;

    /* # of vectors */
    m = (*w / *tile_w) * (*h / *tile_h);

    *C = malloc(sizeof(double *) * *k);
    for (i = 0; i < *k; i++)
        (*C)[i] = malloc(sizeof(double) * n);

    *L = malloc(sizeof(unsigned) * m);

    /* read tiles */
    for (i = 0; i < *k; i++) {
        for (j = 0; j < n; j++) {
            fread(&buf8, sizeof(uint8_t), 1, fp);
            (*C)[i][j] = buf8;
        }
    }

    /* read IDs */
    i = 0;
    while(i < m) {
        /* squeeze bits together */
        if (fread(&buf16, sizeof(uint16_t), 1, fp) < 1) {
            printf("load_compressed: error reading IDs\n");

            if (feof(fp)) {
                printf("load_compressed: reached EOF\n");
            }
            exit(1);
        }

        for (j = 0; j < 16-log2(*k); j += log2(*k)) {
            mask = (*k-1) << (j);
            (*L)[i] = (buf16 & mask) >> j;
            i++;
        }
    }

    fclose(fp);
}


/*
 * Save pixels into a text file
 * Pixels are separated by newlines, channels by spaces
 *
 * Currently only supports 1x1 tiles
 */
void save_txt_pixels(char *path, double **X, unsigned n)
{
    unsigned i, j;
    FILE* fp;

    fp = fopen(path, "w");

    if (fp == NULL) {
        printf("error: couldn't open file %s\n", path);
        exit(1);
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < 3; j++)
            fprintf(fp, "%u ", (unsigned)X[i][j]);
        fprintf(fp, "\n");
    }
}
