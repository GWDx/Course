module cpu_pl (clk, rst, io_addr, io_dout, io_we, io_din, m_rf_addr, rf_data, m_data,
    pc, pcd, ir, pcin, pce, a, b, imm, rd, ctrl, y, bm, rdm, ctrlm, yw, mdr, rdw, ctrlw);

    input clk, rst;

    // IO_BUS
    output [7:0] io_addr;      //led和seg的地址
    output [31:0] io_dout;     //输出led和seg的数据
    output io_we;                 //输出led和seg数据时的使能信号
    input [31:0] io_din;        //来自sw的输入数据

    reg io_din5;
    wire ioAddress4;
    assign ioAddress4 = Y[10];
    assign io_we = ioAddress4 & memWrite4;
    assign io_addr = Y[7:0];
    assign io_dout = BM;

    always@(posedge clk)
        io_din5 <= io_din;


    // Debug_BUS
    input [7:0] m_rf_addr;   //存储器(MEM)或寄存器堆(RF)的调试读口地址
    output [31:0] rf_data;    //从RF读取的数据
    output [31:0] m_data;    //从MEM读取的数据

    assign rf_data = regData3;
    assign m_data = memoryData2;


    // PC/IF/ID 流水段寄存器
    output [31:0] pc, pcd, ir, pcin;

    assign pc = PCAddress;
    assign pcd = Instruction;
    assign ir = IR;
    assign pcin = nextPCAddress;


    // ID/EX 流水段寄存器
    output [31:0] pce, a, b, imm, ctrl;
    output [4:0] rd;

    assign pce = PCE;
    assign a = A;
    assign b = B;
    assign imm = Imm;
    assign ctrl ={
        fStall, dStall, 1'b0, eFlush,
        2'b0, forward1,
        2'b0, forward2,
        1'b0, regWrite3, regSrc3,
        2'b0, 1'b1, memWrite3,
        2'b0, jal3, branch3,
        6'b0, aluOp3};
    assign rd = Rd;


    // EX/MEM 流水段寄存器
    output [31:0] y, bm, ctrlm;
    output [4:0] rdm;

    assign y = Y;
    assign bm = BM;
    assign ctrlm ={
        fStall, dStall, 1'b0, eFlush,
        2'b0, forward1,
        2'b0, forward2,
        1'b0, regWrite4, regSrc4,
        2'b0, 1'b1, memWrite4,
        4'b0,
        8'b0};
    assign rdm = RdM;


    // MEM/WB 流水段寄存器
    output [31:0] yw, mdr, ctrlw;
    output [4:0] rdw;

    assign yw = YW;
    assign mdr = MDR;
    assign ctrlw ={
        fStall, dStall, 1'b0, eFlush,
        2'b0, forward1,
        2'b0, forward2,
        1'b0, regWrite5, regSrc5,
        4'b0,
        4'b0,
        8'b0};
    assign rdw = RdW;


    parameter NOP = 32'h0000_0013;

    // hazard
    wire eFlush, fStall, dStall, runAll1;
    assign runAll1 = & run[4:0];
    hazardDetection hazardDetection(clk, runAll1, IR, IR3, eFlush, fStall, dStall, prevJump);
    dataForward dataForward(IR3, RdM, RdW, regWrite4, regWrite5, forward1, forward2);

    // PC
    wire pcSrc3, prevJump;
    wire [31:0] nextPCAddress, jalAddress3;
    reg [31:0] PCAddress;

    assign nextPCAddress = prevJump ? pcSrc3 ? jalAddress3 : PCAddress : PCAddress + 4;

    reg [5:0]run;
    always @(posedge clk)
        run <= rst ? 0 : run*2+1;

    always @(posedge clk or posedge rst)
    begin
        if(rst)
            PCAddress <= 0;
        else
            if(!fStall && runAll1)
                PCAddress <= nextPCAddress;
    end

    wire [6:0] opcode;
    assign opcode = IR[6:0];


    // 指令存储
    wire [31:0] Instruction;
    wire [7:0] IRAddress;
    assign IRAddress = PCAddress[9:2];
    instructionMemory instructionMemory(IRAddress, Instruction);
    //                instructionMemory(   a,     spo);


    // IF-ID
    wire[31:0] pcAdd43;
    reg [31:0] PCD, PCE, pcAdd44, pcAdd45;
    reg [31:0] IR, IR3;
    always @(posedge clk)
        if(run[0])
            if(!dStall)
            begin
                PCD <= PCAddress;
                IR <= !runAll1 ? NOP : Instruction;
            end

    // 控制器
    wire jal2, jal3, branch2, branch3, memWrite3, memWrite2, aluSrc2, regWrite2, regWrite3, regWrite4;
    wire jal4, branch4, memWrite4, aluSrc3, regWrite5;
    wire [1:0] aluOp2, regSrc2, aluOp3, regSrc5, regSrc3, regSrc4;
    wire [8:0] controls;

    assign {regSrc3, regWrite3}= WB3;
    assign {regSrc4, regWrite4}= WB4;

    assign {jal2, aluSrc2, regSrc2, regWrite2, memWrite2, branch2, aluOp2} = controls;
    controlUnit controlUnit(eFlush ? NOP : IR, controls);

    // 寄存器
    wire regWE;
    wire [4:0] regRead1, regRead2, regRead3, regWriteAddress;
    wire [31:0] regData1, regData2, regData3;
    reg  [31:0] regWriteData;

    assign regWE = regWrite5 && run[5];
    assign regRead1 = IR[19:15];
    assign regRead2 = IR[24:20];
    assign regWriteAddress = RdW;
    assign regRead3 = m_rf_addr[4:0];

    always @(*)
        case(regSrc5)
            0:  regWriteData = YW;
            1:  regWriteData = ioAddress5 ? io_din5 : MDR;
            2:  regWriteData = pcAdd45;
            default:  regWriteData = YW;
        endcase
    regesterFile #(32, 5) regesterFile(clk, rst, regRead1, regData1, regRead2, regData2, regWriteAddress, regWriteData, regWE, regRead3, regData3);
    //regesterFile #(dataWidth, addressN)(clk, rst,  ra0,     rd0,     ra1,      rd1,           wa,           wd,         we,    ra2,      rd2);

    // 立即数生成
    wire [31:0] imme;
    immeGen immeGen(IR, imme);


    // ID-EX
    wire[2:0] EX2, M2, WB2;
    reg [2:0] EX3, M3, WB3, M4, WB4, WB5;
    reg [4:0] Rd, RdM, RdW;
    reg [31:0] A, B, Imm;


    assign EX2 = {aluOp2, aluSrc2};
    assign M2 = {memWrite2, branch2, jal2};
    assign WB2 = {regSrc2, regWrite2};

    always @(posedge clk)
        if(run[1])
        begin
            WB3 <= WB2;
            M3 <= M2;
            EX3 <= EX2;
            PCE <= PCD;
            A <= eFlush ? 0 : regData1;
            B <= eFlush ? 0 : regData2;
            Imm <= eFlush ? 0 : imme;
            Rd <= eFlush ? 0 : IR[11:7];
            IR3 <= eFlush ? NOP : IR;
        end

    assign pcAdd43 = PCE+4;
    assign {aluOp3, aluSrc3} = EX3;
    assign jalAddress3 = PCE+(Imm<<1);

    // ALU Control
    wire [3:0] aluControlResult3;
    ALUControl ALUControl(IR3, aluOp3, aluControlResult3);

    // ALU
    wire zero3;
    wire [1:0] forward1, forward2;
    wire [31:0] aluIn2, aluResult;
    reg [31:0] aluIn1, normalAluIn2;

    always @(*)
        case (forward1)
            0:  aluIn1 = A;
            1:  aluIn1 = regWriteData;
            2:  aluIn1 = Y;
            default:  aluIn1 = A;
        endcase

    always @(*)
        case (forward2)
            0:  normalAluIn2 = B;
            1:  normalAluIn2 = regWriteData;
            2:  normalAluIn2 = Y;
            default:  normalAluIn2 = B;
        endcase

    assign aluIn2 = aluSrc3 ? Imm : normalAluIn2;
	alu #(32) alu(aluIn1, aluIn2, aluControlResult3, aluResult, zero3);
    //        alu(   a,      b,           f,            y,      z)


    assign {memWrite3, branch3, jal3}= M3;
    assign {memWrite4, branch4, jal4}= M4;

    assign pcSrc3 = jal3 | (branch3 & zero3);


    // EX-MEM
    reg[31:0]Y, YW, BM;
    always @(posedge clk)
        if(run[2])
        begin
            WB4 <= WB3;
            M4 <= M3;
            Y <= aluResult;
            BM <= normalAluIn2;
            RdM <= Rd;
            pcAdd44 <= pcAdd43;
        end


    // 数据存储器
    wire memWE;
    wire [7:0] memAddress1, memAddress2;
    wire [31:0] memoryWriteData, memoryData1, memoryData2;

    assign memWE = memWrite4 & !ioAddress4;
    assign memAddress1 = Y;
    assign memAddress2 = m_rf_addr;
    assign memoryWriteData = BM;
    dataMemory dataMemory(memAddress1, memoryWriteData, memAddress2, clk, memWE, memoryData1, memoryData2);
    //         dataMemory(    a,              d,           dpra,     clk,  we,       spo,        dpo);


    // MEM-WB
    reg[31:0]MDR;
    reg ioAddress5;
    always @(posedge clk)
        if(run[3])
        begin
            WB5 <= WB4;
            MDR <= memoryData1;
            YW <= Y;
            RdW <= RdM;
            pcAdd45 <= pcAdd44;
            ioAddress5 <= ioAddress4;
        end

    assign {regSrc5, regWrite5}= WB5;

endmodule
