.syntax unified
.arch armv7-m
.cpu cortex-m4

.thumb

.global _start

.equ memoire,    0x10000000
.equ memoire2,   0x10000300
.equ memoire3,   0x10000600
.equ taille,     4

_start:
  ldr r3, =memoire       @ initial address of memory zone
  ldr r2, =taille        @ max value
  mov r0, #0             @ counter
  bl function
  ldr r3, =memoire2
  bl function
  ldr r3, =memoire3
  bl function
end:
  b end

@ function to copy and paste a string
function:
  adr r1, string         @ string to be saved
loop_function:
  ldrb r4, [r1], #1
  strb r4, [r3], #1
  cmp r4, #0
  bne loop_function
  mov pc, lr

string: 
  .asciz "Bonjour le monde!" 
