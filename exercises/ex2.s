.syntax unified
.arch armv7-m
.cpu cortex-m4
.thumb

.global _start

.text
_start:
    ldr r0, =ap      @ charge l'addresse de a
    ldr r1, [r0]    @ charge l'addresse pointé par a
    ldr r2, [r1]    @ charge la valeur pointé par a
    ldr r3, =cp      @ charge l'addresse de c
    ldr r4, [r3]    @ charge la valeur pointé par c
    ldr r5, [r4]    @ charge la valeur pointé par c
    add r2, r2, r5  @ additionne les valeurs
    str r2, [r1]    @ stocke la valeur dans l'adresse pointé par a
    ldr r5, [r4]    @ charge la valeur pointé par c
    ldr r0, =bp      @ charge l'addresse de b
    ldr r1, [r0]    @ charge l'addresse pointé par b
    ldr r2, [r1]    @ charge la valeur pointé par b
    add r2, r2, r5  @ additionne les valeurs
    str r2, [r1]    @ stocke la valeur dans l'adresse pointé par b
_end:
    b _end

.data
ap:
    .global ap
    .word 0x00000000
bp:
    .global bp
    .word 0x0000000A
cp:
    .global cp
    .word 0x0000000B
    