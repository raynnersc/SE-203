.syntax unified
.arch armv7-m
.cpu cortex-m4
.thumb

.global _start

.text
_start:
    ldr r0, =a
    ldr r1, [r0]    @ charge le contenu de la variable a
    mov r2, #0      @ variable i
loop:
    cmp r2, r1
    bgt _end
    bl  g
    add r2, r2, #1
    b loop
_end:
    b _end
g:
    bx lr

.data
a:
    .word 0x00000003
