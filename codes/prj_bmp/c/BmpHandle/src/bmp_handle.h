#ifndef BMP_HANDLE_H
#define BMP_HANDLE_H
//------------------------------------------------------------------------------
// Copyright (c) 2017-2018 Ando Ki
// 3-clause BSD license.
//------------------------------------------------------------------------------
// bmp_handle.h
//------------------------------------------------------------------------------
// Windows BMP file structures
//------------------------------------------------------------------------------
// VERSION = 2018.03.20.
//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32
   // Useful for non-windows compile targets
   typedef struct {                  /**** BMP file header structure ****/
         unsigned short bfType;      /* Magic number for file 0x4D42 ('B''M') */
         unsigned int   bfSize;      /* Size of file */
         unsigned short bfReserved1; /* Reserved */
         unsigned short bfReserved2; /* ... */
         unsigned int   bfOffBits;   /* Offset to bitmap data */
   } __attribute__((__packed__)) BITMAPFILEHEADER;
   
   typedef struct {                      /**** BMP file info structure ****/
         unsigned int   biSize;          /* Size of info header */
         unsigned int   biWidth;         /* Width of image */
         unsigned int   biHeight;        /* Height of image */
         unsigned short biPlanes;        /* Number of color planes */
         unsigned short biBitCount;      /* Number of bits per pixel */
         unsigned int   biCompression;   /* Type of compression to use */
         unsigned int   biSizeImage;     /* Size of image data including skip */
         unsigned int   biXPelsPerMeter; /* X pixels per meter */
         unsigned int   biYPelsPerMeter; /* Y pixels per meter */
         unsigned int   biClrUsed;       /* Number of colors used */      
         unsigned int   biClrImportant;  /* Number of important colors */
   } __attribute__((__packed__)) BITMAPINFOHEADER; // DIB header
   
   typedef struct {
         unsigned char rgbBlue; 
         unsigned char rgbGreen; 
         unsigned char rgbRed; 
         unsigned char rgbReserved; 
   } __attribute__((__packed__)) RGBQUAD; 
   
   typedef struct { 
         BITMAPINFOHEADER bmiHeader; 
         RGBQUAD          bmiColors[1]; 
   } __attribute__((__packed__)) BITMAPINFO;
#else
#      include <windows.h>
#endif /* !WIN32 */

//-----------------------------------------------------------
typedef struct image_info {
       BITMAPFILEHEADER header      ; // Bitmap header
       BITMAPINFOHEADER info        ; // Bitmap information
       unsigned int     ImageWidth  ; // width in pixel
       unsigned int     ImageHeight ; // height in pixel
       unsigned int     BitsPerPixel; // bits per pixel
       unsigned int     BytesPerLine; //
       unsigned int     SkipPerLine ; //
       unsigned int     DibSize     ; // DIB header size in bytes
       unsigned int     ClrSize     ; // Color table size in bytes
       unsigned int     ImageSize   ; // ImageSize in Byte including skip (in bytes)
       unsigned char *  pDibHdr     ; // DIB (device independent bitmap)
       unsigned char *  pColor      ; // 
       unsigned char *  pBitMap     ; // pixels (order: B/G/R - B comes first)
} image_info_t;

//-----------------------------------------------------------
// It parses BMP file and fills image_info structure.
extern int bmp_read  ( char *bmp  , image_info_t *image_info, int upsidedown );
// It creates BMP file using image_info structure.
extern int bmp_write ( char *bmp  , image_info_t *image_info, int upsidedown );
// It fills BITMAPFILEHEADER of image_info structure
extern int bmp_gen_file_header( image_info_t *image_info, int width, int height );
// It fills BITMAPINFOHEADER of image_info structure
extern int bmp_gen_img_header( image_info_t *image_info, int width, int height );
// It fills image_info structure
extern int bmp_gen_info( image_info_t *image_info, int width, int height );
// It creates a file containing values of color component specified.
extern int comp_write( char *fname, image_info_t *image_info, char *comp, int upsidedown);
// It creates a image_info structure using colo components data in the file.
extern int raw_read  ( char *fname, image_info_t *image_info, int upsidedown );
// It initializes image_info structure. It must be called before using it.
extern int bmp_init  ( image_info_t *image_info );
// It releases resoueces
extern int bmp_wrapup( image_info_t *image_info );

//-----------------------------------------------------------
#define bmp_get_width(X)        ((X)->ImageWidth)
#define bmp_get_height(X)       ((X)->ImageHeight)
#define bmp_get_pixel(X)        ((X)->pBitMap)
#define bmp_get_image_size(X)   ((X)->ImageSize)
#define bmp_set_width(X,Y)      (X)->ImageWidth = (Y)
#define bmp_set_height(X,Y)     (X)->ImageHeight = (Y)
#define bmp_set_pixel(X,Y)      (X)->pBitMap = (Y)
#define bmp_set_image_size(X,Y) (X)->ImageSize = (Y)

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
// Revision History
//
// 2018.03.20: macro added: bmp_width(), bmp_height(), bmp_pixel()
// 2017.12.18: Bug-fixed for 8-bit gray case.
// 2017.12.16: Start by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
#endif /*BMP_H_*/
