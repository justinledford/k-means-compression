# kmc-ic
Compress .rgb (pixmap) image files using k-means clustering  


## Usage
```
compress: ./compress -c <k> <w> <h> <tile_w> <tile_h > <input> <output>
decompress: ./compress -d <input> <output>
```
Note: k should be a power of 2
____
ImageMagick can be used to convert to and from .rgb files:
```
convert image.jpeg image.rgb
convert -size <w x h> -depth 8 image.rgb image.jpeg
```
## Example

| Original      | Compressed (k = 128, 2x2 tiles) |          
| :-------------: |:-------------:|
| <img src ="https://github.com/justinledford/kmc-ic/raw/master/examples/doge.jpg" width="338">     | <img src="https://github.com/justinledford/kmc-ic/raw/master/examples/doge_compressed.jpg" width="338">|
| 768K pixmap .rgb      | 66K compressed     |

*Images have been converted to JPEG to display here*
 



