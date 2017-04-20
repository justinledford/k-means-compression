#ifndef IMAGE_H
#define IMAGE_H

double **load_rgb(char *path, unsigned w, unsigned h,
        unsigned tile_w, unsigned tile_h);

void write_rgb(char *path, double **X, unsigned w, unsigned h,
        unsigned tile_w, unsigned tile_h);

void write_clustered_rgb(char *path, unsigned w, unsigned h,
        unsigned tile_w, unsigned tile_h,
        double **C, unsigned *L);

void write_compressed(char *path, unsigned w, unsigned h,
        unsigned tile_w, unsigned tile_h,
        double **C, unsigned *L, unsigned k);

void load_compressed(char *path, unsigned *w, unsigned *h,
        unsigned *tile_w, unsigned *tile_h,
        double ***C, unsigned **L, unsigned *k);

void write_clustered_img_pixel(char *path, unsigned n,
        double **C, unsigned *L);

void save_txt_pixels(char *path, double **X, unsigned n);

#endif
