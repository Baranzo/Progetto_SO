/*NUCLEUS INIZIALIZAITION
 * 
 */
#include "pcb.h"
#include "asl.h"
#include "const.h"
#include "libuarm.h"

#define RAMTOP 0x2D4

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
	unsigned int status;
	status=getSTATUS();
	
	return 0;
}
