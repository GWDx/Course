`timescale 1ns/1ps

/* 
module des3(clk,push,sw,data,an,led);
	input clk,push;
	input [7:0]sw;
	output[3:0]data;
	output[2:0]an;
	output[7:0]led;
endmodule
 */

module test33();
	reg clk=1,push=0;
	reg [7:0]sw=0;
	wire[3:0]data;
	wire[2:0]an;
	wire[7:0]led;
	
	reg [15:0]i=0;
	
	des3 des3(clk,push,sw,data,an,led);
	
	always #2 clk = ~clk;
	
	initial
	begin
		#2;
		forever #4 push = ~push;
	end
	
	initial
	begin
		#4;
		forever #8 i=i+1;
	end
	
	reg [1:141]code=141'o53333333333633333333333333333333334533333333337;
	
	initial #1000 $stop;
	
	always #2
	begin
		if(i<47)
			sw[2:0]<={code[3*i+1],code[3*i+2],code[3*i+3]};
		else
			#4 sw[7]<=1;
		if(47<=i && i<57)
			sw[6:0]<=7'h61;
		if(57<=i && i<107)
			sw[6:0]<=7'h72;
		if(107<=i && i<157)
			sw[6:0]<=7'h0A;
	end
endmodule