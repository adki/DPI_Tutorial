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
     #(parameter PNG_INPUT_FILE="../images/lenna_240x240.png"
               , PNG_OUTPUT_FILE="result.png")
(
      input   wire  clk
    , input   wire  reset_n
);
    //-------------------------------------------------------------------------
    `include "png_handle.sv"
    //-------------------------------------------------------------------------
    image_info_t image_info;
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
        ret = dpi_png_verbose_set( 0 );
        ret = dpi_png_init( image_info ); // initializse image_info structure
        if (ret!=0) $display("%m error on dpi_png_init() %d", ret);

        //----------------------------------------------------------------------
        ret = dpi_png_read( PNG_INPUT_FILE, image_info);
        if (ret!=0) $display("%m error on dpi_png_read() %d", ret);

        //----------------------------------------------------------------------
        $display("%m ImageWidth   ", image_info.ImageWidth  );
        $display("%m ImageHeight  ", image_info.ImageHeight );
        $display("%m BytesPerPixel", image_info.BytesPerPixel);
        $display("%m ImageSize    ", image_info.ImageSize   );

        //----------------------------------------------------------------------
        width  = image_info.ImageWidth;
        height = image_info.ImageHeight;
        channel= image_info.BytesPerPixel; // bytes per pixel
        pixels = new[width*height*channel]; // dynamic array

        //----------------------------------------------------------------------
        ret = dpi_png_get_pixels( pixels, image_info );
        if (ret!=(width*height*channel)) $display("%m error on dpi_png_get_pixels() %d", ret);

        //----------------------------------------------------------------------
        if ((image_info.BytesPerPixel!=3)&&(image_info.BytesPerPixel!=1)) begin
             $display("%m WARNING the picture contains alpha channel and may not work.");
        end
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
        ret = dpi_png_put_pixels( pixels, image_info );
        if (ret!=(width*height*channel)) $display("%m error on dpi_png_put_pixels() %d", ret);

        //----------------------------------------------------------------------
        ret = dpi_png_write( PNG_OUTPUT_FILE, image_info);
        if (ret!=0) $display("%m error on dpi_png_write() %d", ret);

        //----------------------------------------------------------------------
        pixels.delete;

        //----------------------------------------------------------------------
        ret = dpi_png_wrapup( image_info );
        if (ret!=0) $display("%m error on dpi_png_wrapup() %d", ret);
        done = 1'b1;
    end
endmodule
//------------------------------------------------------------------------------
// Revision history
//
// 2020.07.30: Started by Ando Ki.
//------------------------------------------------------------------------------
