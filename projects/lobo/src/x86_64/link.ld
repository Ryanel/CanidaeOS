ENTRY (_start)

SECTIONS
{
	KERNEL_VMA = 0x400000;
	. = KERNEL_VMA;
	/* Add a symbol that indicates the start address of the kernel. */
	_kernel_start = .;
	.text ALIGN (4K) : AT (ADDR (.text) - 0)
	{
		*(.text)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0)
	{
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0)
	{
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}

	_kernel_end = .;
}