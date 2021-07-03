module test2();
    reg clk, button;
    wire [7:0] sw;
    wire[2:0] an;
    wire[3:0] seg;
    wire[7:0] led;
    main main(clk, button, sw, an, seg, led);

    // clk
    initial clk = 0;
    always #5 clk = ~clk;

    // button
    always #25
    begin
        button = 1;
        #5 button = 0;
    end

    // sw
    reg [4:0] in;
    reg rst, valid, run;
    assign sw = {rst, run, valid, in};

    initial
    begin
        rst = 1;
        #100 rst = 0;
    end

    initial run = 0;

    initial
    begin
        valid = 1;  in = 1;
        #300;
        valid = 0;  in = 2;
    end

    initial #1000 $stop;
endmodule
