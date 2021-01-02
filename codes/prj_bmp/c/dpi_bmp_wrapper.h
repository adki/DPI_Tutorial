#ifndef DPI_BMP_WRAPPER_H
#define DPI_BMP_WRAPPER_H
//------------------------------------------------------------------------------
// Copyright (c) 2020-2021 Ando Ki
// 3-clause BSD license.
//------------------------------------------------------------------------------
// dpi_bmp_wrapper.h
//------------------------------------------------------------------------------
// VERSION = 2021.01.02.
//------------------------------------------------------------------------------
#include "bmp_handle.h"
#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif

// It initialize image_info data structure
extern int dpi_bmp_init  ( image_info_t *image_info );
// It parses BMP file and fills image_info structure.
extern int dpi_bmp_read  ( char *file_name  , image_info_t *image_info, int upsidedown );
// It creates BMP file using image_info structure.
extern int dpi_bmp_write ( char *file_name  , image_info_t *image_info, int upsidedown );
// It gets pixel data from C.
extern int dpi_bmp_get_pixels( svOpenArrayHandle pixel, image_info_t *image_info );
// It puts pixel data to C.
extern int dpi_bmp_put_pixels( const svOpenArrayHandle pixel, image_info_t *image_info );
// It frees all.
extern int dpi_bmp_wrapup( image_info_t *image_info );

extern int dpi_bmp_verbose_set( int verbosity );
extern int dpi_bmp_verbose_get( void );

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
// Revision History
//
// 2021.01.02: 'dpi_bmp_get_pixels()' added
// 2021.01.02: 'dpi_bmp_wrapup()' added
// 2020.08.21: Start by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
#endif
