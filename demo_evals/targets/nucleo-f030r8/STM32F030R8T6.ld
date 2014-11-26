OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(sys_reset_handler)
SEARCH_DIR(.)

/*
 * Define stack size here
 */
SVC_STACK_SIZE = 0x0340;

 
MEMORY
{
  rom (rx)   : ORIGIN = 0x00000000, LENGTH = 128k
  ram (rwx)  : ORIGIN = 0x20000000, LENGTH = 20k
}

/*
 * Do not change the next code
 */
SECTIONS
{

	/* ----- exceptions -------- */
	.text :
	{
        KEEP(*(.ExceptionVectors))
        KEEP(*(.DriverTable))
        KEEP(*(.SWIHandler))
		*(.text .text.*) 
	} >rom
	

	/* ----- fixed(text) -------- */
	.rodata :
	{
        
		*(.rodata .rodata*)
		
  		KEEP(*(.init))
    	KEEP(*(.fini))
  		
  		/* --- preinit array --- */
	    . = ALIGN(4);
		PROVIDE (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE (__preinit_array_end = .);
  		
  		/* --- init array(ctors) --- */
	    . = ALIGN(4);
		PROVIDE (__init_array_start = .);
		PROVIDE (_ctors_start_ = .);
    	KEEP (*(.ctors SORT(.ctors.*)))		 		/* old arm-elf- format	*/
		KEEP (*(.init_array SORT(.init_array.*))) 	/*new arm-none-eabi- format */
   		PROVIDE (_ctors_end_ = .);
		PROVIDE (__init_array_end = .);

  		/* --- fini array(dtors) --- */
	    . = ALIGN(4);
		PROVIDE (__fini_array_start = .);
    	PROVIDE (_dtors_start_ = .);
    	KEEP (*(.dtors SORT(.dtors.*)))				/* old arm-elf- format	*/
		KEEP (*(.fini_array SORT(.fini_array.*)))	/* new arm-none-eabi- format */
		PROVIDE (__fini_array_end = .);
    	PROVIDE(_dtors_end_ = .);

  		/* --- tmos task descriptins etc --- */
		KEEP(*(.romcodesec .romcodesec.*));
		KEEP(*(.rominitsec .rominitsec.*));

        /* --- version --- */
        __BUILD_VERSION = .;
        KEEP(*(.version))
        BYTE(0)
        . = ALIGN(4);
        
  		/* --- no idea --- */
		*(.gnu.warning)
		*(.gnu.linkonce.t.*)
		*(.gnu.linkonce.r.*)
		*(.glue_7) 		/* all .glue_7 sections (no idea what these are) */
		*(.glue_7t)
		*(.got)
		*(.gcc_except_table)
		
        _efixed = .;
	} > rom 
	
	/* section .data is placed in rom and copied by run code to ram */
	.data : 
	{
    	_srelocate = .;
    	
		*(.ramcodesec)
		*(.ramcodesec.*)
	    *(.data .data.* .gnu.linkonce.d.*)
	    
	    _erelocate = .;
  	} > ram AT > rom
  	 
	.bss  :
	{
		*(.ramnoinitsec .ramnoinitsec.*)
		
        _szero = .;
	    *(.bss .bss.* .gnu.linkonce.b.*)
	    *(COMMON) 
        _ezero = .;
        
	    . = ALIGN(4);
        _stasks = .;
	  	*(SORT(.task.*));
        _etasks = .;
        
	    . = ALIGN(4);
	    PROVIDE (__stack_svc_start = .);	/*SVC*/
	    . += SVC_STACK_SIZE;
	    PROVIDE (__stack_svc_end = .);
	    
		PROVIDE (end = .);
		
  	} > ram


	/* for exception handling/unwind - some Newlib functions (in common with C++
		 and STDC++) use this. */
	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > ram
	
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > ram
	__exidx_end = .;
	

	/* Exception handling??? GDB cannot unwind without this section
		 replaced by -fno-dwarf2-cfi-asm */
  	eh_frame   : ONLY_IF_RW 
  	{ 
  		KEEP (*(.eh_frame)) 
  	} >ram 


	/* The following sections are for the debugger only and are never loaded to the target */

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	        
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	
	/* DWARF 3 */
	.debug_pubtypes 0 : { *(.debug_pubtypes) }
	.debug_ranges   0 : { *(.debug_ranges) }
	
	.ARM.attributes 0 : { *(.ARM.attributes) *(.gnu.attributes) }
	.note.gnu.arm.ident 0 : { *(.note.gnu.arm.ident) }
	/DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) }
}

