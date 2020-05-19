# Memory in Lobo

## Memory Regions

The virtual memory map for any process in Lobo can be broken down into two regions:

```
0x0000000000000000 - 0xFFFFFFF7FFFFFFFF - User Address Space
0xFFFFFFF800000000 - 0xFFFFFFFFFFFFFFFF - Kernel Address Space
```

## Physical Memory Map

Lobo is passed a simplified and processed representation of the Grub Memory Map by AWD. Lobo then constructs its memory mapping regions based on it. 

Lobo will refuse to use the first 1MB of memory, to perserve BIOS, GRUB, and AWD structures. It will not clobber GRUB modules in memory either. It will only use memory regions marked as "FREE" by AWD.

## Virtual Memory
### Map Requests
A map request is a request to map some `v_address` to some `p_address`. It has `perm` permissions, and can signal many `flag`s.

v_address can be nullptr, which allows the VMM to pick it.

Virtual Memory map requests can ask for a couple of types of mappings:

* VMM_MAP_UNBACKED: Requests that there be no physical memory allocated from the PMM for this request.
* VMM_MAP_REGION_KERNEL: Map `v_address - KERNEL_VMB` into the kernel address space.
* VMM_MAP_IDENTITY: Force an identity map for `v_address`.
* VMM_MAP_FIXED: `v_address` will be mapped exactly with no fudging.
* VMM_MAP_NOCLEAR: