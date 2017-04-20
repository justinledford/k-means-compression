#!/bin/bash

# Compress RGB, decompress, then convert to JPEG
# to view results
#
# ImageMagick required

if [ "$#" -ne 6 ]; then
    echo "Usage: ./compress2jpg.sh k w h tile_w tile_h input"
    exit
fi

K=$1
W=$2
H=$3
TILE_W=$4
TILE_H=$5
INPUT=$6
OUTPUT_BIN=`basename $INPUT | sed 's/\(.*\)\.rgb/\1_c.bin/'`
OUTPUT_RGB=`basename $INPUT | sed 's/\(.*\)\.rgb/\1_c.rgb/'`
OUTPUT_JPG=`basename $INPUT | sed 's/\(.*\)\.rgb/\1_c.jpg/'`

./compress -c $K $W $H $TILE_W $TILE_H $INPUT $OUTPUT_BIN -w &&
    ./compress -d $OUTPUT_BIN $OUTPUT_RGB &&
    convert -size ${W}x${H} -depth 8 -quality 100 \
        $OUTPUT_RGB $OUTPUT_JPG &&
    display $OUTPUT_JPG
