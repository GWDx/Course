module hazardDetection(clk, runAll2, IR, IR3, eFlush, fStall, dStall, prevJump);
    input clk, runAll2;
    input [31:0] IR, IR3;
    // input [4:0] regRead1, regRead2, Rd;
    output eFlush, fStall, dStall;
    output reg prevJump;

    wire [6:0] opcode;
    wire bubble, jump, condition, rawBubble;
    reg prevJump2, prevBubble;

    lwDataRelate lwDataRelate(IR, IR3, rawBubble);

    assign opcode = IR[6:0];
    assign condition = !runAll2 || bubble || prevJump2 || prevJump;
    assign jump = condition ? 0 : opcode == 7'b1100011 || opcode == 7'b1101111 || opcode == 7'b1100111; // 跳转, jal
    assign bubble = !runAll2 ? 0 : rawBubble;

    assign fStall = bubble || jump;
    assign dStall = bubble || prevJump || jump;
    assign eFlush = bubble || prevJump2 || prevJump;

    always @(posedge clk)
    begin
        prevJump <= jump;
        prevJump2 <= prevJump;
        prevBubble <= bubble;
    end
endmodule



module lwDataRelate(IR, IR3, bubble);
    input [31:0] IR, IR3;
    output bubble;

    wire [4:0] regRead1, regRead2, Rd;
    wire [6:0] opcode;
    wire IR3ld, read2, dataRelated;

    assign opcode = IR[6:0];
    assign regRead1 = IR[19:15];
    assign regRead2 = IR[24:20];
    assign read2 = opcode == 7'b0110011 || opcode == 7'b1100011;    // R型, beq

    assign Rd = IR3[11:7];
    assign IR3ld = IR3[6:0] == 7'b0000011;    // lw

    assign dataRelated = Rd == regRead1 || read2 && Rd == regRead2;
    assign bubble = IR3ld && dataRelated;
endmodule



module dataForward(IR3, RdM, RdW, regWrite4, regWrite5, forward1, forward2);
    input [31:0] IR3;
    input [4:0] RdM, RdW;
    output [1:0] forward1, forward2;
    input regWrite4, regWrite5;

    wire [4:0] regSource1, regSource2;
    wire [6:0] opcode;
    wire dataRelatedM1, dataRelatedM2, dataRelatedW1, dataRelatedW2;

    assign regSource1 = IR3[19:15];
    assign regSource2 = IR3[24:20];

    assign dataRelatedM1 = regWrite4 && RdM == regSource1 && RdM!= 0;
    assign dataRelatedM2 = regWrite4 && RdM == regSource2 && RdM!= 0;
    assign dataRelatedW1 = regWrite5 && RdW == regSource1 && RdW!= 0;
    assign dataRelatedW2 = regWrite5 && RdW == regSource2 && RdW!= 0;

    assign forward1 = dataRelatedM1 ? 2 : dataRelatedW1 ? 1 : 0;
    assign forward2 = dataRelatedM2 ? 2 : dataRelatedW2 ? 1 : 0;
endmodule
