//------------------------------------------------------------------------------
// Copyright (c) 2021 Ando Ki
// 3-clause BSD license.
//------------------------------------------------------------------------------
// VERSION = 2021.01.02.
//------------------------------------------------------------------------------

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include "dpi_png_wrapper.h"

int verbose=0;

int dpi_png_verbose_set( int verbosity )
{
    verbose = verbosity;   
    return verbose;
}

int dpi_png_verbose_get( void )
{
    return verbose;
}

// It initializes image_info
int dpi_png_init( image_info_t *image_info )
{
    image_info->ImageWidth    =0; // width in pixel
    image_info->ImageHeight   =0; // height in pixel
    image_info->BytesPerPixel =0; // bytes per pixel
    image_info->BytesPerLine  =0; //
    image_info->SkipPerLine   =0; //
    image_info->ImageSize     =0; // ImageSize in Byte including skip (in bytes)
    image_info->pBitMap       =0; // pixels order: R/G/B (Red comes first)
    return 0;
}

// It parses BMP file and fills image_info structure.
int dpi_png_read( char *file_name
                , image_info_t *image_info )
{
    image_info->pBitMap = stbi_load( file_name
                                   ,&(image_info->ImageWidth)   // width in pixel
                                   ,&(image_info->ImageHeight)  // height in pixel
                                   ,&(image_info->BytesPerPixel)// bytes per pixel
                                   , 0);
    if (image_info->pBitMap==NULL) {
        return -1;
    } else {
        image_info->BytesPerLine = image_info->BytesPerPixel*image_info->ImageWidth;
        image_info->SkipPerLine  = 0;
        image_info->ImageSize    = image_info->BytesPerLine*image_info->ImageHeight;
        return 0;
    }
}

// It creates BMP file using image_info structure.
int dpi_png_write ( char *file_name
                  , image_info_t *image_info)
{
    int ret;
    ret = stbi_write_png( file_name
                        , image_info->ImageWidth
                        , image_info->ImageHeight
                        , image_info->BytesPerPixel
                        , image_info->pBitMap
                        , 0);
    if (ret==1) {
      return 0;
    } else {
      return -1;
    }
}

// It gets pixels data from C.
// It resutns the number of bytes on sucess.
// It returns negative on failure.
int dpi_png_get_pixels( svOpenArrayHandle pixels
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
// It resutns the number of bytes on sucess.
// It returns negative on failure.
int dpi_png_put_pixels( const svOpenArrayHandle pixels
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
int dpi_png_wrapup( image_info_t *image_info )
{
    stbi_image_free(image_info->pBitMap);
    return 0;
}

//------------------------------------------------------------------------------
// Revision History
//
// 2021.01.02: Start by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
