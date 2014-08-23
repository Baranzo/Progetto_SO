/*NUCLEUS INIZIALIZAITION
 * 
 */
#include "pcb.h"
#include "asl.h"
#include "const.h"
#include "libuarm.h"

#define RAMTOP 0x2D4
#define STATUS_CLEAR_M_I_F_BITS 0xFFFFFF20 

//nucleus variables
pcb_t *readyQueue, *currentProcess;
int processCount,softBlockCount;

void interruptHandler(){}
void TLBHandler(){}
void PGMTHandler(){}
void syscallHandler(){}

int main(void)
{
	state_t *tmp;
	int size;
	size=sizeof(unsigned int);
	//credo che sia il new da settare ma non sono sicura
	//interrupt new
	tmp=(state_t*)0x7080;
	
	tmp[15*size]=&interruptHandler;
	tmp[13*size]=(void*)RAMTOP;
	
	//TLB new
	tmp=(state_t*)0x7180;
	
	tmp[15*size]=&TLBHandler;
	tmp[13*size]=(void*)RAMTOP;
	
	//PGMT new
	tmp=(state_t*)0x7280;
	
	tmp[15*size]=&PGMTHandler;
	tmp[13*size]=(void*)RAMTOP;
	
	//syscall new
	tmp=(state_t*)0x7380;
	
	tmp[15*size]=&syscallHandler;
	tmp[13*size]=(void*)RAMTOP;
	
	//status register inizialization
	unsigned int status,value;
	status=getSTATUS();
	//metto a zero i irq fiq e status bits
	status &= STATUS_CLEAR_M_I_F_BITS;
	//ora li setto al valore desiderato
	value=0xDF;
	//fast interrupt e interrupt settati a 1 cosi sono disabilitati, credo
	//i 5 bit meno significativi sono impostati a 0x1F  ovvero system/kernel mode
	status|=value;
	setSTATUS(status);
	//inizializzazione delle strutture del livello 2
	initPcbs();
	initASL();
	//inizialize nucleus mantained variables process count soft block count ready queue current process
	processCount=0;
	softBlockCount=0;
	readyQueue=NULL;
	currentProcess=NULL;
	
	
	return 0;
}
