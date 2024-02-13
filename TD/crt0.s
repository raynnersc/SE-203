.syntax unified
.arch armv7-m
.cpu cortex-m4
.global _start
.global _exit
.thumb

.section .bootloader, "ax", %progbits

.thumb_func
_start:
    ldr sp, =_stack_start   @ addresse initiale de la pile
    bl init_text
    bl init_data
    bl init_irq
    bl init_bss
    bl main
_exit:
    b _exit
