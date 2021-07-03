module cpu(clk, rst, io_addr, io_dout, io_we, io_din, m_rf_addr, rf_data, m_data, pc);
    input clk, rst;

    //IO_BUS
    output [7:0] io_addr;   //led和seg的地址
    output [31:0] io_dout;  //输出led和seg的数据
    output io_we;           //输出led和seg数据时的使能信号
    input [31:0] io_din;    //来自sw的输入数据

    wire ioAddr10;
    assign ioAddr10 = aluResult[10];
    assign io_we = ioAddr10 & memWrite;
    assign io_addr = aluResult[7:0];
    assign io_dout = regData2;


    //Debug_BUS
    input [7:0] m_rf_addr;  //存储器(MEM)或寄存器堆(RF)的调试读口地址
    output [31:0] rf_data;  //从RF读取的数据
    output [31:0] m_data;   //从MEM读取的数据
    output [31:0] pc;       //PC的内容

    assign rf_data = regData3;
    assign m_data = memoryData2;


    // PC
    wire pcSrc;
    wire [31:0] nextPCAddress, pcAdd4;
    reg  [31:0] PCAddress;
    assign pc = PCAddress;
    assign pcSrc = jal | (branch & zero);
    assign pcAdd4 = PCAddress+4;
    assign nextPCAddress = pcSrc ? PCAddress+(imme<<1) : pcAdd4;


    // 指令存储器
    wire [31:0] instruction;
    wire [7:0] instructionAddress;
    assign instructionAddress = PCAddress[9:2];
    //                instructionMemory(        a,              spo);
    instructionMemory instructionMemory(instructionAddress, instruction);


    // 控制器
    wire [6:0] opcode;
    wire jal, branch, immeGen, memWrite, aluSrc, regWrite;
    wire [1:0] aluOp, regSrc;
    reg [9:0] controls;

    assign opcode = instruction[6:0];
    assign {jal, aluSrc, regSrc, regWrite, memWrite, branch, aluOp, immeGen}= controls;
    always @(*)
        case(opcode)
            7'b0000011: controls = 10'b0101100001;  // ld
            7'b0100011: controls = 10'b0100010001;  // sd
            7'b0110011: controls = 10'b0000100100;  // add
            7'b0010011: controls = 10'b0100100101;  // addi
            7'b1100011: controls = 10'b0000001011;  // beq
            7'b1101111: controls = 10'b1010100111;  // jal
            default controls = 0;
        endcase


    // 寄存器
    wire [4:0] regRead1, regRead2, regRead3, regWriteAddress;
    wire [31:0] regData1, regData2, regData3;
    reg  [31:0] regWriteData;

    assign regRead1 = instruction[19:15];
    assign regRead2 = instruction[24:20];
    assign regWriteAddress = instruction[11:7];
    assign regRead3 = m_rf_addr[4:0];

    always @(*)
        case(regSrc)
            0:  regWriteData = aluResult;
            1:  regWriteData = ioAddr10 ? io_din : memoryData1;
            2:  regWriteData = pcAdd4;
        endcase
    //regesterFile #(dataWidth, addressN)(clk, rst,  ra0,     rd0,     ra1,      rd1,           wa,           wd,         we,       ra2,      rd2);
    regesterFile #(32, 5) regesterFile(clk, rst, regRead1, regData1, regRead2, regData2, regWriteAddress, regWriteData, regWrite, regRead3, regData3);


    // 立即数生成
    reg [31:0] imme;
    always @(*)
        case(opcode)
            7'b0000011: // ld
                imme = {{20{instruction[31]}}, instruction[31:20]};

            7'b0100011: // sd
                imme = {{20{instruction[31]}}, instruction[31:25], instruction[11:7]};

            7'b0010011: // addi
                imme = {{20{instruction[31]}}, instruction[31:20]};

            7'b1100011: //beq
            begin
                imme[31:12] = {20{instruction[31]}};
                {imme[11], imme[9:4]}= instruction[31:25];
                {imme[3:0], imme[10]}= instruction[11:7];
            end

            7'b1101111: // jal
            begin
                imme[31:20] = {12{instruction[31]}};
                {imme[19], imme[9:0], imme[10], imme[18:11]} = instruction[31:12];
            end

            default:
                imme = 0;
        endcase

    // ALU Control
    wire funct7;
    reg [3:0] aluControlResult;

    assign funct7 = instruction[31:25];
    always @(*)
        case(aluOp)
            0:  aluControlResult = 2;   // ld, sd, addi
            1:  aluControlResult = 6;   // beq
            2:  //R 型
            case(funct7)
                7'b0000000: aluControlResult = 2;    // add
                7'b0100000: aluControlResult = 6;
                default:    aluControlResult = 2;
            endcase

            default:    aluControlResult = 15;  // jal
        endcase


    // ALU
    wire zero;
    wire [31:0] aluIn1, aluIn2, aluResult;

    assign aluIn1 = regData1;
    assign aluIn2 = aluSrc ? imme : regData2;
    //        alu(   a,      b,           f,            y,      z)
	alu #(32) alu(aluIn1, aluIn2, aluControlResult, aluResult, zero);


    // 数据存储器
    wire memWE;
    wire [7:0] memAddress1, memAddress2;
    wire [31:0] memoryWriteData, memoryData1, memoryData2;

    assign memWE = memWrite & !ioAddr10;
    assign memAddress1 = aluResult[9:2];
    assign memAddress2 = m_rf_addr;
    assign memoryWriteData = regData2;
    //         dataMemory(    a,              d,           dpra,     clk,  we,       spo,        dpo);
    dataMemory dataMemory(memAddress1, memoryWriteData, memAddress2, clk, memWE, memoryData1, memoryData2);


    always @(posedge clk or posedge rst)
    begin
        if(rst)
            PCAddress <= 32'h0000_0000;
        else
            PCAddress <= nextPCAddress;
    end

endmodule
