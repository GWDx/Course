`timescale 1ns/1ps

module test22();
    reg [3:0] addra;
    reg [7:0] dina;
    reg clka, wea, ena;
    wire[7:0] douta;

    blk_mem_gen_0 blk_mem_gen_0(clka, ena, wea, addra, dina, douta);
    // blk_mem_gen_0(clka, ena, wea, addra, dina, douta);

    // clka
    initial clka = 0;
    always #5 clka = ~clka;

    // addra
    initial addra = 0;
    always #10 addra = addra + 1;

    // wea
    initial
    begin
        wea = 0;
        #50 wea = 1;
        #50 wea = 0;
    end

    // dina
    initial dina = $random;
    always #10 dina = $random;

    // ena
    initial
    begin
        ena = 0;
        #10 ena = 1;
    end

    initial #300 $stop;
endmodule
