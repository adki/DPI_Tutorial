//------------------------------------------------------------------------------
// Copyright (c) 2017-2018 by Ando Ki.
// 3-clause BSD license.
//------------------------------------------------------------------------------
//  bmp_handle.c
//------------------------------------------------------------------------------
//  VERSION: 2018.08.02.
//------------------------------------------------------------------------------
// Note:
// * FRAME is an area in the memory containing image data.
// * IMAGE is a sub-area in the frame which will be displayed.
// * DISPLAY is a physical screen to display the image.
// Thus, IMAGE is equal to or smaller than FRAME.
// And IMAGE is equal to or smaller than DISPLAY.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "bmp_handle.h"

//------------------------------------------------------------------------------
//         +------------+
//         |File Header |
//         +------------+
//         |DIB Header  | - BMP Header
//         |            |
//         +------------+
//         |Color Table | - optional (no for 24-bit RGB case)
//         +------------+
//         |Bit Map     |
//         +------------+
//------------------------------------------------------------------------------
// 24-bit RGB case
// - No color table
//
// ImageSize = BytesPerLine * ImageHeight
//   +-------------------------------+------+
//  0|                               |      |
//   +-------------------------------+------+
//  1|                               |      |
//   +-------------------------------+------+
//
//  x|                               |      | (ImageHeight-1)
//   +-------------------------------+------+
//     BytesPerPixel*ImageWidth        SkipPerLine
//     BytesPerline= (BytesPerPixel*ImageWidth + SkipPerLine)
//
//------------------------------------------------------------------------------
// 8-bit Gray case
//
// - image size will be 0, but color table resides
// - make 256 color table between DIB and BITMAP
//   00 00 00 00 01 01 01 00 ...
//   B  G  R  -  B  G  R  -
// - BITMAP contains index of the color table
//------------------------------------------------------------------------------
// * It parses BMP file and fills image imformation structure.
// * When 'upsidedown' is 1, the image bitmap will be read upside-down.
int bmp_read ( char *bmp , image_info_t *image_info, int upsidedown )
{
   BITMAPFILEHEADER header      ; // Bitmap header
   BITMAPINFOHEADER info        ; // Bitmap information
   unsigned int     FileSize    = 0;
   unsigned int     DibSize     = 0;
   unsigned int     ClrSize     = 0;
   unsigned int     ImageSize   = 0;
   unsigned int     ImageWidth  = 0; // width in pixel
   unsigned int     ImageHeight = 0; // height in pixel
   unsigned int     BitsPerPixel= 0; // bits per pixel
   unsigned int     PixelCount  = 0; // width*height
   unsigned int     BytesPerLine= 0;
   unsigned int     SkipPerLine = 0;
   unsigned int     Planes      = 0;
   unsigned int     Compression = 0;
   unsigned char   *pBitMap = NULL; // pixels
   unsigned char   *pColor  = NULL; // Color table
   unsigned char   *pDibHdr = NULL; // DIB header (i.e., bitmap infor)
   extern int verbose;
   //------------------------------------------------------
   FILE *fp_get=NULL;
   //------------------------------------------------------
   if (bmp==NULL||bmp[0]=='\0'||image_info==NULL) return 1;
   //------------------------------------------------------
   fp_get = fopen(bmp, "rb");
   if ( fp_get == NULL ) {
      fprintf(stderr, "%s() Error opening BMP file \"%s\"\n", __func__, bmp);
      return(1);
   }
   // Read the file header and info sections...
   if (fread(&header, 1, sizeof(BITMAPFILEHEADER), fp_get) < sizeof(BITMAPFILEHEADER)) {
      fprintf(stderr, "Error reading BMP header for %s\n", bmp);
      fclose(fp_get);
      return(1);
   }
   //------------------------------------------------------
   // Check for BM magic number
   if (header.bfType != 0x4d42) { // note little-endia: 0x4D='M', 0x42='B'
      fprintf(stderr, "File %s does not appear to be a BMP file.\n", bmp);
      fclose(fp_get);
      return(1);
   }
   //------------------------------------------------------
   FileSize      = header.bfSize; // get DIB header size
   //------------------------------------------------------
   if (fread(&info, 1, sizeof(BITMAPINFOHEADER), fp_get) < sizeof(BITMAPINFOHEADER)) {
      fprintf(stderr, "Couldn't read the bitmap info header for %s.\n", bmp);
      fclose(fp_get);
      return(1);
   }
   //------------------------------------------------------
   ImageWidth    = info.biWidth;
   ImageHeight   = info.biHeight;
   Planes        = info.biPlanes;
   BitsPerPixel  = info.biBitCount;
   Compression   = info.biCompression;
   DibSize       = info.biSize;
   ImageSize     = info.biSizeImage;
   BytesPerLine  = (((ImageWidth*BitsPerPixel)/8)+3)/4;
   BytesPerLine *= 4; // must be a multiple of four
   SkipPerLine   = BytesPerLine - (ImageWidth*info.biBitCount)/8;
   PixelCount    = ImageWidth * ImageHeight;
   //------------------------------------------------------
   // read DIB header
   if (fseek(fp_get, sizeof(BITMAPFILEHEADER), SEEK_SET)) {
       fprintf(stderr, "Couldn't reposition stream position indicator\n");
       fclose(fp_get);
       return(1);
   }
   pDibHdr = (unsigned char*)malloc(DibSize);
   if (pDibHdr==NULL) {
      fprintf(stderr, "Couldn't allocate\n");
      fclose(fp_get);
      return(1);
   }
   if (fread(pDibHdr, 1, DibSize, fp_get) < DibSize) {
      fprintf(stderr, "Couldn't read DIB header for %s.\n", bmp);
      fclose(fp_get);
      free(pDibHdr); pDibHdr = NULL;
      return(1);
   }
if (verbose) {
BITMAPINFOHEADER *xpt=(BITMAPINFOHEADER*)pDibHdr;
printf("biSize         =0x%08X(%d)\n", xpt->biSize         , xpt->biSize         );
printf("biWidth        =0x%08X(%d)\n", xpt->biWidth        , xpt->biWidth        );
printf("biHeight       =0x%08X(%d)\n", xpt->biHeight       , xpt->biHeight       );
printf("biPlanes       =0x%08X(%d)\n", xpt->biPlanes       , xpt->biPlanes       );
printf("biBitCount     =0x%08X(%d)\n", xpt->biBitCount     , xpt->biBitCount     );
printf("biCompression  =0x%08X(%d)\n", xpt->biCompression  , xpt->biCompression  );
printf("biSizeImage    =0x%08X(%d)\n", xpt->biSizeImage    , xpt->biSizeImage    );
printf("biXPelsPerMeter=0x%08X(%d)\n", xpt->biXPelsPerMeter, xpt->biXPelsPerMeter);
printf("biYPelsPerMeter=0x%08X(%d)\n", xpt->biYPelsPerMeter, xpt->biYPelsPerMeter);
printf("biClrUsed      =0x%08X(%d)\n", xpt->biClrUsed      , xpt->biClrUsed      );
printf("biClrImportant =0x%08X(%d)\n", xpt->biClrImportant , xpt->biClrImportant );
}
   //------------------------------------------------------
   if ((info.biBitCount==24)&&((sizeof(BITMAPFILEHEADER)+DibSize)!=header.bfOffBits)) {
      fprintf(stderr, "Offset and header does not match\n");
   }
   //------------------------------------------------------
#if 0
   if (info.biBitCount!=24) {
      fprintf(stderr, "May not handle bitmap %d-bpp\n", info.biBitCount);
      //fclose(fp_get);
      //return(1);
   }
#endif
   //------------------------------------------------------
   // read color table if any
   // only for bits-per-pixel is 8.
   if ((info.biCompression==0)&&(info.biSizeImage==0)&&
       (info.biBitCount==8)) {
        ClrSize = 256*4;
        pColor = (unsigned char *)malloc(ClrSize);
        if (pColor==NULL) {
            fprintf(stderr, "Couldn't allocate\n");
            fclose(fp_get);
            free(pDibHdr); pDibHdr = NULL;
            return(1);
        }
        rewind(fp_get); // fseek(fp_get, 0, SEEK_SET);
        if (fseek(fp_get, sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER), SEEK_SET)) {
            fprintf(stderr, "Couldn't reposition stream position indicator\n");
            fclose(fp_get);
            free(pColor); pColor = NULL;
            free(pDibHdr); pDibHdr = NULL;
            return(1);
        }
        if (fread(pColor, 1, 256*4, fp_get) < 256*4) {
           fprintf(stderr, "Couldn't read the color table for %s.\n", bmp);
           fclose(fp_get);
           free(pColor); pColor = NULL;
           free(pDibHdr); pDibHdr = NULL;
           return(1);
        }
   }
   //------------------------------------------------------
   if (ImageSize!=(BytesPerLine*ImageHeight)) {
         if (ImageSize==0) {
            //fprintf(stderr, "Need color table\n");
            ImageSize = BytesPerLine*ImageHeight;
         } else {
            fprintf(stderr, "Image size mis-match %d %d*%d=%d\n",
                    ImageSize,BytesPerLine,ImageHeight,(BytesPerLine*ImageHeight));
         }
   }
   pBitMap = (unsigned char*)malloc(ImageSize);
   if (pBitMap==NULL) {
         fprintf(stderr, "Couldn't allocate memory for the pixel data: %d\n", ImageSize);
         fclose(fp_get);
         free(pDibHdr); pDibHdr = NULL;
         return(1);
   }
   if (verbose) {
       printf("ImageWidth=%d ImageHeight=%d\n", ImageWidth, ImageHeight);
       printf("PixelCount=%d BitsPerPixel=%d\n", PixelCount, BitsPerPixel);
       printf("ByteCountForPixels=0x%x (%d) bytes\n", ImageSize, ImageSize);
       printf("BytesPerLine=%d SkipPerLine=%d\n", BytesPerLine, SkipPerLine);
       printf("Planes=%d\n", info.biPlanes);
       printf("Compression=%s\n", (info.biCompression==0) ? "BI_RGB"
                                : (info.biCompression==1) ? "BI_RLE8"
                                : (info.biCompression==2) ? "BI_RLE4"
                                : (info.biCompression==3) ? "BI_BITFIELDS"
                                : (info.biCompression==4) ? "BI_JPEG"
                                : (info.biCompression==5) ? "BI_PNG"
                                : "UNKNOWN");
       printf("Color=%d\n", info.biClrUsed);
       fflush(stdout);
   }
   //------------------------------------------------------
   rewind(fp_get); // fseek(fp_get, 0, SEEK_SET);
   if (fseek(fp_get, header.bfOffBits, SEEK_SET)) {
       fprintf(stderr, "Couldn't reposition stream position indicator\n");
       fclose(fp_get);
       free(pColor); pColor = NULL;
       free(pDibHdr); pDibHdr = NULL;
       return(1);
   }
   if (upsidedown==1) {
       unsigned char *pt=pBitMap;
       pt += (ImageSize - BytesPerLine);
       int i=0;
       for (i=0; i<ImageHeight; i++) {
            if (fread(pt, 1, BytesPerLine, fp_get) < BytesPerLine) {
               fprintf(stderr, "Couldn't read the pixel data for %s.\n", bmp);
               fclose(fp_get);
               free(pColor); pColor = NULL;
               free(pDibHdr); pDibHdr = NULL;
               return(1);
            }
            pt -= BytesPerLine;
       }
   } else {
       if (fread(pBitMap, 1, ImageSize, fp_get) < ImageSize) {
          fprintf(stderr, "Couldn't read the pixel data for %s.\n", bmp);
          fclose(fp_get);
          free(pColor); pColor = NULL;
          free(pDibHdr); pDibHdr = NULL;
          return(1);
       }
   }
   fclose(fp_get);
   //------------------------------------------------------
   image_info->header       = header      ; // Bitmap header
   image_info->info         = info        ; // Bitmap information
   image_info->ImageWidth   = ImageWidth  ; // width in pixel
   image_info->ImageHeight  = ImageHeight ; // height in pixel
   image_info->BitsPerPixel = BitsPerPixel; // bits per pixel
   image_info->BytesPerLine = BytesPerLine; 
   image_info->SkipPerLine  = SkipPerLine ; 
   image_info->DibSize      = DibSize     ; 
   image_info->ClrSize      = ClrSize     ; 
   image_info->ImageSize    = ImageSize   ;
#if 0
printf("DibSize=%d ClrSize=%d ImageSize=%d\n",
   DibSize     , 
   ClrSize     , 
   ImageSize   );
#endif
   if (image_info->pDibHdr!=NULL) free(image_info->pDibHdr);
   image_info->pDibHdr      = pDibHdr     ; // DIB header
   if (image_info->pColor !=NULL) free(image_info->pColor );
   image_info->pColor       = pColor      ; // Color table
   if (image_info->pBitMap!=NULL) free(image_info->pBitMap);
   image_info->pBitMap      = pBitMap     ; // pixels
   //------------------------------------------------------
   if ((sizeof(BITMAPFILEHEADER)+DibSize+ClrSize+ImageSize)!=
        header.bfSize) {
        fprintf(stderr, "Error file size mismatch\n");
        header.bfSize = (sizeof(BITMAPFILEHEADER)+DibSize+ClrSize+ImageSize);
   }
   //------------------------------------------------------
   return (0);
}

//------------------------------------------------------------------------------
// * It creates BMP file using image imformation structure.
// * When 'upsidedown' is 1, the image bitmap will be upside-down.
int bmp_write ( char *bmp , image_info_t *image_info, int upsidedown )
{
   if (  bmp==NULL||bmp[0]=='\0'
       ||image_info==NULL||image_info->pBitMap==NULL) return 1;
   //------------------------------------------------------
   FILE *fp_put=NULL;
   //------------------------------------------------------
   fp_put = fopen(bmp, "wb");
   if ( fp_put == NULL ) {
      fprintf(stderr, "%s() Error opening BMP file \"%s\" for writing\n", __func__, bmp);
      return(1);
   }
   //------------------------------------------------------
   if ((image_info->BitsPerPixel==24)&&
       ((sizeof(BITMAPFILEHEADER)+image_info->DibSize)!=image_info->header.bfOffBits)) {
      fprintf(stderr, "Offset and header does not match\n");
   }
#if 0
   if (image_info->info.biSize!=sizeof(BITMAPINFOHEADER)) {
      fprintf(stderr, "Error infor size mismatch\n");
      fclose(fp_put);
      return(1);
   }
#endif
#if 0
   int sz =sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO);
   // file_header + DIB_header + COLOR_table + bitmap
   if (image_info->header.bfOffBits!=sz) {
      fprintf(stderr, "Error offset mismatch: %d %d\n",
                       image_info->header.bfOffBits, sz);
   }
#endif
   //------------------------------------------------------
   // Write the file header and info sections...
   if (fwrite(&image_info->header, sizeof(BITMAPFILEHEADER), 1, fp_put) < 1) {
      fprintf(stderr, "Error writing BMP header for %s\n", bmp);
      fclose(fp_put);
      return(1);
   }
#if 0
   if (fwrite(image_info->pDibHdr, image_info->DibSize, 1, fp_put) < 1) {
      fprintf(stderr, "Error writing DIB for %s\n", bmp);
      fclose(fp_put);
      return(1);
   }
#else
   if (fwrite(&image_info->info, image_info->info.biSize, 1, fp_put) < 1) {
      fprintf(stderr, "Error writing DIB for %s\n", bmp);
      fclose(fp_put);
      return(1);
   }
#endif
   if ((image_info->info.biCompression==0)&&(image_info->info.biSizeImage==0)&&
       (image_info->info.biBitCount==8)&&(image_info->pColor!=NULL)) {
       if (fwrite(image_info->pColor, image_info->ClrSize, 1, fp_put) < 1) {
          fprintf(stderr, "Error writing Color table for %s\n", bmp);
          fclose(fp_put);
          return(1);
       }
   }
   int loc = (sizeof(BITMAPFILEHEADER)+image_info->DibSize+image_info->ClrSize);
   if (loc>image_info->header.bfOffBits) {
       fprintf(stderr, "Error header-bitmap mis-match\n");
   } else if (loc<image_info->header.bfOffBits) {
       rewind(fp_put); // fseek(fp_put, 0, SEEK_SET);
       if (fseek(fp_put, image_info->header.bfOffBits, SEEK_SET)) {
           fprintf(stderr, "Couldn't reposition stream position indicator\n");
           fclose(fp_put);
           return(1);
       }
   }
   if (upsidedown==1) {
       unsigned char *pt = image_info->pBitMap;
       pt += (image_info->ImageSize - image_info->BytesPerLine);
       int i;
       for (i=0; i<image_info->ImageHeight; i++) {
            if (fwrite(pt, 1, image_info->BytesPerLine, fp_put)
                < image_info->BytesPerLine) {
               fprintf(stderr, "Error writing BMP info for %s\n", bmp);
               fclose(fp_put);
               return(1);
            }
            pt -= image_info->BytesPerLine;
       }
   } else {
     if (fwrite(image_info->pBitMap, 1, image_info->ImageSize, fp_put)
         < image_info->ImageSize) {
        fprintf(stderr, "Error writing BMP info for %s\n", bmp);
        fclose(fp_put);
        return(1);
     }
   }
   return fclose(fp_put);
}

//------------------------------------------------------------------------------
// It fills BITMAPFILEHEADER of image_info structure
int bmp_gen_file_header( image_info_t *image_info, int width, int height )
{
   if (image_info==NULL) return -1;
    image_info->header.bfType      = 0x4D42;// ('B''M')
    image_info->header.bfSize      = 54+(width*height*3); // it may need to add color table
    image_info->header.bfReserved1 = 0x0;
    image_info->header.bfReserved2 = 0x0;
    image_info->header.bfOffBits   = 54; // sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
    return 0;
}

//------------------------------------------------------------------------------
// It fills BITMAPINFOHEADER of image_info structure
int bmp_gen_img_header( image_info_t *image_info, int width, int height )
{
   if (image_info==NULL) return -1;
    image_info->info.biSize         = 40; /* Size of info header */
    image_info->info.biWidth        = width; /* Width of image */
    image_info->info.biHeight       = height; /* Height of image */
    image_info->info.biPlanes       = 1; /* Number of color planes */
    image_info->info.biBitCount     = 24; /* Number of bits per pixel */
    image_info->info.biCompression  = 0; /* Type of compression to use */
    image_info->info.biSizeImage    = width*height*3; /* Size of image data including skip */
    image_info->info.biXPelsPerMeter= 3780; /* X pixels per meter */
    image_info->info.biYPelsPerMeter= 3780; /* Y pixels per meter */
    image_info->info.biClrUsed      = 0; /* Number of colors used */      
    image_info->info.biClrImportant = 0; /* Number of important colors */
    return 0;
}

//------------------------------------------------------------------------------
int bmp_gen_info( image_info_t *image_info, int width, int height )
{
   if (image_info==NULL) return -1;
   bmp_gen_file_header( image_info, width, height );
   bmp_gen_img_header( image_info, width, height );
   image_info->ImageWidth   = width; // width in pixel
   image_info->ImageHeight  = height; // height in pixel
   image_info->BitsPerPixel = 24; // bits per pixel
   image_info->BytesPerLine = 0; //
   image_info->BytesPerLine  = (((width*24)/8)+3)/4;
   image_info->BytesPerLine *= 4; // must be a multiple of four
   image_info->SkipPerLine   = image_info->BytesPerLine - (width*24)/8;
 //image_info->DibSize      = sizeof(BITMAPINFOHEADER); // DIB header size in bytes
   image_info->DibSize      = 0;
   image_info->ClrSize      = 0; // Color table size in bytes
   image_info->ImageSize    = image_info->BytesPerLine*height; // ImageSize in Byte including skip (in bytes)
 //image_info->pDibHdr = (unsigned char*)&(image_info->info);
   image_info->pDibHdr = NULL;
   image_info->pColor  = NULL;
   image_info->pBitMap = NULL;
   return 0;
}

//------------------------------------------------------------------------------
// * It creates BMP file using image imformation structure.
// * comp Blue  --> 0
// * comp Green --> 1
// * comp Red   --> 2
//
// result data will be "widxhei" and each row does not have any paddings.
int comp_write ( char *fname, image_info_t *image_info, char *comp, int upsidedown )
{
   if ( fname==NULL||fname[0]=='\0'
      ||image_info==NULL||image_info->pBitMap==NULL
      ||comp==NULL||comp[0]=='\0') return 1;
   int offset;
        if (!strcmp(comp,"blue" )) offset=0;
   else if (!strcmp(comp,"green")) offset=1;
   else if (!strcmp(comp,"red"  )) offset=2;
   else if (!strcmp(comp,"luma" )) offset=3;
   else { fprintf(stderr, "Error componet %s\n", comp);
          return(1);
   }
   //------------------------------------------------------
   FILE *fp_put=NULL;
   //------------------------------------------------------
   fp_put = fopen(fname, "wb");
   if ( fp_put == NULL ) {
      fprintf(stderr, "Error opening \"%s\" file \"%s\" for writing\n", comp, fname);
      return(1);
   }
   int wid = image_info->ImageWidth;
   int hei = image_info->ImageHeight;
   int bpl = image_info->BytesPerLine;
#if 0
   printf("# COMPONENT: %s\n", comp);
   printf("# WIDTH: %d\n", wid);
   printf("# HEIGHT: %d\n", hei);
   printf("# BytePerLine: %d\n", bpl);
#endif

   unsigned char *pLine=(unsigned char*)malloc(bpl);
   if (pLine==NULL) {
      fprintf(stderr, "Error malloc\n");
      return(1);
   }
   unsigned char *pBitMap= image_info->pBitMap;
   if (upsidedown==1) {
       pBitMap += (image_info->ImageSize-bpl);
   }
   int line = 0;
   do { int pix;
        if (offset==3) { // luma
           for (pix=0; pix<wid; pix++) {
               double Y = 0.257*(double)pBitMap[pix*3+2]
                        + 0.504*(double)pBitMap[pix*3+1]
                        + 0.098*(double)pBitMap[pix*3+0]
                        + 16.5;
               pLine[pix] = ((int)Y)&0xFF;
           }
       } else { // rgb
           for (pix=0; pix<wid; pix++) {
                pLine[pix] = pBitMap[pix*3+offset]; // BGR format
           }
        }
        if (fwrite(pLine, 1, wid, fp_put)<wid) {
            fclose(fp_put);
            if (pLine!=NULL) free(pLine);
            fprintf(stderr, "Error writing %s\n", comp);
            return(1);
        }
        if (upsidedown==1) {
            pBitMap -= bpl;
        } else {
            pBitMap += bpl;
        }
        line++;
   } while (line<hei);

   if (pLine!=NULL) free(pLine);
   return fclose(fp_put);
}

//------------------------------------------------------------------------------
// It reads color-components and fills necessary fields in "image_info"
// and then returns.
// At this moment, only lumma/gray is read.
// When upsidedown is 1, the bit-map will be upside-down, i.e.,
// it reads raw data as upside-down.
// Note that BMP uses upside-down format.
int raw_read ( char *fname, image_info_t *image_info, int upsidedown )
{
   if (fname==NULL||fname[0]=='\0'||image_info==NULL)
       return 1;
   //------------------------------------------------------
   FILE *fp_luma=NULL;
   //------------------------------------------------------
   fp_luma = fopen(fname, "rb");
   if ( fp_luma == NULL ) {
      fprintf(stderr, "Error opening \"%s\" file for reading\n", fname);
      return(1);
   }
   //------------------------------------------------------
   unsigned char *pt=(unsigned char *)malloc(image_info->ImageSize);
   if (pt==NULL) {
       fprintf(stderr, "Error cannot allocate pBitMap\n");
       fclose(fp_luma);
       return (1);
   }
   image_info->pBitMap = pt;
   if (upsidedown==1) {
       pt += (image_info->ImageSize - image_info->BytesPerLine);
       int h;
       for (h=0; h<image_info->ImageHeight; h++) {
            if (fread(pt, 1, image_info->ImageWidth, fp_luma) < image_info->ImageWidth) {
               fprintf(stderr, "Error reading BMP header for %s\n", fname);
               fclose(fp_luma);
               if (image_info->pBitMap!=NULL) {
                   free(image_info->pBitMap);
                   image_info->pBitMap = NULL;
               }
               return(1);
            }
            pt -= image_info->BytesPerLine;
       }
   } else {
       int h;
       for (h=0; h<image_info->ImageHeight; h++) {
            if (fread(pt, 1, image_info->ImageWidth, fp_luma) < image_info->ImageWidth) {
               fprintf(stderr, "Error reading BMP header for %s\n", fname);
               fclose(fp_luma);
               if (image_info->pBitMap!=NULL) {
                   free(image_info->pBitMap);
                   image_info->pBitMap = NULL;
               }
               return(1);
            }
            pt += image_info->BytesPerLine;
       }
   }
   fclose(fp_luma);
   //------------------------------------------------------
   pt=(unsigned char *)malloc(image_info->ClrSize);
   if ((pt==NULL)||(image_info->ClrSize!=(256*4))) {
       fprintf(stderr, "Error cannot allocate pColor\n");
       if (image_info->pBitMap!=NULL) {
           free(image_info->pBitMap);
           image_info->pBitMap = NULL;
       }
       return (1);
   }
   image_info->pColor = pt;
   int i;
   for (i=0; i<256; i++) {
        pt[i*4+0] = i; // b
        pt[i*4+1] = i; // g
        pt[i*4+2] = i; // r
        pt[i*4+3] = 0x00;
   }
   //------------------------------------------------------
   BITMAPINFOHEADER *bpt=(BITMAPINFOHEADER*)malloc(image_info->DibSize);
   if ((pt==NULL)||(image_info->DibSize!=40)) {
       fprintf(stderr, "Error cannot allocate pDibHdr\n");
       if (image_info->pColor!=NULL) {
           free(image_info->pColor);
           image_info->pColor = NULL;
       }
       if (image_info->pBitMap!=NULL) {
           free(image_info->pBitMap);
           image_info->pBitMap = NULL;
       }
       return (1);
   }
   image_info->pDibHdr = (unsigned char*)bpt;
   bpt->biSize         = image_info->DibSize;   /* Size of info header */
   bpt->biWidth        = image_info->ImageWidth;/* Width of image */
   bpt->biHeight       = image_info->ImageHeight;/* Height of image */
   bpt->biPlanes       = 1;   /* Number of color planes */
   bpt->biBitCount     = 8;   /* Number of bits per pixel */
   bpt->biCompression  = 0;   /* Type of compression to use */
   bpt->biSizeImage    = 0;   /* Size of image data including skip */
   bpt->biXPelsPerMeter= 0xEC3;   /* X pixels per meter */
   bpt->biYPelsPerMeter= 0xEC3;   /* Y pixels per meter */
   bpt->biClrUsed      = 0;   /* Number of colors used */
   bpt->biClrImportant = 0;   /* Number of important colors */
   //------------------------------------------------------
   memcpy((void*)&image_info->info,
          (const void*)image_info->pDibHdr, sizeof(BITMAPINFOHEADER));
   //------------------------------------------------------
   image_info->header.bfType = 0x4D42;
   image_info->header.bfSize = sizeof(BITMAPFILEHEADER)
                             + image_info->DibSize
                             + image_info->ClrSize
                             + image_info->ImageSize;
   image_info->header.bfReserved1 = 0x0;
   image_info->header.bfReserved2 = 0x0;
   image_info->header.bfOffBits   = sizeof(BITMAPFILEHEADER)
                                  + image_info->DibSize
                                  + image_info->ClrSize;
   //------------------------------------------------------
   return 0;
}

//------------------------------------------------------------------------------
int bmp_init( image_info_t *image_info )
{
   if (image_info==NULL) return -1;
   image_info->ImageWidth   = 0; // width in pixel
   image_info->ImageHeight  = 0; // height in pixel
   image_info->BitsPerPixel = 0; // bits per pixel
   image_info->BytesPerLine = 0; //
   image_info->SkipPerLine  = 0; //
 //image_info->DibSize      = sizeof(BITMAPINFOHEADER); // DIB header size in bytes
   image_info->DibSize      = 0;
   image_info->ClrSize      = 0; // Color table size in bytes
   image_info->ImageSize    = 0; // ImageSize in Byte including skip (in bytes)
 //image_info->pDibHdr = (unsigned char*)&(image_info->info);
   image_info->pDibHdr = NULL;
   image_info->pColor  = NULL;
   image_info->pBitMap = NULL;
   return 0;
}

//------------------------------------------------------------------------------
int bmp_wrapup( image_info_t *image_info )
{
   if (image_info==NULL) return -1;
   if (image_info->pDibHdr!=NULL) free(image_info->pDibHdr);
   if (image_info->pColor !=NULL) free(image_info->pColor );
   if (image_info->pBitMap!=NULL) free(image_info->pBitMap);
   return 0;
}

//------------------------------------------------------------------------------
// Revision history
//
// 2018.08.02: BITMAPV4HEADER case applied for 'bmp_write()',
//             where BITMAPHEADER is larger than 40, e.g., 124.
// 2018.03.20: Minor correction for G++
// 2017.12.18: Bug-fixed for 8-bit gray case.
// 2017.12.16: start by Ando Ki
//------------------------------------------------------------------------------
