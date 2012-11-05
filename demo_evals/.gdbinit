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

#-----------------------
# boot OCD reset
#-----------------------
define bccc
	set $temp0 = *(int*)(0)
	set $temp1 = *(int*)(0x00100000)
	set $temp2 = *(int*)(0x00200000)
	if $temp1 == $temp2
		set $temp2 = $temp2+1
		mon mww 0x00200000 $temp2
		set $temp0 = *(int*)(0)
	end
	if $temp0 == $temp2
		mon mww 0xffffff00 1
	end
	mon mww 0xFFFFF20C 0xFFFFFFFF
	mon reg cpsr 0xd3
	mon reg pc 0
	si
end

#-----------------------
# boot PEEDI reset
#-----------------------
define bcc
	set $temp0 = *(int*)(0)
	set $temp1 = *(int*)(0x00100000)
	set $temp2 = *(int*)(0x00200000)
	if $temp1 == $temp2
		set $temp2 = $temp2+1
		mon m w 0x00200000 $temp2
		set $temp0 = *(int*)(0)
	end
	if $temp0 == $temp2
		mon m w 0xffffff00 1
	end
	mon m w 0xFFFFF20C 0xFFFFFFFF
	mon set pc 0
	mon set cpsr 0xd3
	mon set pc 0
	si
end

#-----------------------
# app OCD reset
#-----------------------
define ccc
	mon mww 0xE000ED0C 0x05FA0007
	#reset flash
	mon mww 0x400fe0f0 1					
	set $sp = *(int*)(0)
	set $pc = *(int*)(4)
	mon reg xPSR 0x01000000
	si	

end

#-----------------------
# app PEEDI reset
#-----------------------
define cc
	# ------------- reset core

	# NVIC_APINT = NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ | NVIC_APINT_VECT_CLR_ACT | NVIC_APINT_VECT_RESET
	# ... System reset request | Clear active NMI/fault info | System reset
	mon m w 0xE000ED0C 0x05FA0007	
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
		echo Undefined	
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


