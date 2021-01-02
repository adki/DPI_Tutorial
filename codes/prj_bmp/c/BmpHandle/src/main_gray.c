//------------------------------------------------------------------------------
// Copyright (c) 2017 by Ando Ki.
// 3-clause BSD license.
//------------------------------------------------------------------------------
// VERSION = 2017.12.18.
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <sys/types.h>
#ifdef WIN32
#	include <windows.h>
#endif
#include "bmp_handle.h"

//------------------------------------------------------------------------------
char foutput[128]="\0"; // gray BMP file to be made
char fluma  [128]="\0"; // raw luma component (8-bit intensity)
unsigned int width =0;
unsigned int height=0;
image_info_t image_info;
int  verbose = 0;
int  upsidedown = 0;

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  extern void sig_handle(int);
  extern int  arg_parser(int, char **);
  int rtn;

  if ((signal(SIGINT, sig_handle)==SIG_ERR)
#ifndef WIN32
	  ||(signal(SIGQUIT, sig_handle)==SIG_ERR)
#endif
	  ) {
        fprintf(stderr, "Error: signal error\n");
        exit(1);
  }

  //----------------------------------------------------------------------------
  arg_parser(argc, argv);

  //----------------------------------------------------------------------------
  image_info.ImageWidth   = width;
  image_info.ImageHeight  = height;
  image_info.BitsPerPixel = 8;
  image_info.SkipPerLine  = (width&0x3) ? (4-(width&0x3)) : 0;
  image_info.BytesPerLine = width + image_info.SkipPerLine;
  image_info.DibSize      = 40;
  image_info.ClrSize      = 256*4; // 8-bit
  image_info.ImageSize    = image_info.BytesPerLine * height;
  image_info.pDibHdr =NULL; // DIB (device independent bitmap)
  image_info.pColor  =NULL; // 
  image_info.pBitMap =NULL; // pixels

  //----------------------------------------------------------------------------
  if (raw_read(fluma, &image_info, upsidedown)) {
      return -1;
  }

  //----------------------------------------------------------------------------
  bmp_write(foutput, &image_info, 0); // do not make upside-down twice

  //----------------------------------------------------------------------------
  bmp_wrapup(&image_info);

  return 0;
}

//------------------------------------------------------------------------------
// 1. get simulator options from command line
// 2. returns the argv index for the program options
#define XXTX\
	if ((i+1)>=argc) {\
	fprintf(stderr, "Error: need more for %s option\n", argv[i]);\
	exit(1);}
int arg_parser(int argc, char **argv) {
  int i;
  extern void help(int, char **);

  for (i=1; i<argc; i++) {
           if (!strcmp(argv[i], "-h")||!strcmp(argv[i], "-?")) {
	help(argc, argv);
	exit(0);
    } else if (!strcmp(argv[i], "-v")) {
	verbose = 1;
    } else if (!strcmp(argv[i], "-u")) {
	upsidedown = 1;
    } else if (!strcmp(argv[i], "-o")) { XXTX
        sprintf(foutput, "%s", argv[++i]);
    } else if (!strcmp(argv[i], "-y")) { XXTX
        sprintf(fluma, "%s", argv[++i]);
    } else if (!strcmp(argv[i], "-w")) { XXTX
        width = (unsigned int)strtoul(argv[++i], NULL, 0);
    } else if (!strcmp(argv[i], "-t")) { XXTX
        height = (unsigned int)strtoul(argv[++i], NULL, 0);
    } else if (argv[i][0]=='-') {
	fprintf(stderr, "undefined option: %s\n", argv[i]);
	help(argc, argv);
	exit(1);
    } else break;
  }
  return i;
}
#undef XXTX

//----------------------------------------------------------------------------
void
help(int argc, char **argv) {

  fprintf(stderr, "[Usage] %s [options]\n", argv[0]);
  fprintf(stderr, "\t-o  fname   output file (BMP gray)\n");
  fprintf(stderr, "\t-y  luma    raw file (LUMA/gray component)\n");
  fprintf(stderr, "\t-w  width   width (num of columns) in pixels\n");
  fprintf(stderr, "\t-t  height  height (num of rows) in pixels\n");
  fprintf(stderr, "\t-u          upside-donw\n");
  fprintf(stderr, "\t-v          verbos\n");
  fprintf(stderr, "\t-h          print help message\n");
}

//----------------------------------------------------------------------------
void
sig_handle(int sig) {
  switch (sig) {
  case SIGINT:
#ifndef WIN32
  case SIGQUIT:
#endif
       exit(0);
       break;
  }
}
//------------------------------------------------------------------------------
// Revision History
//
// 2017.12.16: Start by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
