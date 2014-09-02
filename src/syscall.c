 #include "pcb.h"
#include "asl.h"
#include "const.h"
#include "libuarm.h"
#include "uARMconst.h"
#include "arch.h"
#include "scheduler.h"

unsigned int size=sizeof(unsigned int);
void error(char *strp) {
	tprint(strp);
	PANIC();
}
void createProcess(state_t* proc_state)
{	
	
	pcb_t *new_proc,*parent;
	state_t *parent_state;
	parent=currentProcess;
	parent_state=parent->p_s;
	
	new_proc=allocPcb();
	if (new_proc==NULL){
		//non sono riuscito ad allocare il pcb quindi l operazione viene abortita e metto in v0 del padre -1
			parent_state[4*size]=(void *)(-1);
			return;
		}
	
	insertChild(parent,new_proc);
	//copio lo stato del padre nel processo figlio e lo metto nella readyqueue
	new_proc->p_s=parent->p_s;
	insertProcQ(&readyQueue,new_proc);
	//in caso di successo metto in v0 0
	parent_state[4*size]=(void *)(0);
	processCount++;
	return;
}


/* 
 * WARNING this function has to be called only by terminateProcess;
 * input: a process to eliminate
 * output: it's sibling or null if onlychild
 * this function does not kill all the child of the process
 */

pcb_t *terminateSingleProcess(pcb_t *process)
{		
		pcb_t *sib; 
		sib=process->p_sib;
		//makes the process no longer the child of it's parent
		outChild(process);
		//tolgo il processo dalla ready queue
		if(outProcQ(&readyQueue,process)==NULL){
			error("processo non trovato nella readyqueue");
		}
		//aggiorno la lista dei pcb liveri e il process count
		freePcb(process);
		processCount--;
		
		return sib;	
	
}
/*
 * kills a process and all of its child recursively
 */
void terminateProcess()
{
	pcb_t *process;
	process=currentProcess;
	//se ho  dei figli chiamo ricorsivamente questa funzione
	if(emptyChild(process)==FALSE)
	{
		terminateProcess(process->p_child);
	}
	//non ho figli
	if (process!=currentProcess)
	{
		//non sono il processo sul quale e stata fatta la richiesta di kill ma un suo figlio
		while(process!=NULL)
		{
			//uccido tutti i figli
			//nb terminatesingleprocess mi restituisce il fratello del processo ucciso o null
			process=terminateSingleProcess(process);
		}
	}else
	{	
		//termino currentProcess
		terminateSingleProcess(process);
	}
}

void verhogen(state_t *semaphore)
{
	
	(*semaphore)++;
}

void syscallHandler(int sistype)
{

	state_t *tmp, *a1;

	unsigned int sys;
	tmp=(state_t *)SYSBK_OLDAREA;
	
	sys=(int)tmp[0];
	a1=tmp[1*size];
	switch (sys)
	{
		case CREATEPROCESS:
		{
			createProcess(a1);
			break;
		}
		case TERMINATEPROCESS:
		{
			terminateProcess();
			break;
		}
		case VERHOGEN:
		{
			verhogen(a1);
			break;
		}
	}
	
};
