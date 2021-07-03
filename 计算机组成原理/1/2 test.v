`timescale 1ns/1ps

module test2();
    reg clk, en;
    wire[7:0] sw, led;
    reg [7:6] sel;
    reg [5:0] data;

    main main(clk, sw, en, led);
    assign sw={sel, data};

    initial clk = 0;
    always #5 clk = ~clk;

	initial sel = 2;
    always #20 sel = sel==2 ? 0 : sel+1;

    initial data = 2;
    always #20 data = $random;

	initial en = 0;
    always #20 en = ~en;

	initial #250 $stop;
endmodule
