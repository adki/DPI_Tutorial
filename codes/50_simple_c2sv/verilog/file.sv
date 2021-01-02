module top;
    export "DPI-C"      function     mySVFunc;
    import "DPI-C" pure function int myCFunc(input int A, input int B);

    initial begin
        $display("%m %d", myCFunc(10, 20));
        $finish;
    end

    function int mySVFunc(input int A, input int B);
    begin
        $display("Hello from SystemVerilog! %d %d", A, B);
        return A+B;
    end
    endfunction
endmodule
