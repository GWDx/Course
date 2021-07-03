`timescale 1ns/1ps

module test21();
    reg [3:0] a;
    reg [7:0] d;
    reg clk,we;
    wire[7:0] spo;

    dist_mem_gen_0 dist_mem_gen_0 (a, d, clk, we, spo);
    // dist_mem_gen_0 (a, d, clk, we, spo);

    // clk
    initial clk = 0;
    always #5 clk = ~clk;

    // a
    initial a = 0;
    always #10 a = a + 1;

    // we
    initial
    begin
        we = 0;
        #50 we = 1;
        #50 we = 0;
    end

    // d
    initial d = $random;
    always #10 d = $random;

    initial #300 $stop;
endmodule
