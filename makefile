
file_to_compile := p1test pcb asl

core_file := kernel.core.uarm

src_dir_name := src/

file_to_link := $(wildcard $(src_dir_name)*.o)

CC = arm-none-eabi-gcc
CO = -mcpu=arm7tdmi -c -o 

LD = arm-none-eabi-ld -T ldscripts/elf32ltsarm.h.uarmcore.x -o

CONV = elf2uarm -k


objects := $(addprefix $(src_dir_name), $(addsuffix .o,$(file_to_compile)))

compile := $(addprefix $(src_dir_name), $(addsuffix .c,$(file_to_compile)))


# $(foreach var,list,text)



$(objects) : $(compile)
	@echo "Compiling into ARM architeture..." 
	$(CC) $(CO) $@ $<


#print:
#	$(foreach x, $(compile),@echo $(x))
	

#$(objects) : $(compile)
#	
#	$(CC) $(CO) $(objects) $(compile)
