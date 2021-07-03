`timescale 1ns/1ps

module regesterFile #(parameter dataWidth = 4, addressN = 3)(clk, rst, ra0, rd0, ra1, rd1, wa, wd, we, ra2, rd2);
    input clk, rst, we; //写使能
    input [addressN-1 :0] wa, ra0, ra1, ra2; //写入的地址，读地址
    input [dataWidth-1 : 0] wd; //写入的数据
    output[dataWidth-1 : 0] rd0, rd1, rd2; //读出的数据

    reg [dataWidth-1 : 0] regfile[0 : 2**addressN-1];

    //读出数据
    assign rd0 = regfile[ra0];
    assign rd1 = regfile[ra1];
    assign rd2 = regfile[ra2];

    //写入数据
    always @(posedge clk or posedge rst)
    begin
        if(rst)
            regfile[0] <= 0;
        else
            if(we)
                regfile[wa] <= wd;
    end
endmodule
