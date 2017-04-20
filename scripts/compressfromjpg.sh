#!/bin/bash

# Convert JPEG to RGB, Compress RGB, decompress,
# then convert to JPEG  to view results
#
# ImageMagick required

if [ "$#" -ne 4 ]; then
    echo "Usage: ./compressfromjpg.sh k tile_w tile_h input"
    exit
fi

K=$1
TILE_W=$2
TILE_H=$3
INPUT=$4
INPUT_RGB=`basename $INPUT | sed 's/\(.*\)\.jpg/\1.rgb/'`
OUTPUT_BIN=`basename $INPUT | sed 's/\(.*\)\.jpg/\1_c.bin/'`
OUTPUT_RGB=`basename $INPUT | sed 's/\(.*\)\.jpg/\1_c.rgb/'`
OUTPUT_JPG=`basename $INPUT | sed 's/\(.*\)\.jpg/\1_c.jpg/'`

W=`identify -format "%[fx:w]" $INPUT`
H=`identify -format "%[fx:h]" $INPUT`

convert $INPUT $INPUT_RGB &&
    echo 'Converted JPG to RGB' &&
    ./compress -c $K $W $H $TILE_W $TILE_H $INPUT_RGB $OUTPUT_BIN -w &&
    echo 'Converted RGB to k-means compresion' &&
    ./compress -d $OUTPUT_BIN $OUTPUT_RGB &&
    echo 'Converted from kmeans compression to RGB' &&
    convert -size ${W}x${H} -depth 8 -quality 100 \
        $OUTPUT_RGB $OUTPUT_JPG &&
    echo 'Converted from RGB to JPG' &&
    display $OUTPUT_JPG
