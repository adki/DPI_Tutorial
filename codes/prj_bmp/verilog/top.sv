//------------------------------------------------------------------------------
// Copyright (c) 2020 by Ando Ki.
// All right reserved.
//
// http://www.future-ds.com
// adki@future-ds.com
//------------------------------------------------------------------------------
`include "sim_define.v"

`ifndef BMP_INPUT_FILE
`define BMP_INPUT_FILE   "../images/lenna_240x240.bmp"
`endif
`ifndef BMP_OUTPUT_FILE
`define BMP_OUTPUT_FILE   "result.bmp"
`endif

`include "tester.sv"

`timescale 1ns/1ns

module top ;
    //--------------------------------------------------------------------------
    reg  clk     =1'b0;
    reg  reset_n =1'b0;
    //--------------------------------------------------------------------------
    tester #(.BMP_INPUT_FILE  (`BMP_INPUT_FILE  )
            ,.BMP_OUTPUT_FILE (`BMP_OUTPUT_FILE ))
    u_tester (
          .clk     ( clk     )
        , .reset_n ( reset_n )
    );
    //--------------------------------------------------------------------------
    always #5 clk <= ~clk;
    initial begin
        reset_n = 1'b0;
        repeat (10) @ (posedge clk);
        reset_n = 1'b1;
        repeat (20) @ (posedge clk);
        reset_n = 1'b1;
        repeat (10) @ (posedge clk);
        wait (u_tester.done==1'b1);
        repeat (10) @ (posedge clk);
        $finish(2);
    end
    //--------------------------------------------------------------------------
`ifdef VCD
    initial begin
        $dumpfile("wave.vcd"); //$dumplimit(1000000);
        $dumpvars(0);
    end
`endif
endmodule
