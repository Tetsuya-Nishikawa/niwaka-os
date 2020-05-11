ENTRY(niwaka_main);

SECTIONS{
    . = 0xa0000000 + SIZEOF_HEADERS;
    .text : {*(.text)}
    . = 0xa0004000;
    .data : {*(.data)*(.rodata*)}
    . = 0xa0006000;
    .bss  : {*(.bss)}
}

