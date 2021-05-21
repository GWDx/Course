`timescale 100ps/1ps

/* 
module des3(clk,push,sw,data,an,led);
	input clk,push;
	input [7:0]sw;
	output[3:0]data;
	output[2:0]an;
	output[7:0]led;
endmodule
 */

module test32();
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
	
	reg [1:261]code=261'o333030300202002622226022260222203302221113713712227000403303404420222202224203306034117;
	
	initial #10000 $stop;
	
	always #2
	begin
		if(i<87)
			sw[2:0]<={code[3*i+1],code[3*i+2],code[3*i+3]};
		else
			#4 sw[7]<=1;
	end
endmodule