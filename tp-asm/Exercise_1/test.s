.syntax unified
.arch armv7-m
.cpu cortex-m4

.thumb

.global _start


.equ constante,  0xdeadbeef
.equ memoire,    0x10000000
.equ taille,     64

/*.text*/
_start:
  ldr r3, =memoire       @ address
  ldr r2, =taille        @ max value
  ldr r1, =constante     @ value to be saved
  mov r0, #0             @ counter
loop:
  str r1, [r3], #4
  add r0, #1
  cmp r2, r0
  bne loop
end:
  b end
