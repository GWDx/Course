`timescale 1ns/1ps

module test1();
	parameter WIDTH = 8;

    reg [WIDTH-1:0] a, b;
    reg [2:0] f;
    wire[WIDTH-1:0] y;
    wire z;

	alu #(WIDTH) alu(a, b, f, y, z);

	initial a = 2;
	always #10 a <= a + b;

	initial b = 1;
	always #10 b <= a * b;

	initial f = 0;
	always #10 f = f + 1;

	initial #200 $stop;
endmodule
