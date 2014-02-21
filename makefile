# This Makefile provide complete core building for UARM Emulator, from cross compiling to .elf to .uarm conversion.
# The PATH of this file must be: /usr/include/uarm/test
# AUTHOR: Agnese Sgarbi 
# DATE: 2014/02/06


# Source file
# Edit FILE for change the C source file you want to use for build the UARM core
# WARNING: it work with only 1 source code file

FILE = p1test
FILEPCB = pcb
CFILE = $(FILE).c
CFILEPCB = $(FILEPCB).c
OFILE = $(FILE).o
OFILEPCB = $(FILEPCB).o
CORE = $(FILE).core.uarm

# Compiling macros 
# Complete command:  arm-none-eabi-gcc -mcpu=arm7tdmi -c -o p1test.o /usr/include/uarm/test/p1test.c

CC = arm-none-eabi-gcc
CO = -mcpu=arm7tdmi -c -o 


# Linking macros
# complete command:  arm-none-eabi-ld -T /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x -o p1test /usr/include/uarm/crtso.o /usr/include/uarm/libuarm.o p1test.o

LD = arm-none-eabi-ld -T ../ldscripts/elf32ltsarm.h.uarmcore.x -o
LFILE = ../crtso.o ../libuarm.o $(OFILEPCB)


# Conversion macros
# complete command:  elf2uarm -k test/p1test.elf
CONV = elf2uarm -k

#Building the arm core 

#Convert the .elf file into the core for uarm
$(CORE): $(FILE)
	@echo "Converting to the UARM core file..."
	$(CONV) $(FILE)


#Linking the precompiled test file with the file necessary for build the .elf file
$(FILE) : $(LFILE) $(OFILE)
	@echo "Linkink necessary file with the obj file..."
	$(LD) $(FILE) $(LFILE) $(OFILE)

#Cross compiling from the C source file pcb.c into arm architeture
$(OFILEPCB) : $(CFILEPCB)	
	@echo "Compiling pcb into arm architeture"
	$(CC) $(CO) $(OFILEPCB) $(CFILEPCB)

#Cross compiling from the C source file into arm architeture
$(OFILE) : $(CFILE)
	@echo "Compiling into ARM architeture..." 
	$(CC) $(CO) $(OFILE) $(CFILE)

#Remove all created file
cleanall :
	rm  $(OFILE) $(FILE).stab.uarm $(FILE).core.uarm 
