//------------------------------------------------------------------------------
// Copyright (c) 2020 by Ando Ki.
// All right reserved.
//------------------------------------------------------------------------------
// VERSION: 2020.07.30.
//------------------------------------------------------------------------------
// FILE: tester.v
//------------------------------------------------------------------------------
`timescale 1ns/1ps

module tester
     #(parameter BMP_INPUT_FILE="../images/lenna_240x240.bmp"
               , BMP_OUTPUT_FILE="result.bmp")
(
      input   wire  clk
    , input   wire  reset_n
);
    //-------------------------------------------------------------------------
    `include "bmp_handle.sv"
    //-------------------------------------------------------------------------
    image_info_t image_info;
    int upside_down;
    int width, height, channel;
    byte pixels[]; // dynamic array
    int line, pxl, dpt, loc;
    int ret;
    //-------------------------------------------------------------------------
    reg done=1'b0;
    initial begin
        wait (reset_n==1'b0);
        wait (reset_n==1'b1);
        repeat (10) @ (posedge clk);
        upside_down = 0;
        ret = dpi_bmp_verbose_set( 0 );
        ret = dpi_bmp_init( image_info ); // initializse image_info structure
        if (ret!=0) $display("%m error on dpi_bmp_init() %d", ret);

        //----------------------------------------------------------------------
        ret = dpi_bmp_read( BMP_INPUT_FILE, image_info, upside_down);
        if (ret!=0) $display("%m error on dpi_bmp_read() %d", ret);

        //----------------------------------------------------------------------
        $display("%m ImageWidth  ", image_info.ImageWidth  );
        $display("%m ImageHeight ", image_info.ImageHeight );
        $display("%m BitsPerPixel", image_info.BitsPerPixel);
        $display("%m BytesPerLine", image_info.BytesPerLine);
        $display("%m SkipPerLine ", image_info.SkipPerLine );
        $display("%m ImageSize   ", image_info.ImageSize   );

        //----------------------------------------------------------------------
        width  = image_info.ImageWidth;
        height = image_info.ImageHeight;
        channel= image_info.BitsPerPixel/8; // bytes per pixel
        pixels = new[width*height*channel]; // dynamic array

        //----------------------------------------------------------------------
        ret = dpi_bmp_get_pixels( pixels, image_info );
        if (ret!=(width*height*channel)) $display("%m error on dpi_bmp_get_pixels() %d", ret);

        //----------------------------------------------------------------------
        loc = 0;
        for (line=0; line<height; line=line+1) begin
            for (pxl=0; pxl<width; pxl=pxl+1) begin
                 for (dpt=0; dpt<channel; dpt=dpt+1) begin
                     pixels[loc] = 255 - pixels[loc];
                     loc = loc + 1;
                 end
            end
        end

        //----------------------------------------------------------------------
        ret = dpi_bmp_put_pixels( pixels, image_info );
        if (ret!=(width*height*channel)) $display("%m error on dpi_bmp_put_pixels() %d", ret);

        //----------------------------------------------------------------------
        ret = dpi_bmp_write( BMP_OUTPUT_FILE, image_info, upside_down );
        if (ret!=0) $display("%m error on dpi_bmp_write() %d", ret);

        //----------------------------------------------------------------------
        pixels.delete;

        //----------------------------------------------------------------------
        ret = dpi_bmp_wrapup( image_info );
        if (ret!=0) $display("%m error on dpi_bmp_wrapup() %d", ret);
        done = 1'b1;
    end
endmodule
//------------------------------------------------------------------------------
// Revision history
//
// 2020.07.30: Started by Ando Ki.
//------------------------------------------------------------------------------
