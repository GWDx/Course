// 立即数生成
module immeGen(instruction,imme);
    input [31:0] instruction;
    output reg [31:0] imme;

    wire [6:0] opcode;
    assign opcode = instruction[6:0];
    always @(*)
        case(opcode)
            7'b0000011: // lw
                imme = {{20{instruction[31]}}, instruction[31:20]};

            7'b0100011: // sw
                imme = {{20{instruction[31]}}, instruction[31:25], instruction[11:7]};

            7'b0010011: // addi
                imme = {{20{instruction[31]}}, instruction[31:20]};

            7'b1100011: // beq
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

endmodule


// 控制器
module controlUnit(instruction,controls);
    input [31:0] instruction;
    output reg [8:0] controls;

    wire [6:0] opcode;
    assign opcode = instruction[6:0];
    always @(*)
        case(opcode)
            7'b0000011: controls = 9'b010110000;  // lw
            7'b0100011: controls = 9'b010001000;  // sw
            7'b0110011: controls = 9'b000010010;  // add
            7'b0010011: controls = 9'b010010010;  // addi
            7'b1100011: controls = 9'b000000101;  // beq
            7'b1101111: controls = 9'b101010011;  // jal
            default controls = 0;
        endcase
endmodule


module ALUControl(instruction,aluOp,aluControlResult);
    input [31:0] instruction;
    input [1:0] aluOp;
    output reg [3:0] aluControlResult;

    wire funct7;
    assign funct7 = instruction[31:25];
    always @(*)
        case(aluOp)
            0:  aluControlResult = 2;   // lw, sw, addi
            1:  aluControlResult = 6;   // beq
            2:  //R 型
            case(funct7)
                7'b0000000: aluControlResult = 2;    // add
                7'b0100000: aluControlResult = 6;
                default:    aluControlResult = 2;
            endcase

            default:    aluControlResult = 15;  // jal
        endcase
endmodule
