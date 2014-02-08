# This Makefile provide complete core building for UARM Emulator, from cross compiling to .elf to .uarm conversion.
# The PATH of this file must be: /usr/include/uarm/test
# AUTHOR: Agnese Sgarbi 
# DATE: 2014/02/06


# Source file
# Edit FILE for change the C source file you want to use for build the UARM core
# WARNING: it work with only 1 source code file

FILE = helloworld
CFILE = $(FILE).c
OFILE = $(FILE).o
ELFFILE = $(FILE).elf


# Compiling macros 
# Complete command:  arm-none-eabi-gcc -mcpu=arm7tdmi -c -o p1test.o /usr/include/uarm/test/p1test.c

CC = arm-none-eabi-gcc
CO = -mcpu=arm7tdmi -c -o 


# Linking macros
# complete command:  arm-none-eabi-ld -T /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x -o p1test /usr/include/uarm/crtso.o /usr/include/uarm/libuarm.o p1test.o

LD = arm-none-eabi-ld -T ../ldscripts/elf32ltsarm.h.uarmcore.x -o
LFILE = ../crtso.o ../libuarm.o


# Conversion macros
# complete command:  elf2uarm -k test/p1test.elf
CONV = elf2uarm -k

#Building the arm core 

#Convert the .elf file into the core for uarm
p1test : $(ELFFILE)
	@echo "Converting to the UARM core file..."
	$(CONV) $(ELFFILE)


#Linking the precompiled test file with the file necessary for build the .elf file
$(ELFFILE) : $(LFILE) $(OFILE)
	@echo "Linkink necessary file with the obj file..."
	$(LD) $(FILE) $(LFILE) $(OFILE)


#Cross compiling from the C source file into arm architeture
$(OFILE) : $(CFILE)
	@echo "Compiling into ARM architeture..." 
	$(CC) $(CO) $(OFILE) $(CFILE)

#Remove all created file
clear :
	rm $(FILE) $(CFILE) $(OFILE) $(ELFFILE)
