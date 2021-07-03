`timescale 1ns/1ps

module enPulse(clk, en, enPul);
    input clk, en;
    output enPul;

    reg en1, en2;

    assign enPul = en1 && !en2;

    always @(posedge clk)
    begin
        en2 <= en1;
        en1 <= en;
    end
endmodule


module main(clk, sw, en, led);
    input clk, en;
    input [7:0] sw;
    output[7:0] led;

    reg [5:0] a, b;
    reg [2:0] f;
    wire[5:0] y;
    wire z, enPul;

    alu #(6) alu(a, b, f, y, z);
    enPulse enPulse(clk, en, enPul);

    assign led = {z, 2'b0, y};

    always @(posedge clk)
        if(enPul)
            case(sw[7:6])
                0:  f <= sw[2:0];
                1:  a <= sw[5:0];
                2:  b <= sw[5:0];

                default:
                begin
                    f <= 0;
                    a <= 0;
                    b <= 0;
                end
            endcase
endmodule
