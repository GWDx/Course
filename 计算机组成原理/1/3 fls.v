`timescale 1ns/1ps

module fls(d, en, rst, clk, f);
    input [6:0] d;
    input en, rst, clk;
    output[6:0]f;

    reg [15:0] nextState=0;
    reg [6:0] a=0, b=0;
    reg [1:0] mode=0;
    wire[6:0] subtract;
    wire enPul, z;

    enPulse enPulse(clk, en, enPul);

    //       alu (a, b, f, y, z)
    alu #(7) alu1(a, b, 0, f, z);        // f = a+b
    alu #(7) alu2(d, a, 1, subtract, z); // subtract = d-a

    always @(posedge clk)
    begin
        if(rst)
            {mode, a, b} <= 0;
        else
            {mode, a, b} <= nextState;
    end


    always @(*)
    begin
        if(enPul)
            case (mode)
                0 : nextState = {2'd1, d, 7'b0};
                1 : nextState = {2'd2, a, subtract};
                2 : nextState = {2'd2, f, a};
                default : nextState = 0;
            endcase
        else
            nextState = {mode, a, b};
    end
endmodule


module main2(clk, sw, en, led);
    input clk, en;
    input [7:0] sw;
    output[7:0]led;

    assign led[7]=0;

    fls fls(sw[6:0], en, sw[7], clk, led[6:0]); //fls(d, en, rst, clk, f);
endmodule
