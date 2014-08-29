#include "pcb.h"
#include "asl.h"
#include "const.h"
#include "libuarm.h"
//inizializzate dal nucleus inizialization in realta

int processCount=1,softBlockCount=1;

//control == 1 finito il time slice == 0 il processo ha finito ci sara un registro dove leggerlo lo scopriro un giorno
//control ==9
void scheduler(int control)
{
	if (readyQueue == NULL)
	{
		if (processCount == 0) HALT();
		if ((processCount >0) && (softBlockCount == 0)) PANIC();
		if ((processCount >0) && (softBlockCount > 0)) WAIT();
	}
	/*se tutto va bene faccio andare il prossimo processo.
	 * salvo il process statedel processo attualle e
	 * cambio il process state?
	 */
	 //save the processor state of the process
	STST(&(currentProcess->p_s));
	if (control == 0)
	{
		//il processo e terminato, devo mettere il suo pcb nella lista dei pcb liberi
		freePcb(currentProcess);
		//prendo il nuovo processo da mettere sul processore
		currentProcess=removeProcQ(&readyQueue);
		processCount--;
		
		//aggiorna process state
	}
		
	if (control == 1)
	////// ci sono le funzioni gia fatte probabilmente, controlla. intanto cosi dovrebbe andare pero
	{
		//time slice ended, back to ready queue
		insertProcQ(&readyQueue,currentProcess);
		//get new process to execute
		currentProcess=removeProcQ(&readyQueue);
		//manca load di nuovo processor state
	}
	if(control == 2)
	{
		//in caso di inputoutput 
		//sviluppare dopo aver fatto le syscall per questo interrupt.
		//ricorda di aggiornare il softblockcount
	}
	if (control == 9){
		currentProcess=removeProcQ(&readyQueue);
	}
	//modifico lo stato del processore con quello del nuovo processo caricato
	LDST(&(currentProcess->p_s)); 

}
