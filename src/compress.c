/*
 * Compress and decompress pixmap (.RGB) files
 * using k-means clusterning
 */

#include <stdio.h>
#include <string.h>

#include "image.h"
#include "lloyds.h"

void compress(unsigned k, unsigned w, unsigned h, unsigned tile_w,
        unsigned tile_h, char *input_path, char *output_path, bool savetxt)
{
    double **X, **C;
    unsigned *L, m, n;
    n = tile_w * tile_h * 3;
    m = (w / tile_w) * (h / tile_h);

    X = load_rgb(input_path, w, h, tile_w, tile_h);
    lloyds(X, m, n, k, 10, &C, &L);
    write_compressed(output_path, w, h, tile_w, tile_h, C, L, k);

    if (savetxt && (tile_h * tile_w == 1)) {
        save_txt_pixels("pixels.txt", X, w*h);
        save_txt_pixels("centroids.txt", C, k);
    }
}

void decompress(char *input_path, char *output_path)
{
    double **C;
    unsigned *L, k, w, h, tile_w, tile_h;

    load_compressed(input_path, &w, &h, &tile_w, &tile_h, &C, &L, &k);
    write_clustered_rgb(output_path, w, h, tile_w, tile_h, C, L);
}

void usage(char *exe)
{
        printf("Usage:\n"
               "compress: %s -c <k> <w> <h> <tile_w> <tile_h > "
               "<input> <output>\n"
               "decompress: %s -d <input> <output>\n", exe, exe);
        exit(1);
}

int main(int argc, char **argv)
{
    unsigned k, w, h, tile_w, tile_h;
    char *input_path, *output_path;
    bool savetxt;

    if ((argc < 2) ||
        (!strcmp(argv[1], "-c") && argc < 9) ||
        (!strcmp(argv[1], "-d") && argc < 4) ||
        (strcmp(argv[1], "-c") && strcmp(argv[1], "-d")))
        usage(argv[0]);

    /* compress */
    if (!strcmp(argv[1], "-c")) {

        k = atoi(argv[2]);
        w = atoi(argv[3]);
        h = atoi(argv[4]);
        tile_w = atoi(argv[5]);
        tile_h = atoi(argv[6]);
        input_path = argv[7];
        output_path = argv[8];

        savetxt = false;
        if (argc == 10 && !strcmp("-w", argv[9]))
            savetxt = true;

        compress(k, w, h, tile_w, tile_h, input_path, output_path, savetxt);
    }

    /* decompress */
    else if (!strcmp(argv[1], "-d")) {
        input_path = argv[2];
        output_path = argv[3];

        decompress(input_path, output_path);
    }
}

