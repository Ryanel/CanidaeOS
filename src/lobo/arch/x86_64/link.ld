ENTRY (_start)

SECTIONS
{
	KERNEL_VMB = 0xffffffff80000000;
	KERNEL_VMA = 0xffffffff80200000;
	
	. = KERNEL_VMA;

	_kernel_start = .;
	
	.text ALIGN (0x1000) : AT (ADDR (.text) - KERNEL_VMA)
	{
		_kernel_ro_start = .;
		*(.text)
	}
	.rodata ALIGN (0x1000) : AT (ADDR (.rodata) - KERNEL_VMA)
	{
		*(.rodata)
	}

	.data ALIGN (0x1000) : AT (ADDR (.data) - KERNEL_VMA)
	{
		_kernel_ro_end = .;
		_kernel_data_start = .;
		*(.data)
		*(.ctor*)
        *(.dtor*)
	}
	.bss ALIGN (0x1000) : AT (ADDR (.bss) - KERNEL_VMA)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}

	.ehframe ALIGN (0x1000) : AT(ADDR(.ehframe) - KERNEL_VMA)
    {
        ehframe = .;
        *(.ehframe)
        . = ALIGN(4096);
    }

	_kernel_data_end = .;
	_kernel_end = .;
}