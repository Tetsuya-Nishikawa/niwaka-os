ENTRY(efi_main);

SECTIONS{
    . = 0x0000000000401000;
    .text : {*(.text)}
    .data : {*(.data)*(.rodata*)}
    .bss  : {*(.bss)}
}

