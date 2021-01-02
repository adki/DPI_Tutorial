#!/bin/bash

if [ -d obj           ]; then /bin/rm -fr  obj          ; fi
if [ -f bmp_extractor ]; then /bin/rm -f   bmp_extractor; fi
if [ -f bmp_gray      ]; then /bin/rm -f   bmp_gray     ; fi
if [ -f bmp_extractor.exe ]; then /bin/rm -f   bmp_extractor.exe; fi
if [ -f bmp_gray.exe      ]; then /bin/rm -f   bmp_gray.exe     ; fi
/bin/rm -f *.bmp
/bin/rm -f *.raw

