`timescale 1ns/1ps

module test3();
    reg clk, rst, enq, deq;
    reg [3:0] in;
    wire full, empty;
    wire[3:0] out;
    wire[2:0] an;
    wire[3:0] seg;

    FIFO FIFO(clk, rst, enq, deq, in, full, empty, out, an, seg);

    // clk
    initial clk = 0;
    always #5 clk = ~clk;

    // rst
    initial
    begin
        rst = 1;
        #10 rst = 0;
    end

    // enq, in, deq
    initial
    begin
        enq = 0; deq = 0;
        #30 enq = 0; deq = 1; in = 1;   #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 2;   #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 3;   #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 4;   #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 5;   #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 6;   #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 7;   #20 enq = 0; deq = 0;
        #30 enq = 0; deq = 1; in = 8;   #20 enq = 0; deq = 0;
        #30 enq = 0; deq = 0; in = 9;   #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 10;  #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 11;  #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 12;  #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 13;  #20 enq = 0; deq = 0;
        #30 enq = 1; deq = 0; in = 14;  #20 enq = 0; deq = 0;
        #30 enq = 0; deq = 1; in = 15;  #20 enq = 0; deq = 0;
        #30 enq = 0; deq = 1; in = 0;   #20 enq = 0; deq = 0;
        #30 enq = 0; deq = 1; in = 1;   #20 enq = 0; deq = 0;
        #30 $stop;
    end

endmodule
