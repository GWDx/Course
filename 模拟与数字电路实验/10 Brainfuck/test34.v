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

module test34();
	reg clk=1,push=0;
	reg [7:0]sw=0;
	wire[3:0]data;
	wire[2:0]an;
	wire[7:0]led;
	
	reg [7:0]i=0;
	
	des3 des3(clk,push,sw,data,an,led);
	
	always #2 clk = ~clk;
	
	initial
	begin
		#2;
		repeat(200)
			#4 push = ~push;
		#2500 push=0;
		forever #4 push = ~push;
	end
	
	initial
	begin
		#4;
		repeat(100)
			#8 i=i+1;
		#2500 i=0;
		forever #8 i=i+1;
	end
	
	reg [1:54]code;
	initial
	begin
		code=54'o222602022113740404;
		#2500;
		code=54'o226022021137240404;
	end
	
	initial #4000 $stop;
	
	always #2
	begin
		if(i<18)
			sw[2:0]<={code[3*i+1],code[3*i+2],code[3*i+3]};
		if(i==18)
			sw[7]=1;
		if(i==99)
			sw[7]=0;
	end
endmodule