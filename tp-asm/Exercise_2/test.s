.syntax unified
.arch armv7-m
.cpu cortex-m4

.thumb

.global _start

.equ constante,  0xdeadbeef
.equ memoire,    0x10000000
.equ memoire2,   0x10000300
.equ taille,     64

_start:
  ldr r4, =memoire2      @ address zone 2
  ldr r3, =memoire       @ address zone 1
  ldr r2, =taille        @ max value
  ldr r1, =constante     @ value to be saved
  mov r0, #0             @ counter
loop:
  str r1, [r3], #4
  add r0, #1
  cmp r2, r0
  bne loop
  mov r0, #0             @ counter
  ldr r3, =memoire       @ address zone 1
loop2:
  ldr r1, [r3], #4
  str r1, [r4], #4
  add r0, #1
  cmp r2, r0
  bne loop2
end:
  b end

