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