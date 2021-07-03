`timescale 1ns/1ps

module test3();
    reg clk, en;
    wire[7:0] sw, led;

    reg [6:0] data;
    reg rst;

    assign sw={rst, data};
    main2 main2(clk, sw, en, led); //des2(clk, sw, en, led);

    initial clk = 0;
    always #5 clk = ~clk;

    initial
    begin
        rst=1;
        #10 rst=0;
	end

    initial en = 0;
    always #20 en = ~en;

    initial data = 2;
    always #40 data = data + 1;

	initial #250 $stop;
endmodule
