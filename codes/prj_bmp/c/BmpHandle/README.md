# BmpHandle

This is an easy to use BMP (Microsoft Bitmap) handling API.
Tested on Ubuntu 16.04, MinGW, and Cygwin.

It will be staight forward to use the API, refer to 'src/main_bmp.c' or 'src/main_gray.c'.

## License
This is licensed with the 3-clause BSD license to make the library useful in open and closed source products independent of their licensing scheme.

# Basic steps of calling API will be

0) image_info_t im;
1) bmp_init(&im);
2) bmp_read(file_name_to_read, &im, 0);
3) do whatever you want on pixel data which is im.pBitMap[] and it follows BMP file pixel format
    * 8-bit BGR (little-endian format, B comes first), i.e., im.BitsPerPixel will be 24.
    * each line will be im.BytesPerLine bytes long, which may not be equal to (im.ImageWidth\*3)
3) bmp_write(file_name_to_write, &im, 0);
4) bmp_wrapup(&im);

# To read and then wirte a BMP file,

1) $ make -f Makefile.main_bmp
2) $ ./bmp_extractor -i images/lenna_320x240.bmp -o a.bmp

It reads 'images/lenna_320x240.bmp' file and then writes it to 'a.bmp' file.
You can see the bmp file on Ubuntu as follows.

$ display a.bmp

# To extract color components from BMP file,

1) $ make -f Makefile.main_bmp
2) $ ./bmp_extract -i images/lenna_320x240.bmp -r r.raw -g g.raw -b b.raw -y luma.raw

It reads 'images/leena_320x240.bmp' file and extracts each color components and then writes them.
It should be noted that the resultant color component file only contains raw data and it does not have any graphic information.

# To make a gray-level BMP file,

1) $ make -f Makefile.main_gray
2) $ ./bmp_gray -w 320 -t 240 -y luma.raw -o x.bmp

It reads data from 'luma.raw' file and it will be luminance values of each pixel.
Then it writes a gray-level BMP file as 'x.bmp'.

To check the BMP file,

$ display x.bmp

That's it.
