module top();
    typedef struct {
        byte       A;
        int        B;
        shortreal  C;
        real       D;
    } pkt_t;

    import "DPI-C" function int myFunc(inout pkt_t v);

    pkt_t  pkt;
    int ret;
    initial begin
       ret = myFunc(pkt);
       #1; $write("\n");
       if (ret!=0) $display("%m Failed ", ret);
       else begin
            $display("%m pkt.A = %c", pkt.A);
            $display("%m pkt.B = %d", pkt.B);
            $display("%m pkt.C = %f", pkt.C);
            $display("%m pkt.D = %f", pkt.D);
       end
    end
endmodule
