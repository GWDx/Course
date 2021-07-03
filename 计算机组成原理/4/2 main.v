module main(clk, button, sw, an, seg, led);
    input clk, button;
    input [7:0] sw;
    output[2:0] an;
    output[3:0] seg;
    output[7:0] led;

    // input
    wire [4:0] in;
    wire rst, run, valid, step;

    assign rst = sw[7];
    assign run = sw[6];
    assign valid = sw[5];
    assign in = sw[4:0];
    assign step = button;

    //output
    wire [1:0] check;
    wire [4:0] out0;
    wire ready;

    assign led[7]= ready;
    assign led[6:5]= check;
    assign led[4:0]= out0;


    wire clk_cpu;

    //IO_BUS
    wire [7:0] io_addr;
    wire [31:0] io_dout;
    wire io_we;
    wire [31:0] io_din;

    //Debug_BUS
    wire [7:0] m_rf_addr;
    wire [31:0] rf_data;
    wire [31:0] m_data;
    wire [31:0] pc;

    wire rst_pdu;
    enPulse enPulse(clk, rst, rst_pdu);

    //         pdu_1cycle(clk,   rst,   run, step, clk_cpu, valid, in, check, out0, an, seg, ready, io_addr, io_dout, io_we, io_din, m_rf_addr, rf_data, m_data, pc);
    pdu_1cycle pdu_1cycle(clk, rst_pdu, run, step, clk_cpu, valid, in, check, out0, an, seg, ready, io_addr, io_dout, io_we, io_din, m_rf_addr, rf_data, m_data, pc);

    //  cpu(  clk,   rst, io_addr, io_dout, io_we, io_din, m_rf_addr, rf_data, m_data, pc);
    cpu cpu(clk_cpu, rst, io_addr, io_dout, io_we, io_din, m_rf_addr, rf_data, m_data, pc);

endmodule



module enPulse(clk, en, enp);
    input clk, en;
    output enp;

    reg en1, en2;

    assign enp = en1 && !en2;

    always @(posedge clk)
    begin
        en2 <= en1;
        en1 <= en;
    end
endmodule
