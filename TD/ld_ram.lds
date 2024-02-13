ENTRY(_start)

MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1M
	SRAM1 (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
	SRAM2 (rwx) : ORIGIN = 0x10000000, LENGTH = 32K
}

_stack_start = ORIGIN(SRAM2) + LENGTH(SRAM2);

_data_src = LOADADDR(.data);
_irq_src = LOADADDR(.irq);
_text_src = LOADADDR(.text);

SECTIONS
{
	.irq_init : {
		*(.irq_init)
	} > FLASH

	.irq : {
		_irq_dst_start = .;
		*(.irq)
		_irq_dst_end = .;
	} > SRAM1 AT> FLASH 
	
	.bootloader : {
		*(.bootloader)
	} > FLASH

	.text : {
		_text_dst_start = .;
		*(.text)
		_text_dst_end = .;
	} > SRAM1 AT> FLASH 

	.rodata : {
		*(.rodata)
	} > FLASH

	.data : {
		_data_dst_start = .;
		*(.data)
		_data_dst_end = .;
	} > SRAM1 AT> FLASH  

	.bss : {
		_bss_start = .;
		*(.bss) *(COMMON)
		_bss_end = .;
	} > SRAM1
}
