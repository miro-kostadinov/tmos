echo Setting up the environment for debugging gdb with PEEDI.\n

set complaints 1
set remotetimeout 10
set backtrace limit 100

dir .
set prompt (peedi) 


define conn
    target remote peedi:2000
    info target
end

# define user command: disable all interrupts and load program
#-------------------------------------------------------------

#-------------------------------------------------------------------------------
# mem_info 
#-------------------------------------------------------------------------------
define mem_info
	set $_ptr = &end
	set $_used = 0
	set $_usedspace = 0
	set $_free = 0
	set $_freespace = 0
	
	echo ---------- Memory info -------------------- \n
	printf"\r\n %08X %04X %04X  %04X ", $_ptr, ((unsigned short*)$_ptr)[0], ((unsigned short*)$_ptr)[1]>>1, ((unsigned short*)$_ptr)[2]
	
	while ((unsigned short*)$_ptr)[0]
		set $_ptr = &((unsigned short*)$_ptr)[((unsigned short*)$_ptr)[0]<<1]
		set $_p0 = ((unsigned short*)$_ptr)[0]
		set $_p1 = ((unsigned short*)$_ptr)[1]
		set $_p2 = ((unsigned short*)$_ptr)[2]
		set $_p3 = ((unsigned short*)$_ptr)[3]
		
		printf"\n %08X %04X %04X ", $_ptr, $_p0, $_p1>>1
		if !($_p1 & 1)
			printf"              "
			set $_used =  $_used +1
			set $_usedspace =  $_usedspace + ($_p0<<2)
		else
			set $_free =  $_free +1
			set $_freespace =  $_freespace + ($_p0<<2)
		end
		printf" %04X %04X", $_p2, $_p3
		if $_p0 && $_p0 != ( ((unsigned short*)$_ptr)[($_p0<<1)+1] >> 1)
			echo \n !!!!!! ************* 
		end
	end
	echo \n ---------- check empty holes -------------------- 
	set $_ptr = &end
	while ((unsigned short*)$_ptr)[0]
		set $_ptr = &((unsigned short*)$_ptr)[((signed short*)$_ptr)[2]<<1]
		
		set $_p0 = ((unsigned short*)$_ptr)[0]
		set $_p1 = ((unsigned short*)$_ptr)[1]
		set $_p2 = ((signed short*)$_ptr)[2]
		set $_p3 = ((signed short*)$_ptr)[3]
		
		printf"\n %08X %04X %04X %04X %04X", $_ptr, $_p0, $_p1>>1, $_p2, $_p3
		if $_p0 && !($_p1 & 1)
			echo \n used !!!!!! *************
			break 
		end
		if $_p0 && $_p2 !=  (((signed short*)$_ptr)[($_p2<<1)+3] )
			echo \n !!!!!! ************* 
		end
		
	end

	echo \n ---------- Memory info -------------------- 
	printf"\n used %d bytes in %d ptrs", $_usedspace, $_used
	printf"\n free %d bytes in %d holes", $_freespace, $_free
end

#-----------------------
# app PEEDI reset
#-----------------------
define cc
	# ------------- reset core

	set $_ID = *(unsigned int*)0xE0042000
	if $_ID >0
		echo ---------- STM32 reset ---------------- \n
	
		# NVIC_APINT = NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ | NVIC_APINT_VECT_CLR_ACT | NVIC_APINT_VECT_RESET
		# ... System reset request | Clear active NMI/fault info | System reset
		#mon m w 0xE000ED0C 0x05FA0007	
		mon set CONTROL 0
		mon m w 0xE000ED0C 0x05FA0003	
		mon set xPSR 0x01000000
		#mon res	

		set $_start = *(int*)4
		if $_start >= 0x01000000
			echo PROBLEM> Invalid flash content!
		end
	
		#jump to app
	
		# NVIC_ST_CTRL
		mon m w 0xe000e010 0x00
		set $sp = *(int *)0x00
		set $pc = *(int *)0x04
		#sys_reset_handler
		si
	else
		echo ---------- LM3S reset -------------------- \n
		mon m w 0xE000ED0C 0x05FA0007
		mon set xPSR 0x01000000
		#mon res	

		#reset flash for LM3S
		if *(int*)0x400fe0f0 == 0x00000001
			echo Mapping flash...\n
			mon m w 0x400fe0f0 1	
			set $_start = *(int*)4
			if $_start < 0x01000000
				#mon res
				set $sp = *(int *)0x0
				set $pc = $_start
	
				if $_start == 0x9b9
					set $_start = *(int*)0x1004
					echo Execute boot ROM and FLASH PATCH...\n
				else				
					echo Execute boot ROM...\n
				end
	
				tbreak  * ((int*)$_start)
				c
			else
				echo PROBLEM> Invalid flash content! \n
			end
	
		else
			set $_start = *(int*)4
			if $_start < 0x01000000
				if $_start == 0x9b9
					echo Execute FLASH PATCH...\n
	
					#mon res
					mon m w 0xE000ED0C 0x05FA0007
					set $sp = *(int *)0x0
					set $pc = $_start

					set $_start = *(int*)0x1004
					tbreak  * ((int*)$_start)
					c
				end
			else
				echo PROBLEM> Invalid flash content! \n
			end
		end
	
		#jump to app
	
		mon m w 0x400FE044 0xFFFFFFFF
		mon m w 0xe000e010 0x00
		mon m w 0x400FE044 0x0
		set $sp = *(int *)0x4100
		set $pc = sys_reset_handler
		si
	end
end


#define ___hook-stop
#	tt
#end

define tt
	echo \nCURRENT: 
	if $r11 == 0
		echo NONE\n
	else		
		printf" %4.4s", ((Task *)$r11)->name
	end
	echo \n  READY:
	set $_mt=&main_task
	set $_t=main_task.next
	set $count=0
	while ($_t != $_mt) && ($count <30)
		printf" %4.4s",((Task *)$_t)->name
		set $_t=(Task *)$_t->next
		set $count=1+(int)$count
	end

	set $count=0
	echo \n   WAIT:
	set $_mt=&main_task
	set $_t=main_task.tprev
	while ($_t != $_mt) && ($count <30)
		printf" %4.4s(%d)",((Task *)$_t)->name, ((Task *)$_t)->time - ((Task *)$_mt)->time
		set $_t=(Task *)$_t->tprev
		set $count=1+(int)$count
	end
end

# string display command
define str
	if( $arg0.ptr == 0 )
		echo Undefined	\n
	else
		printf"len:%d\n", $arg0.ptr.len
		Printf"adr:%x\n", &$arg0.ptr.len
		printf"buf:%s", $arg0.ptr.buf	
end

# start connect command
#----------------------
#conn
#fast
#load
#si


