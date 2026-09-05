/**
 * @file    linker_script.ld
 * @author  Pratik Dhulubulu
 * @brief   Linker Script File.
 */

ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
  /* FLASH */
  .text :
  {
    KEEP(*(.isr_vector))
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  .ARM.exidx :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } > FLASH

  /* End of FLASH */
  _etext = .;

  /* RAM */
  .data : AT(_etext)
  {
    _sidata = LOADADDR(.data);

    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM

  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  /* HEAP & STACK */

  ._user_heap_stack (NOLOAD) :
  {
    . = ALIGN(8);
    _sheap = .;
    . = . + 0x2000;     /* 8 KB heap */
    _eheap = .;

    . = ALIGN(8);
    _sstack = .;
    . = ORIGIN(RAM) + LENGTH(RAM);
    _estack = .;
  } > RAM

  /* DISCARD */

  /DISCARD/ :
  {
    *(.comment)
    *(.note*)
    *(.eh_frame*)
  }
}
