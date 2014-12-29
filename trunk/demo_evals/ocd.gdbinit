echo Setting up the environment for debugging gdb with OPENOCD.\n

dir .
set prompt (openocd) 


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
	monitor reset init
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


