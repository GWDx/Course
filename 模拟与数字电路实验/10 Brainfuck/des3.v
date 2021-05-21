`timescale 1ns/1ps

/* 
module dist_mem_gen_0(a,d,dpra,clk,we,dpo);
	input[7:0]a;	//写入的地址
	input[6:0]d;	//写入的数据
	input[7:0]dpra;	//读取的地址
	input clk,we;	//we=1 表示可写入
	output[6:0]dpo;	//读取的结果
endmodule
 */


module des3(clk,push,sw,data,an,led,rx,tx);
	input clk,push,rx,tx;
	input [7:0]sw;
	output reg[3:0]data;
	output[2:0]an;
	output[7:0]led;
	

	//rx,tx
	parameter rst=0;
    wire tx_ready,tx_rd,rx_vld;
    wire[7:0]tx_data,rx_data;

	// reg ansn1;
	reg[3:0]mark;

	rx rx0(clk,rst,rx,rx_vld,rx_data);
	// tx tx0(clk,rst,tx,tx_ready,tx_rd,tx_data);

	// assign tx_ready = |(ansn ^ ~ansn1);	//assign tx_ready = rx_vld;
	// assign tx_data = ans[ansn];

	// always@(posedge clk)
	// 	ansn1<=ans;
	
    always@(*)
        case(rx_data)
            62: mark=0;
            60: mark=1;
            43: mark=2;
            45: mark=3;
            46: mark=4;
            44: mark=5;
            91: mark=6;
            93: mark=7;
            10: mark=10;
            default: mark=15;
        endcase


    //RAM
	reg [7:0]a=0,dpra=0;
	reg [6:0]d;
	reg we=0;
	wire[6:0]dpo;

	dist_mem_gen_0 dist_mem_gen_0(a,d,dpra,clk,we,dpo);
	
	
	//输入
	reg [7:0]coden=0;
	reg [449:0]codes;
	reg push1,push2;
	wire pushpul;
	reg [2:0]mode=0;  
	//	0: code
	//	1: run
	//	2: read
	//	3: stop
	//	4: rst
	
	always@(posedge clk)
	begin
		push1<=push;
		push2<=push1;
	end
	
	assign pushpul = push1 && !push2;
	

	//过程 part1
    reg [7:0]now=0;
	wire[7:0]next;
    wire[2:0]codenow,codenext;
    reg [1:0]work=1;
	reg [3:0]pass=0,back=0;
	reg [7:0]error=0;
	reg [3:0]ansn=0;
    reg [6:0]ans [0:15];
	reg sw71,sw72;

    assign next = back ? now-1 : now+1;
    assign codenow = {codes[3*now+2], codes[3*now+1], codes[3*now]};
	assign codenext= {codes[3*next+2],codes[3*next+1],codes[3*next]};

	always@(posedge clk)
	begin
		sw71<=sw[7];
		sw72<=sw71;
	end


	//输出
	reg [3:0]outn=0;
	wire[6:0]out;
	reg [31:0]cnt=0;
	wire[2:0]an0;

	assign out = ans[outn];
	assign an0 = cnt/1000000 % 3;
	assign an  = an0==2 ? 7 : an0;
	assign led = error;
	
	always@(posedge clk)
		cnt <= cnt+1;
	
	always@(*)
		case(an0)
			0:data=out % 16;
			1:data=out / 16;
			2:data=outn;
			default:data=0; 
		endcase
	

	//过程 part2
	always@(posedge clk)
		case (mode)
			0:	//code
			if(rx_vld)
			begin
				if(mark<=7)
				begin
					{codes[3*coden+2],codes[3*coden+1],codes[3*coden]}<=mark[2:0];
					coden<=coden+1;
				end
				else mode<=1;
			end

			1:	//run
			begin
				if(sw[7]==0)
					mode<=3;
				else

				if(pass)
				begin
					now<=now+1;
					if(codenext==6) pass<=pass+1;
					if(codenext==7) pass<=pass-1;
				end else

				if(back)
				begin
					now<=now-1;
					if(codenext==6) back<=back-1;
					if(codenext==7) back<=back+1;
				end else
				
				case (work)
					0:	
					begin
						now<=now+1;
						if(next>=coden)
						begin
							error[7]<=1;
							mode<=3;
						end else
						work<=1;
					end

					1:
					case(codenow)
						0:begin	a<=a+1;		dpra<=dpra+1;	work<=0;	end
						1:begin	a<=a-1;		dpra<=dpra-1;	work<=0;	end
						2:begin	we<=1;		d<=dpo+1;	work<=2;	end
						3:begin	we<=1;		d<=dpo-1;	work<=2;	end
						4:begin	ans[ansn]<=dpo;		ansn<=ansn+1;	work<=0;	end
						5:begin we<=1;	mode<=2;	work<=2;	end
						6:begin if(!dpo)	pass<=1;	work<=0;	end
						7:begin	if(dpo)		back<=1;	work<=0;	end
						default:error[0]<=1;
					endcase
					
					2:
					begin
						work<=0;
						we<=0;
					end

					default:error[1]<=1;
				endcase
			end

			2:	//read
			if(rx_vld)
			begin
				if(rx_data==10)
					mode<=1;
				else
					d<=rx_data[6:0];
			end

			3:	//stop
			begin
				if(pushpul)
					outn<=(outn+1)%ansn;
				if(sw[7]==0)
				begin
					coden<=0;
					now<=0;
					work<=1;
					pass<=0;
					back<=0;
					ansn<=0;
					error<=0;
					outn<=0;

					a<=0;
					dpra<=0;
					d<=0;
					we<=1;
					mode<=4;
				end
			end

			4:	//rst
			begin
				if(a==1)
				begin
					we<=0;
					mode<=0;
				end
				a<=a-1;
			end

			default:error[4]<=1;
		endcase
	
endmodule