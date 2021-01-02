#ifndef DPI_PNG_WRAPPER_H
#define DPI_PNG_WRAPPER_H
//------------------------------------------------------------------------------
// Copyright (c) 2021 Ando Ki
// 3-clause BSD license.
//------------------------------------------------------------------------------
// dpi_png_wrapper.h
//------------------------------------------------------------------------------
// VERSION = 2021.01.02.
//------------------------------------------------------------------------------
#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct image_info {
       unsigned int     ImageWidth   ; // width in pixel
       unsigned int     ImageHeight  ; // height in pixel
       unsigned int     BytesPerPixel; // bytes per pixel
       unsigned int     BytesPerLine ; //
       unsigned int     SkipPerLine  ; //
       unsigned int     ImageSize    ; // ImageSize in Byte including skip (in bytes)
       unsigned char *  pBitMap      ; // pixels order: R/G/B (Red comes first)
} image_info_t;

// It initialize image_info data structure
extern int dpi_png_init  ( image_info_t *image_info );
// It parses PNG file and fills image_info structure.
extern int dpi_png_read  ( char *file_name  , image_info_t *image_info );
// It creates PNG file using image_info structure.
extern int dpi_png_write ( char *file_name  , image_info_t *image_info );
// It gets pixel data from C.
extern int dpi_png_get_pixels( svOpenArrayHandle pixel, image_info_t *image_info );
// It puts pixel data to C.
extern int dpi_png_put_pixels( const svOpenArrayHandle pixel, image_info_t *image_info );
// It frees all.
extern int dpi_png_wrapup( image_info_t *image_info );

extern int dpi_png_verbose_set( int verbosity );
extern int dpi_png_verbose_get( void );

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
// Revision History
//
// 2021.01.02: Start by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
#endif
