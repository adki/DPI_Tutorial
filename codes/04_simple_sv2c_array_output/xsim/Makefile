SHELL=/bin/sh

DIR_C       = ../c
DIR_VERILOG = ../verilog
LIB_DPI     = mydpi

all:
	xsc -compile ${DIR_C}/function.c
	xsc -shared -o ${LIB_DPI}.so
	xelab -svlog ${DIR_VERILOG}/file.sv -sv_lib ${LIB_DPI}
	xsim top -t xsim_run.tcl

all_one:
	xsc -o ${LIB_DPI}.so ${DIR_C}/function.c
	xelab -nolog -sv -svlog ${DIR_VERILOG}/file.sv -sv_lib ${LIB_DPI}
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

cleanup clobber: clean
	/bin/rm   -f   dpi.so

cleanupall distclean: cleanup

