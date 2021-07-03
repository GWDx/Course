`timescale 1ns/1ps


module FIFO(clk, rst, enq, deq, in, full, empty, out, an, seg);
    input clk, rst, enq, deq;
    input [3:0] in;
    output full, empty;
    output[3:0] out;
    output[2:0] an;
    output[3:0] seg;

    wire [3:0] rd0;
    wire we;
    wire [2:0] ra0, wa;
    wire [3:0] wd;
    wire [7:0] valid;

    wire [3:0] rd1;
    wire [2:0] ra1;

    //  LCU(clk, rst,    enq,      deq,   rd0, in, out, full, empty, ra0, wa, wd, we, valid);
    LCU LCU(clk, rst, enqPulse, deqPulse, rd0, in, out, full, empty, ra0, wa, wd, we, valid);

    //                  regesterFile(clk,ra0,rd0,ra1,rd1,wa,wd,we);
    regesterFile #(4,3) regesterFile(clk,ra0,rd0,ra1,rd1,wa,wd,we);

    //  SDU(clk, rst, rd1, valid, ra1, an, seg);
    SDU SDU(clk, rst, rd1, valid, ra1, an, seg);


    wire enqPulse, deqPulse;

    //      enPulse (clk, en, enp);
    enPulse enPulse1(clk, enq, enqPulse);
    enPulse enPulse2(clk, deq, deqPulse);

endmodule


module main(clk,sw,button,led,an,seg);
    input clk,button;
    input [7:0] sw;
    output[7:0] led;
    output[2:0] an;
    output[3:0] seg;

    assign led[5:4]=0;

    FIFO FIFO(clk, button, sw[7], sw[6], sw[3:0], led[7], led[6], led[3:0], an, seg);
    //   FIFO(clk,   rst,   enq,   deq,     in,    full,  empty,    out,    an, seg)

endmodule


module enPulse(clk,en,enp);
    input clk,en;
    output enp;

    reg en1, en2, en3;

    assign enp = en2 && !en3;

    always @(posedge clk)
    begin
        en3 <= en2;
        en2 <= en1;
        en1 <= en;
    end
endmodule
