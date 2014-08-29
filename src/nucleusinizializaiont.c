/*NUCLEUS INIZIALIZAITION
 * 
 */
#include "pcb.h"
#include "asl.h"
#include "const.h"
#include "libuarm.h"
#include "uARMconst.h"
#include "arch.h"
#include "scheduler.h"
#include "syscall.h"
//usero RAM_TOP definito da arch.h 
#define RAMTOP 0x2D4

EXTERN void test();
//nucleus variables
pcb_t *readyQueue, *currentProcess;
int processCount,softBlockCount;
//semafori
int semTape0,semDisk0,semNetwork0,semPrinter0,semTerminalRead0,semTerminalWrite0;
void interruptHandler(){}
void TLBHandler(){}
void PGMTHandler(){}


int main(void)
{
	state_t *tmp,proc;
	pcb_t *process0;
	int size;
	unsigned int status,process_status;
	size=sizeof(unsigned int);
	//credo che sia il new da settare ma non sono sicura
	//interrupt new
	tmp=(state_t*)INT_NEWAREA;
	//pc
	tmp[15*size]=&interruptHandler;
	//sp
	tmp[13*size]=(void*)RAM_TOP;
	
	//TLB new
	tmp=(state_t*)TLB_NEWAREA;
	
	tmp[15*size]=&TLBHandler;
	tmp[13*size]=(void*)RAM_TOP;
	
	//PGMT new
	tmp=(state_t*)PGMTRAP_NEWAREA;
	
	tmp[15*size]=&PGMTHandler;
	tmp[13*size]=(void*)RAM_TOP;
	
	//syscall new
	tmp=(state_t*)SYSBK_NEWAREA;
	
	tmp[15*size]=&syscallHandler;
	tmp[13*size]=(void*)RAM_TOP;
	
	//status register inizialization
	status=getSTATUS();
	//disabilito tutti gli interrupt
	STATUS_ALL_INT_DISABLE(status);	
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
	
	//initialize all nucleus mantained semaphores, possono esserci piu tape,disk,printer e terminal.
	//puo essere da espandere
	//vanno tutti inizializzati a 0 da indicazioni del pdf
	semTape0=0;
	semDisk0=0;
	semNetwork0=0;
	semPrinter0=0;
	//send e recieve su un terminale sono concorrenti
	semTerminalRead0=0;
	semTerminalWrite0=0;
	
	//istanzio il primo processo
	process0=allocPcb();
	process_status=(int)process0->p_s;
	
	STATUS_ALL_INT_ENABLE(process_status);
	STATUS_ENABLE_TIMER(process_status);
	
	//virtual memory e sempre disabilitata perche non ancora completamente implementata
	STATUS_ENABLE_TIMER(process_status);
	
	//kernel mode
	//pulisco i bit dello stato del processore
	process_status &= STATUS_CLEAR_MODE;
	//setto i bit a sys ovvero kernel mode
	process_status |= STATUS_SYS_MODE;
	
	process0->p_s=(state_t)process_status;
	//sp
	STST(&proc);
	tmp=&proc;
	tmp[13*size]=(void*)(RAM_TOP-FRAMESIZE);
	//metto PC all indirizzo della funzione test()
	tmp[15*size]=&test;
	LDST(&proc);
	
	
	//metto il processo in ready queue
	readyQueue=mkEmptyProcQ();
	insertProcQ(&readyQueue, process0);
	
	scheduler(1);
	
	
	return 0;
}
