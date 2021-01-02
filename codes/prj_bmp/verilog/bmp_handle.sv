
// Useful for non-windows compile targets
typedef struct {                  /**** BMP file header structure ****/
      shortint unsigned bfType;      /* Magic number for file 0x4D42 ('B''M') */
      int      unsigned bfSize;      /* Size of file */
      shortint unsigned bfReserved1; /* Reserved */
      shortint unsigned bfReserved2; /* ... */
      int      unsigned bfOffBits;   /* Offset to bitmap data */
} BITMAPFILEHEADER;

typedef struct {                      /**** BMP file info structure ****/
      int      unsigned biSize;          /* Size of info header */
      int      unsigned biWidth;         /* Width of image */
      int      unsigned biHeight;        /* Height of image */
      shortint unsigned biPlanes;        /* Number of color planes */
      shortint unsigned biBitCount;      /* Number of bits per pixel */
      int      unsigned biCompression;   /* Type of compression to use */
      int      unsigned biSizeImage;     /* Size of image data including skip */
      int      unsigned biXPelsPerMeter; /* X pixels per meter */
      int      unsigned biYPelsPerMeter; /* Y pixels per meter */
      int      unsigned biClrUsed;       /* Number of colors used */      
      int      unsigned biClrImportant;  /* Number of important colors */
} BITMAPINFOHEADER; // DIB header

typedef struct {
      byte unsigned rgbBlue; 
      byte unsigned rgbGreen; 
      byte unsigned rgbRed; 
      byte unsigned rgbReserved; 
} RGBQUAD; 

typedef struct { 
      BITMAPINFOHEADER bmiHeader; 
      RGBQUAD          bmiColors[1]; 
} BITMAPINFO;

typedef struct {
       BITMAPFILEHEADER header      ; // Bitmap header
       BITMAPINFOHEADER info        ; // Bitmap information
       int     unsigned ImageWidth  ; // width in pixel
       int     unsigned ImageHeight ; // height in pixel
       int     unsigned BitsPerPixel; // bits per pixel
       int     unsigned BytesPerLine; //
       int     unsigned SkipPerLine ; //
       int     unsigned DibSize     ; // DIB header size in bytes
       int     unsigned ClrSize     ; // Color table size in bytes
       int     unsigned ImageSize   ; // ImageSize in Byte including skip (in bytes)
       chandle          pDibHdr     ; // DIB (device independent bitmap)
       chandle          pColor      ; // 
       chandle          pBitMap     ; // pixels
} image_info_t;

import "DPI-C" function int
dpi_bmp_init( inout image_info_t image_info );

import "DPI-C" function int
dpi_bmp_read( input string file_name, inout image_info_t image_info, input int upsidedown );

import "DPI-C" function int
dpi_bmp_write ( input string file_name, inout image_info_t image_info, input int upsidedown );

import "DPI-C" function int
dpi_bmp_get_pixels( output byte pixel[], inout image_info_t image_info );

import "DPI-C" function int
dpi_bmp_put_pixels( input byte pixel[], inout image_info_t image_info );

import "DPI-C" function int
dpi_bmp_wrapup ( inout image_info_t image_info );

import "DPI-C" function int
dpi_bmp_verbose_set( int verbosity );

import "DPI-C" function int
dpi_bmp_verbose_get( );

