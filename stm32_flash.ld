/* * Linker script cho STM32F103C6T6 
 * Flash: 32KB (0x8000)
 * RAM: 10KB (0x2800)
 */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 10K
}

/* Định nghĩa các section bắt buộc cho ARM GCC */
SECTIONS
{
  .text :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Bảng Vector ngắt */
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  _sidata = .;

  .data : AT ( _sidata )
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  /DISCARD/ :
  {
    *(.note*)
    *(.comment*)
  }
}
