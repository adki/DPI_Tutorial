SHELL=/bin/sh

DIR_C       = ../c
DIR_PNG_LIB = ../c
DIR_VERILOG = ../verilog
LIB_DPI     = mydpi

PNG_INPUT  ?= ${DIR_IMAGES}/face_240x240.png
PNG_OUTPUT ?= result.png

all: lib sim

lib:
	xsc -compile ${DIR_C}/dpi_png_wrapper.c --additional_option "-I${DIR_PNG_LIB} -I${DIR_C}"
	xsc -shared -o ${LIB_DPI}.so

sim:
	xelab -sv_lib ${LIB_DPI} -svlog ${DIR_VERILOG}/top.sv
	xsim top -t xsim_run.tcl

all_one:
	xsc -o ${LIB_DPI}.so ${DIR_C}/dpi_png_wrapper.c
	xelab -nolog -sv -svlog ${DIR_VERILOG}/top.sv -sv_lib ${LIB_DPI}
	xsim -nolog work.top -runall

clean:
	/bin/rm  -f   ${LIB_DPI}.so
	/bin/rm  -f   xelab.log
	/bin/rm  -f   xelab.pb
	/bin/rm  -f   xsc.log
	/bin/rm  -f   xsc.pb
	/bin/rm  -fr  xsim.dir
	/bin/rm  -f   webtalk_*.backup.jou
	/bin/rm  -f   webtalk_*.backup.log
	/bin/rm  -f   webtalk.jou
	/bin/rm  -f   webtalk.log
	/bin/rm  -f   xsim_*.backup.jou
	/bin/rm  -f   xsim_*.backup.log
	/bin/rm  -f   xsim.jou
	/bin/rm  -f   xsim.log
	/bin/rm  -fr  .Xil
	/bin/rm  -f   x.png ${PNG_OUTPUT}

cleanup clobber: clean
	/bin/rm   -f   dpi.so

cleanupall distclean: cleanup

