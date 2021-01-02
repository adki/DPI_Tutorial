#!/bin/csh -f

if ( -e obj           ) /bin/rm -fr  obj
if ( -e bmp_extractor ) /bin/rm -f   bmp_extractor
if ( -e bmp_gray      ) /bin/rm -f   bmp_gray
if ( -e bmp_extractor.exe ) /bin/rm -f   bmp_extractor.exe
if ( -e bmp_gray.exe      ) /bin/rm -f   bmp_gray.exe
/bin/rm -f *.bmp
/bin/rm -f *.raw
