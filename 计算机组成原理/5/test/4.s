.text
    addi x1,x0,0
    addi x2,x0,0
    addi x5,x0,5
begin:
    addi x1,x1,1
    sw x1,0(x0)
    lw x2,0(x0)
    beq x2,x5,end
    jal x6,begin
end:
    addi x6,x0,6
