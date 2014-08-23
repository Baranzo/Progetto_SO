/*NUCLEUS INIZIALIZAITION
 * 
 */
#include "pcb.h"
#include "asl.h"
#include "const.h"
#include "libuarm.h"
#include "uARMconst.h"

#define RAMTOP 0x2D4


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
	//pc
	tmp[15*size]=&interruptHandler;
	//sp
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
	unsigned int status;
	status=getSTATUS();
	//disabilito interrupt
	STATUS_DISABLE_INT(status);
	//disabilito gli interrupt del timer (fast interrupt)
	STATUS_DISABLE_TIMER(status);	
	//pulisco i bit dello stato del processore
	status &= STATUS_CLEAR_MODE;
	//setto i bit a sys ovvero kernel mode
	status |= STATUS_SYS_MODE;
	//riscrivo sul registro
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
