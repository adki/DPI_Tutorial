#-------------------------------------------------------------
SHELL		= /bin/sh

#-------------------------------------------------------------
all: bmp_bmp bmp_gray

bmp_bmp:
	make -f Makefile.main_bmp

bmp_gray:
	make -f Makefile.main_gray

run_bmp_bmp:
	#./bmp_extractor -i images/face_320x240.bmp -o result.bmp
	./bmp_extractor -v -i images/face_240x240.bmp -o result.bmp

run_bmp_gray:
	./bmp_extractor -i images/face_320x240.bmp -r r.raw -g g.raw -b b.raw -y luma.raw
	./bmp_gray -w 320 -t 240 -y luma.raw -o result.bmp

#-------------------------------------------------------------
clean:
	make -f Makefile.main_bmp clean
	make -f Makefile.main_gray clean
	/bin/rm -f result.bmp

cleanup: clean
	make -f Makefile.main_bmp cleanup
	make -f Makefile.main_gray cleanup

clobber: cleanup
#*****************************************************************************
