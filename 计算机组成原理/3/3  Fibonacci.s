.text
main:
    addi    s0, x0, 10
    addi    a2, x0, 1000
    addi    a0, x0, 1
    addi    a1, x0, 2

    sw      a0, 0(a2)
    sw      a1, 4(a2)
    addi    t0, x0, 2
    addi    a2, a2, 4

    mark:
        add     t1, a0, a1      # t1 = a0 + a1
        addi    a0, a1, 0
        addi    a1, t1, 0

        addi    t0, t0, 1
        addi    a2, a2, 4
        sw      a1, 0(a2)
    beq     t0, s0, exit
    jal     x1, mark
    exit:
