/*
    GNU Linker script is used for defining memory layout
*/

/* Define program entry */
ENTRY(vector_start)

/* Define memory sections */
SECTIONS
{
    . = 0x0; /* Program starting address */

    .text :
    {
        *(vector_start)
        *(.text .rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }
}