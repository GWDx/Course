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

            7'b0010011: // addi, andi, ori, xori, srli
                imme = {{20{instruction[31]}}, instruction[31:20]};
            7'b1100111: // jalr
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
            7'b0000011: controls = 9'b010110000;    // lw
            7'b0100011: controls = 9'b010001000;    // sw
            7'b0110011: controls = 9'b000010010;    // add, and, or, xor, srl
            7'b0010011: controls = 9'b010010010;    // addi, andi, ori, xori, srli
            7'b1100011: controls = 9'b000000101;    // beq
            7'b1101111: controls = 9'b101010011;    // jal
            7'b1100111: controls = 9'b011010010;    // jalr
            default controls = 0;
        endcase
endmodule


module ALUControl(instruction,aluOp,aluControlResult);
    input [31:0] instruction;
    input [1:0] aluOp;
    output reg [3:0] aluControlResult;

    wire [2:0] funct3;
    wire [6:0] funct7, opcode;

    assign funct3 = instruction[14:12];
    assign funct7 = instruction[31:25];
    assign opcode = instruction[6:0];

    always @(*)
        case(aluOp)
            0:
            case(opcode)
                7'b0000011: aluControlResult = 2;   // lw
                7'b0100011: aluControlResult = 2;   // sw
                7'b0010011:
                case(funct3)
                    3'b000: aluControlResult = 2;   // addi
                    3'b100: aluControlResult = 3;   // xori
                    3'b101: aluControlResult = 5;   // srli
                    3'b110: aluControlResult = 1;   // ori
                    3'b111: aluControlResult = 0;   // andi
                    default:aluControlResult = 2;
                endcase
                default:    aluControlResult = 2;
            endcase

            1:
            case(funct3)
                3'b000: aluControlResult = 6;   // beq
                3'b111: aluControlResult = 7;   // bgeu
                default:aluControlResult = 6;
            endcase

            2:  //R 型
            case(funct3)
                3'b000:
                case(funct7)
                    7'b0000000: aluControlResult = 2;   // add
                    7'b0100000: aluControlResult = 6;   // sub
                    default:    aluControlResult = 2;
                endcase
                3'b100: aluControlResult = 3;   // xor
                3'b101: aluControlResult = 5;   // srl
                3'b110: aluControlResult = 1;   // or
                3'b111: aluControlResult = 0;   // and
                default:aluControlResult = 2;
            endcase
            3:  aluControlResult = 15;  // jal
            default:    aluControlResult = 15;
        endcase
endmodule
