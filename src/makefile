# This Makefile provide complete core building for UARM Emulator, from cross compiling to .elf to .uarm conversion
# AUTHOR: Agnese Sgarbi 
# DATE: 2014/02/06


# Source file
# Edit FILE* for change the C source file you want to use for build the UARM core


FILE = p1test
FILEPCB = src/pcb
FILEASL = src/asl

CFILE = src/$(FILE).c
CFILEPCB = $(FILEPCB).c
CFILEASL = $(FILEASL).c

OFILE = src/$(FILE).o
OFILEPCB = $(FILEPCB).o
OFILEASL = $(FILEASL).o

CORE = $(FILE).core.uarm

# Compiling macros 

CC = arm-none-eabi-gcc
CO = -mcpu=arm7tdmi -c -o 


# Linking macros

LD = arm-none-eabi-ld -T ldscripts/elf32ltsarm.h.uarmcore.x -o
LFILE = src/crtso.o src/libuarm.o $(OFILEPCB) $(OFILEASL) $(OFILE)


# Conversion macros

CONV = elf2uarm -k


#Building the arm core 

#Convert the .elf file into the core for uarm
$(CORE): $(FILE)
	@echo "Converting to the UARM core file..."
	$(CONV) $(FILE)

#Linking the precompiled test file with the file necessary for build the .elf file
$(FILE) : $(LFILE) $(OFILE)
	@echo "Linkink necessary file with the obj file..."
	$(LD) $(FILE) $(LFILE) 


#CROSS COMPILING PHASE:

#Cross compiling from the C source file asl.c into arm architeture
$(OFILEASL) : $(CFILEASL)
	@echo "Compiling asl into ARM architeture..." 
	$(CC) $(CO) $(OFILEASL) $(CFILEASL)

#Cross compiling from the C source file pcb.c into arm architeture
$(OFILEPCB) : $(CFILEPCB)	
	@echo "Compiling pcb into ARM architeture"
	$(CC) $(CO) $(OFILEPCB) $(CFILEPCB)

#Cross compiling from the C source file into arm architeture
$(OFILE) : $(CFILE)
	@echo "Compiling p1test into ARM architeture..." 
	$(CC) $(CO) $(OFILE) $(CFILE)

#Remove all created file
cleanall :
	rm  $(OFILE) $(OFILEASL) $(OFILEPCB)  *.uarm $(FILE)
