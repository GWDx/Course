`timescale 1ns/1ps

module alu #(parameter WIDTH = 32)(a, b, f, y, z);
    input [WIDTH-1:0] a, b;
    input [2:0] f;
    output reg[WIDTH-1:0] y;
    output z;

    assign z = f<=4;

    always @(*)
        case(f)
            0:  y = a+b;
            1:  y = a-b;
            2:  y = a&b;
            3:  y = a|b;
            4:  y = a^b;
            default:  y = 0;
        endcase
endmodule
