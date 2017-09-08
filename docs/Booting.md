# Booting

### Introduction
- Define multiboot header
- Define GDT64
- Check for long mode
- Clear free memory
- Begin long mode setup
    - Setup Paging (see Paging.md for more info)
- Finish long mode setup
- Load GDT and far jump!
- Reload segment registers
- Setup arguments and jump into the Kernel
