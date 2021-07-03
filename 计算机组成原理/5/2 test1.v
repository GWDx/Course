module test1();
    reg clk, rst;

    // IO_BUS
    wire [7:0] io_addr;   //led和seg的地址
    wire [31:0] io_dout;  //输出led和seg的数据
    wire io_we;           //输出led和seg数据时的使能信号
    reg [31:0] io_din;    //来自sw的输入数据

    // Debug_BUS
    reg [7:0] m_rf_addr;  //存储器(MEM)或寄存器堆(RF)的调试读口地址
    wire [31:0] rf_data;  //从RF读取的数据
    wire [31:0] m_data;   //从MEM读取的数据

    // PC/IF/ID 流水段寄存器
    wire [31:0] pc,pcd,ir,pcin;

    // ID/EX 流水段寄存器
    wire [31:0] pce,a,b,imm,ctrl;
    wire [4:0] rd;

    // EX/MEM 流水段寄存器
    wire [31:0] y,bm,ctrlm;
    wire [4:0] rdm;

    // MEM/WB 流水段寄存器
    wire [31:0] yw,mdr,ctrlw;
    wire [4:0] rdw;

    cpu_pl cpu_pl (clk, rst, io_addr, io_dout, io_we, io_din, m_rf_addr, rf_data, m_data,
        pc, pcd, ir, pcin, pce, a, b, imm, rd, ctrl, y, bm, rdm, ctrlm, yw, mdr, rdw, ctrlw);

    // clk
    initial clk = 0;
    always #5 clk = ~clk;

    // rst
    initial
    begin
        rst = 1;
        #60 rst = 0;
    end

    // io_din
    initial io_din = 1;

    // m_rf_addr
    initial m_rf_addr = 0;

    initial #3000 $stop;
endmodule
