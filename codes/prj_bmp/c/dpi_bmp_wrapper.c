//------------------------------------------------------------------------------
// Copyright (c) 2020-2021 Ando Ki
// 3-clause BSD license.
//------------------------------------------------------------------------------
// VERSION = 2021.01.02.
//------------------------------------------------------------------------------
#include "dpi_bmp_wrapper.h"

int verbose=0;

int dpi_bmp_verbose_set( int verbosity )
{
    verbose = verbosity;   
    return verbose;
}

int dpi_bmp_verbose_get( void )
{
    return verbose;
}

// It initializes image_info
int dpi_bmp_init( image_info_t *image_info )
{
    int ret;
    ret = bmp_init( image_info );
    return ret;
}

// It parses BMP file and fills image_info structure.
int dpi_bmp_read( char *file_name
                , image_info_t *image_info
                , int upsidedown )
{
    int ret;
    ret = bmp_read( file_name
                  , image_info
                  , upsidedown );

    return ret;
}

// It creates BMP file using image_info structure.
int dpi_bmp_write ( char *file_name
                  , image_info_t *image_info
                  , int upsidedown )
{
    int ret;
    ret = bmp_write( file_name
                   , image_info
                   , upsidedown );

    return ret;
}

// It gets pixels data from C.
//    - pixel format: B/G/R (BMP order)
//    - upside down
// It resutns the number of bytes on sucess.
// It returns negative on failure.
int dpi_bmp_get_pixels( svOpenArrayHandle pixels
                      , image_info_t *image_info )
{
    int low = svLow(pixels, 1);
    int high = svHigh(pixels, 1);
    unsigned char *pt=image_info->pBitMap;
    int j = 0;
//printf("%s() low=%d high=%d\n", __func__, low, high);
//printf("%s() 0x%02X:%02X:%02X\n", __func__, pt[0], pt[1], pt[2]);
    for(int i=low; i<=high; i++) {
        *((char*)svGetArrElemPtr1(pixels, i)) = pt[j++];
    }
    return j;
}

// It puts pixels data to C.
//    - pixel format: B/G/R (BMP order)
//    - upside down
// It resutns the number of bytes on sucess.
// It returns negative on failure.
int dpi_bmp_put_pixels( const svOpenArrayHandle pixels
                      , image_info_t *image_info )
{
    int low = svLow(pixels, 1);
    int high = svHigh(pixels, 1);
    unsigned char *pt=image_info->pBitMap;
    int j = 0;
//printf("%s() low=%d high=%d\n", __func__, low, high);
//printf("%s() 0x%02X:%02X:%02X\n", __func__, pt[0], pt[1], pt[2]);
    for(int i=low; i<=high; i++) {
        pt[j++] = *((char*)svGetArrElemPtr1(pixels, i));
    }

    return j;
}

// It frees all.
int dpi_bmp_wrapup( image_info_t *image_info )
{
    int ret;
    ret = bmp_wrapup(image_info);
    return ret;
}

//------------------------------------------------------------------------------
// Revision History
//
// 2021.01.02: 'dpi_bmp_wrapup()' added
// 2020.08.21: Start by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
