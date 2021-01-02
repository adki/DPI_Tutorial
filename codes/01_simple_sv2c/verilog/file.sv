module top;
    import "DPI-C" function void myCFunction();

    initial begin
        myCFunction();
        $finish;
    end
endmodule
