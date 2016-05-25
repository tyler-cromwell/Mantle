# Paging

### Introduction
Ritchie uses 64-bit Paging with 4 KB page sizes.
Each table in the paging hierarchy can hold up to 512 entries, each 8 bytes in size.
The first 16 MB of the virtual address space is Identity Mapped to the first 16 MB of physical memory.
The kernel is loaded immediately after the first 16 MB of physical memory and is also Identity Mapped for its entire size.
This is to account for basic video memory, BIOS memory, GRUB boot modules, any memory holes, and kernel variables.

### Initial setup
Code for initial Paging setup is located in `arch/amd64/boot/boot.asm`.
During bootup, several memory regions are cleared for storing Pages.
These regions are:
- `0x00000500 - 0x00007bff` (29.75 KB)
- `0x00007e00 - 0x0000ffff` (32.5 KB)
- `0x00011000 - 0x0005ffff` (316 KB)

Once cleared, The paging table hierarchy is established.
First, the base address of the Page Map Level 4 Table (PML4T) is set to address `0x00011000`, which is loaded into register `CR3`.
Next, the base addresses of the first Page Directory Pointer Table (PDPT) and first Page Directory Table (PDT) are set to `0x00012000` and `0x00013000` respectively.
Finally, the hierarchy is completed by setting the base addresses for the first 8 Page Tables from `0x00014000` to `0x0001b000`.
Each entry has the first two bits set to indicate that they are Present and Writable.<br/>
<br/>
The memory layout at this point will look like this:
- `[0x00011000] -> 0x00012003` (PML4T[0] -> PDPT)
- `[0x00012000] -> 0x00013003` (PDPT[0] -> PDT)
- `[0x00013000] -> 0x00014003` (PDT[0] -> PT 1)
- `[0x00013008] -> 0x00015003` (PDT[1] -> PT 2)
- `[0x00013010] -> 0x00016003` (PDT[2] -> PT 3)
- `[0x00013018] -> 0x00017003` (PDT[3] -> PT 4)
- `[0x00013020] -> 0x00018003` (PDT[4] -> PT 5)
- `[0x00013028] -> 0x00019003` (PDT[5] -> PT 6)
- `[0x00013030] -> 0x0001a003` (PDT[6] -> PT 7)
- `[0x00013038] -> 0x0001b003` (PDT[7] -> PT 8)

At this point, the amount of Pages and Page Tables necessary to map the kernel is calculated.
The base address for the first Page Table for the kernel (PT 9) is set to `0x0001c000` and it's entry is stored at `0x00013040` (PDT[8]).
Finally, the Identity Mapping takes place.
Each Page in the virtual address space is mapped sequentially to a Page Frame in physical memory starting with the very first Page and Page Frame.
The mapping continues until both the first 16 MB + kernel are mapped.
