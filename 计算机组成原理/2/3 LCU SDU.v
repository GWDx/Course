`timescale 1ns/1ps

module LCU(clk, rst, enq, deq, rd0, in, out, full, empty, ra0, wa, wd, we, valid);
    input clk, rst, enq, deq;
    input [3:0] rd0, in;
    output full, empty;
    output reg we;
    output[2:0] ra0, wa;
    output reg[3:0] wd;
    output reg[3:0] out;
    output reg[7:0] valid;

    reg [2:0] head, tail;
    reg mode;
    reg [18:0]nextState;


    assign ra0 = head - 1;
    assign wa = tail - 1;

    always @(posedge clk)
    begin
        if(rst)
        begin
            we = 1;
            wd = 0; //使初始时输出为 0
        end else
        begin
            we = enq && !full;
            wd = in;
        end
    end

    assign full = &valid;
    assign empty = ~(|valid);


    always @(posedge clk)
    begin
        if(rst)
            {mode, head, tail, valid, out} <= 0;
        else
            {mode, head, tail, valid, out} <= nextState;
    end

    wire[7:0] deValid, enVaild;
    wire[2:0] nextHead, nextTail;
    assign deValid = valid & (~(1<<head));
    assign enVaild = valid | (1<<tail);
    assign nextHead = head + 1;
    assign nextTail = tail + 1;

    always @(*)
    begin
        if(mode)  //mode=1 表示在下一个时钟上升沿输出 rd0
            nextState = {1'b0, head, tail, valid, rd0};
        else if(deq && !empty)
            nextState = {1'b1, nextHead, tail, deValid, out};
        else if(enq && !full)
            nextState = {1'b0, head, nextTail, enVaild, out};
        else
            nextState = {1'b0, head, tail, valid, out};
    end

endmodule



module SDU(clk, rst, rd1, valid, ra1, an, seg);
    input clk, rst;
    input [3:0] rd1;
    input [7:0] valid;
    output[2:0] ra1;
    output reg [2:0] an;
    output[3:0] seg;

	reg [31:0] timeCount;

    assign seg = rd1;
    assign ra1 = an;

	always @(posedge clk)
	begin
        if(rst)
        begin
            timeCount <= 0;
            an <= 0;
        end
        else if(timeCount < 1000000)
        begin
		    timeCount <= timeCount + 1;
            if(!valid[an] && |valid)    // an 位无效，且队列非空
                an <= an + 1;
        end else
        begin
            timeCount <= 0;
            if(|valid)
                an <= an + 1;
        end
	end
endmodule
