.data
char1 : .byte 'a'
char2 : .byte 'b'
int1 : .word 757326
chararray : .space 14
int2 : .word 757326
a : .asciiz "fdwsaa12\\n"
aa : .asciiz "3rr34e"
x2: .byte 5
x1 : .word 49
.extern x 3
.extern y 8
.text
fun1:
lui $t5 '4'
.globl main
main:
li $t1 -26
sw $t1 x1
la $a0 aa
li $v0 4
syscall
la $s1 x1
sw $t1, -4($sp)
sw $t1 int2
seq $t9, $0, $0

li $t1 0xf0000001
mul $t2 $t1 16

