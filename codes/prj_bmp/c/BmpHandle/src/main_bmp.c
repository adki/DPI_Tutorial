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
char finput [128]="\0";
char foutput[128]="\0";
char fred   [128]="\0";
char fgreen [128]="\0";
char fblue  [128]="\0";
char fluma  [128]="\0";
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

  arg_parser(argc, argv);

  //----------------------------------------------------------------------------
  if (bmp_init(&image_info)) { return -1; }
  if (bmp_read(finput, &image_info, upsidedown)) { return -1; }

  //----------------------------------------------------------------------------
#if 0
  image_info_t ii;
  if (bmp_init(&ii)) { return -1; }
  bmp_gen_info( &ii, bmp_get_width(&image_info), bmp_get_height(&image_info));
  bmp_set_pixel(&ii, bmp_get_pixel(&image_info));
  bmp_write (foutput, &ii,          0);
  comp_write(fred,    &ii, "red"  , 0);
  comp_write(fgreen,  &ii, "green", 0);
  comp_write(fblue,   &ii, "blue" , 0);
  comp_write(fluma,   &ii, "luma" , 0);
#else
  bmp_write (foutput, &image_info,          0);
  comp_write(fred,    &image_info, "red"  , 0);
  comp_write(fgreen,  &image_info, "green", 0);
  comp_write(fblue,   &image_info, "blue" , 0);
  comp_write(fluma,   &image_info, "luma" , 0);
#endif

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
  extern int verbose;
  extern char finput[], foutput[];
  extern void help(int, char **);

  for (i=1; i<argc; i++) {
           if (!strcmp(argv[i], "-h")||!strcmp(argv[i], "-?")) {
	help(argc, argv);
	exit(0);
    } else if (!strcmp(argv[i], "-v")) {
	verbose = 1;
    } else if (!strcmp(argv[i], "-u")) {
	upsidedown = 1;
    } else if (!strcmp(argv[i], "-i")) { XXTX
        sprintf(finput, "%s", argv[++i]);
    } else if (!strcmp(argv[i], "-o")) { XXTX
        sprintf(foutput, "%s", argv[++i]);
    } else if (!strcmp(argv[i], "-r")) { XXTX
        sprintf(fred, "%s", argv[++i]);
    } else if (!strcmp(argv[i], "-g")) { XXTX
        sprintf(fgreen, "%s", argv[++i]);
    } else if (!strcmp(argv[i], "-b")) { XXTX
        sprintf(fblue, "%s", argv[++i]);
    } else if (!strcmp(argv[i], "-y")) { XXTX
        sprintf(fluma, "%s", argv[++i]);
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
  fprintf(stderr, "\t-i  fname   input  file (BMP)\n");
  fprintf(stderr, "\t-o  fname   output file (BMP)\n");
  fprintf(stderr, "\t-r  red     output file (RED component)\n");
  fprintf(stderr, "\t-g  green   output file (GREEN component)\n");
  fprintf(stderr, "\t-b  blue    output file (BLUE component)\n");
  fprintf(stderr, "\t-y  luma    output file (LUMA component)\n");
  fprintf(stderr, "\t-u          make output upside-down\n");
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
