.text
    addi x1,x0,45
    addi x2,x0,26
    addi x3,x0,2

# srl
    srli x4,x1,1
    srl  x5,x1,x3
# xor
    xori x6,x1,11
    xor  x7,x1,x2
# or
    ori  x8,x1,11
    or   x9,x1,x2
# and
    andi x10,x1,11
    and x11,x1,x2

    addi x1,x0,1
    addi x2,x0,2
    addi x3,x0,0x304
# sh,sb
    sh x3,0(x0)
    sb x2,2(x0)
    sb x1,3(x0)
# lhu,lbu
    lw x12,0(x0)
    lhu x13,2(x0)
    lbu x14,1(x0)
    lb x15,1(x0)
# lh,lb
    addi x1,x0,-2
    sw x1,4(x0)
    lh x16,4(x0)
    lb x17,4(x0)
# bgeu, jalr
    addi x1,x0,0
    addi x2,x0,2
    addi x3,x0,12
loop:
    addi x1,x1,1
    bgeu x2,x1,loop
    bgeu x1,x3,end
    jalr x18,x3,100
end:
