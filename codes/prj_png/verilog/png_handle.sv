
typedef struct {
       int     unsigned ImageWidth   ; // width in pixel
       int     unsigned ImageHeight  ; // height in pixel
       int     unsigned BytesPerPixel; // bytes per pixel
       int     unsigned BytesPerLine ; //
       int     unsigned SkipPerLine  ; //
       int     unsigned ImageSize    ; // ImageSize in Byte including skip (in bytes)
       chandle          pBitMap      ; // pixels
} image_info_t;

import "DPI-C" function int
dpi_png_init( inout image_info_t image_info );

import "DPI-C" function int
dpi_png_read( input string file_name, inout image_info_t image_info );

import "DPI-C" function int
dpi_png_write ( input string file_name, inout image_info_t image_info );

import "DPI-C" function int
dpi_png_get_pixels( output byte pixel[], inout image_info_t image_info );

import "DPI-C" function int
dpi_png_put_pixels( input byte pixel[], inout image_info_t image_info );

import "DPI-C" function int
dpi_png_wrapup ( inout image_info_t image_info );

import "DPI-C" function int
dpi_png_verbose_set( int verbosity );

import "DPI-C" function int
dpi_png_verbose_get( );

