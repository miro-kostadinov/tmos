OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(sys_reset_handler)
SEARCH_DIR(.)

/*
 * Define stack size here
 */
FIQ_STACK_SIZE = 0x0010;
IRQ_STACK_SIZE = 0x02C0;
ABT_STACK_SIZE = 0x0010;
UND_STACK_SIZE = 0x0010;
SVC_STACK_SIZE = 0x0240;
USR_STACK_SIZE = 0x0080;

 
MEMORY
{
  rom (rx)   : ORIGIN = 0x00000000, LENGTH = 256k
  ram (rwx)  : ORIGIN = 0x20000000, LENGTH = 48k
}

/*
 * Do not change the next code
 */
SECTIONS
{

	/* ----- exceptions -------- */
	.exceptions :
	{
        KEEP(*(.ExceptionVectors))
        KEEP(*(.SWIHandler))
	} >rom
	

	/* ----- fixed(text) -------- */
	.fixed :
	{
        _sfixed = .;
        
		*(.text .text.*) 
		*(.rodata .rodata*)
  		KEEP(*(.init))
    	KEEP(*(.fini))

		KEEP(*(.romcodesec))
		KEEP(*(.romcodesec.*))
		KEEP(*(.rominitsec))
		KEEP(*(.rominitsec.*))

		*(.gnu.linkonce.t.*)
		*(.gnu.linkonce.r.*)
		*(.glue_7) 		/* all .glue_7 sections (no idea what these are) */
		*(.glue_7t)
        _efixed = .;
	} >rom 
	

	/* for exception handling/unwind - some Newlib functions (in common with C++ and STDC++) use this. */
	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > rom
	
	.ARM.exidx :
	{
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} > rom

	.ctors :
	{
		PROVIDE(_ctors_start_ = .);
    	KEEP(*(.ctors))
   		KEEP(*(SORT(.ctors.*)))
   		PROVIDE(_ctors_end_ = .);
  	} >rom

	.dtors :
  	{
    	PROVIDE(_dtors_start_ = .);
    	KEEP(*(.dtors))
    	KEEP(*(SORT(.dtors.*)))
    	PROVIDE(_dtors_end_ = .);
  	} >rom

	
	.noinit (NOLOAD) :
	{
		*(.ramnoinitsec)
		*(.ramnoinitsec.*)
  	} >ram
	    
	/* section .relocate is placed in rom and copied by run code to ram */
	.relocate : AT (_dtors_end_)
	{
    	_srelocate = .;
    	
		*(.ramcodesec)
		*(.ramcodesec.*)
	    *(.data)
	    
	    _erelocate = .;
  	} >ram 


	/* section .zero (bss) is placed in ram and cleared by run code */
	.zero (NOLOAD) :
	{
        _szero = .;
	    *(.bss)
		*(.bss.*)
		*(.gnu.linkonce.b.*)
	    *(COMMON) 
        _ezero = .;
  	} >ram
	    
	/* section .tasks is placed in ram and filled run code */
	.tasks (NOLOAD) :
	{
        _stasks = .;
	  	*(SORT(.task.*));
	    . = ALIGN(4);
        _etasks = .;
  	} >ram


	/* section .stacks is placed in ram  */
	.stacks (NOLOAD) :
	{
        _sstasks = .;
	    PROVIDE (__stack_abt_start = .);	/*abort*/
	    . += ABT_STACK_SIZE;
	    PROVIDE (__stack_abt_end = .);
	
	    PROVIDE (__stack_und_start = .);	/*undef*/
	    . += UND_STACK_SIZE;
	    PROVIDE (__stack_und_end = .);
	
	    PROVIDE (__stack_usr_start = .);	/*user - not really ussed*/
	    . += USR_STACK_SIZE;
	    PROVIDE (__stack_usr_end = .);
	
	    PROVIDE (__stack_svc_start = .);	/*SVC*/
	    . += SVC_STACK_SIZE;
	    PROVIDE (__stack_svc_end = .);
	    
	    PROVIDE (__stack_irq_start = .);   /*IRQ*/
	    . += IRQ_STACK_SIZE;
	    PROVIDE (__stack_irq_end = .);
	
	    PROVIDE (__stack_fiq_start = .);	/*FIQ*/
	    . += FIQ_STACK_SIZE;
	    PROVIDE (__stack_fiq_end = .);
        _estasks = .;
  	} >ram
  
	. = ALIGN(4);
	
	PROVIDE (end = .);

	/* Exception handling??? GDB cannot unwind without this section
	
	// replaced by -fno-dwarf2-cfi-asm  
	    
  	eh_frame   : ONLY_IF_RW 
  	{ 
  		KEEP (*(.eh_frame)) 
  	} 
	*/

	/* The following sections are for the debugger only and are never loaded to the target */
	
	.stab 0 (NOLOAD) : 
	{
		*(.stab)
	}
	.stabstr 0 (NOLOAD) : 
	{
		*(.stabstr)
	}
	
	/* DWARF debug sections.
	* Symbols in the DWARF debugging sections are relative to the beginning
	* of the section so we begin them at 0.
	*/
	/* DWARF 1 */
	.debug 0 : { *(.debug) }
	.line 0 : { *(.line) }

}

