module top;
    import "DPI-C" function int  myCFunc1();
    import "DPI-C" function int  myCFunc2( input int A, output int B );
  //import "DPI-C" function real myCFunc3( input int A, input shortreal B, input real C );
    import "DPI-C" function real mySin( input real C);
    import "DPI-C" function real myCos( input real C);
    import "DPI-C" function real myTan( input real C);

    integer iA, iB, iC;
    shortreal fF;
    real dD, dE;
    initial begin
        iA = myCFunc1();
        $display("%m %d", iA);

        iC = myCFunc2(iA, iB);
        $display("%m %d %d %d", iA, iB, iC);

      //iA = 10;
      //fF = 10.1;
      //dD = 20.2;
      //dE = myCFunc3(iA, fF, dD);
      //$display("%m %d %f %f", iA, fF, dD, dE);

        dD = 3.1415/2.0;
        $display("%m sin:%f cos:%f tan:%f", mySin(dD), myCos(dD), myTan(dD));
        $finish;
    end
endmodule
