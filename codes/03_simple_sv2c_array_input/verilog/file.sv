module top();
    import "DPI-C" function int myFunc(input int v[]);

    int arr[4];
    int dynArr[];
    int ret;
    initial begin
        arr = {4, 5, 6, 7};
        ret = myFunc(arr);
        if (ret==4) $display("%m OK ", ret);
        else        $display("%m Failed ", ret);

        dynArr = new[6];
        dynArr = {8, 9, 10, 11, 12, 13};
        ret = myFunc(dynArr);
        if (ret==6) $display("%m OK ", ret);
        else        $display("%m Failed ", ret);

        arr = {127, 255, 255, -128};
        ret = myFunc(arr);
        if (ret==4) $display("%m OK ", ret);
        else        $display("%m Failed ", ret);
    end
endmodule
