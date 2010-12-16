echo Setting up the environment for debugging gdb with PEEDI.\n

set complaints 1
set remotetimeout 10

dir .
set prompt (peedi) 


define conn
    target remote 192.168.0.109:2000
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
	#reset core
	#mon m w 0xE000ED0C 0x05FA0007
	mon res

	#reset flash
	mon m w 0x400fe0f0 1					
	set $sp = *(int*)(0)
	set $pc = *(int*)(4)
	mon set xPSR 0x01000000
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
	set $_t=main_task.tnext
	while ($_t != $_mt) && ($count <30)
		printf" %4.4s",((Task *)$_t)->name
		set $_t=(Task *)$_t->tnext
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


