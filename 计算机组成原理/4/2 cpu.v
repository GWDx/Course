module  cpu (clk, rst, io_addr, io_dout, io_we, io_din, m_rf_addr, rf_data, m_data, pc);
    input clk, rst;

    //IO_BUS
    output [7:0] io_addr;   //led和seg的地址
    output [31:0] io_dout;  //输出led和seg的数据
    output io_we;           //输出led和seg数据时的使能信号
    input [31:0] io_din;    //来自sw的输入数据

    wire io_addr10;
    assign io_addr10=aluResult[10];
    assign io_we=io_addr10 & memWrite;
    assign io_addr=aluResult[7:0];
    assign io_dout=regData2;

    //Debug_BUS
    input [7:0] m_rf_addr;  //存储器(MEM)或寄存器堆(RF)的调试读口地址
    output [31:0] rf_data;  //从RF读取的数据
    output [31:0] m_data;   //从MEM读取的数据
    output [31:0] pc;       //PC的内容

    assign rf_data = regData3;
    assign m_data = readData2;


    // PC
    wire [31:0] nextPC,pcAdd4;
    assign pcAdd4=pc+4;
    assign nextPC = PCsrc ? pc+(imme<<1): pc+4;


    // 指令存储器
    wire [31:0] instruction;
    wire [7:0] instructionReadAddress;
    assign instruction=pc[7:0];
    //                instructionMemory(        a,              spo);
    instructionMemory instructionMemory(instructionAddress, instruction);

    // 控制器
    reg branch, memRead, regSrc, memWrite, aluSrc, regWrite;
    reg [1:0] aluOp;
    // reg jal, branch, immeGen, regSrc, memWrite, aluSrc, regWrite;
    // reg [1:0] aluOp, regSrc;
    reg [7:0] controls;

    always @(*)
    begin
        {aluSrc, regSrc, regWrite, memRead, memWrite, branch, aluOp}= controls;
        case(instruction[6:0])
            7'b0110011: controls = 8'b00100010;  // R型
            7'b0000011: controls = 8'b11110000;  // ld
            7'b0100011: controls = 8'b10001000;  // sd
            7'b1100011: controls = 8'b00000101;  // beq
            default controls = 0;
        endcase
    end


    // 寄存器
    wire [4:0]regRead1, regRead2, regWrite, regRead3;
    wire [31:0]regData1, regData2, regData3;
    reg[31:0]regWriteData;

    assign regRead1 = instruction[19:15];
    assign regRead2 = instruction[24:20];
    assign regWrite = instruction[11:7];
    assign regRead3 = m_rf_addr[4:0];

    always @(*)
        case(regSrc)
            0:  regWriteData=aluResult;
            1:  regWriteData=io_addr10 ? io_din : memoryData1;
            2:  regWriteData=pcAdd4;
        endcase
    //regesterFile #(dataWidth, addressN)(clk,  ra0,     rd0,     ra1,      rd1,      wa,         wd,         we,       ra2,      rd2);
    regesterFile #(32, 5) regesterFile(clk, regRead1, regData1, regRead2, regData2, regWrite, regWriteData, regWrite, regRead3, regData3);


    // 立即数生成
    wire[31:0]imme;
    assign imme ={20{instruction[31]}, instruction[31:20]};


    // ALU
    wire zero;
    wire [31:0]aluIn1, aluIn2, aluResult;
    wire [2:0]aluControlResult;

    assign aluIn1 = regData1;
    assign aluIn2 = aluSrc ? imme : regData2;
    assign aluControlResult = ;
    //        alu(   a,      b,           f,            y,      z)
	alu #(32) alu(aluIn1, aluIn2, aluControlResult, aluResult, zero);


    // 数据存储器
    wire memWe;
    wire[7:0] address1, address2;
    wire[31:0] regWriteData, memoryData1, memoryData2;

    assign memWe=memWrite;
    assign address1 = aluResult;
    assign address2 = m_rf_addr;
    assign regWriteData = regData2;
    //         dataMemory(   a,        d,        dpra,   clk,  we,      spo,       dpo);
    dataMemory dataMemory(address1, regWriteData, address2, clk, memWe, memoryData1, memoryData2);




    always @(posedge clk)
    begin
        if(rst)
            pc <= 0;
        else
            pc <= nextPC;
    end

endmodule
