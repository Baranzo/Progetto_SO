 #include "pcb.h"
#include "asl.h"
#include "const.h"
#include "libuarm.h"
#include "uARMconst.h"
#include "arch.h"
#include "scheduler.h"

unsigned int size=sizeof(unsigned int);

void create_process(state_t* proc_state)
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
	return;
}
	
void syscallHandler(int sistype)
{
	state_t *tmp, *proc_state;

	unsigned int sys;
	tmp=(state_t *)SYSBK_OLDAREA;
	
	sys=(int)tmp[0];
	switch (sys)
	{
		case 1:
		{
			//unsigned int proc_state;
			proc_state=tmp[1*size];
			create_process(proc_state);
			break;
		}
	}
	
};
