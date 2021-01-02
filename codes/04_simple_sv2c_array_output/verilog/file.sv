module top();
    import "DPI-C" function int myFunc(output int v[]);

    int dynArr[];
    int ret, idx;
    initial begin
        dynArr = new[6];
        ret = myFunc(dynArr);
        if (ret!=6) $display("%m Failed ", ret);
        else begin
            for (idx=0; idx<ret; idx=idx+1)
                 $display("%2m [%4d]=%d", idx, dynArr[idx]);
        end
    end
endmodule
