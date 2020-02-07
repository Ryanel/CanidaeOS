ENTRY (_start)

SECTIONS
{
	KERNEL_VMA = 0xFFFFFFFF00000000;
	. = KERNEL_VMA;
	/* Add a symbol that indicates the start address of the kernel. */
	_kernel_start = .;
	.text ALIGN (4K) : AT (ADDR (.text) - KERNEL_VMA)
	{
		*(.text)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - KERNEL_VMA)
	{
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - KERNEL_VMA)
	{
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - KERNEL_VMA)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}

	_kernel_end = .;
}