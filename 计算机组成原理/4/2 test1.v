module test1();
    reg clk, rst;
    wire [7:0] io_addr;   //led和seg的地址
    wire [31:0] io_dout;  //输出led和seg的数据
    wire io_we;           //输出led和seg数据时的使能信号
    reg [31:0] io_din;    //来自sw的输入数据

    reg [7:0] m_rf_addr;  //存储器(MEM)或寄存器堆(RF)的调试读口地址
    wire [31:0] rf_data;  //从RF读取的数据
    wire [31:0] m_data;   //从MEM读取的数据
    wire [31:0] pc;       //PC的内容
    cpu cpu(clk, rst, io_addr, io_dout, io_we, io_din, m_rf_addr, rf_data, m_data, pc);

    // clk
    initial clk = 0;
    always #5 clk = ~clk;

    // rst
    initial
    begin
        rst = 1;
        #20 rst = 0;
    end

    // io_din
    initial io_din = 1;

    // m_rf_addr
    // initial m_rf_addr = 240;
    // always #50
    //     m_rf_addr = m_rf_addr + 1;
    initial m_rf_addr = 8;
    always #5
        case(m_rf_addr)
            8:  m_rf_addr = 9;
            9:  m_rf_addr = 3;
            3:  m_rf_addr = 4;
            4:  m_rf_addr = 8;
        endcase

    initial #1000 $stop;
endmodule
