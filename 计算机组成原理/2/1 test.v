`timescale 1ns/1ps

module test1();
    parameter dataN = 4,addressN = 3;
    reg [addressN-1 :0] wa, ra0, ra1, i; //写入的地址，读地址
    reg [dataN-1 : 0] wd; //写入的数据
    reg we,clk; //写使能
    wire[dataN-1 : 0] rd0,rd1; //读出的数据

    regesterFile #(dataN,addressN) regesterFile (clk,ra0,rd0,ra1,rd1,wa,wd,we);
    //regesterFile #(parameter dataN=4,addressN=3)(clk,ra0,rd0,ra1,rd1,wa,wd,we);

    // clk
    initial clk = 0;
    always #5 clk = ~clk;

    // we,wd,wa
    initial
    begin
        we=0;
        for(i=0;i<7;i=i+1)
        begin
            #10;
            we = 1;
            wa = i;
            wd = $random;
        end
        #10 we=0;
    end

    // ra0,ra1
    initial
    begin
        #100;
        for(i=0;i<7;i=i+1)
        begin
            ra0=i;
            ra1=6^i;
            #10;
        end
    end

	initial #250 $stop;
endmodule
