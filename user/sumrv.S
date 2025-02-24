.data                         # Data section
msg_incfmt: .asciz "Incorrect format\n"
arr: .space 20
sp: .string " "
out: .string "%d\n"


.text                         # Text (code) section
.global main




main:
    la a0, arr
    li a1, 20
    call gets

    la t3, sp
    lb t2, 0(t3)

    la t0, arr
    la a0, arr
    call atoi
    addi t4, a0, 0
    
    j loop

loop:
    lb t1, 0(t0)
    addi t0, t0, 1
    beq t1, zero, inc_fmt
    bne t1, t2, loop
    
    addi a0, t0, 0
    call atoi
    add t4, t4, a0

    la a0, out
    addi a1, t4, 0
    call printf

    j exit

exit:
    
    li a7, 2
    li a0, 0
    ecall 

inc_fmt:
    
    addi a0, zero, 2
    la a1, msg_incfmt
    call fprintf
    j exit
