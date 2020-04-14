ENTRY(niwaka_main);

SECTIONS{
    . = 0x0c000000 + SIZEOF_HEADERS;
    .text : {*(.text)}
    . = 0x0c004000;
    .data : {*(.data)*(.rodata*)}
    . = 0x0c006000;
    .bss  : {*(.bss)}
}

