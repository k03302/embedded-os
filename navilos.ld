/*
    GNU Linker script는 프로그램의 메모리 레이아웃을 정의하는 데 사용됩니다. 
*/

/* 프로그램의 진입점을 정의합니다. */
ENTRY(vector_start)

/* 메모리 섹션을 정의합니다. */
SECTIONS
{
    . = 0x0; /* 프로그램 시작 주소 설정 */

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