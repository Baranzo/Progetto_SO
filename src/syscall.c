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
	state_t parent_state;
	parent=currentProcess;
	parent_state=parent->p_s;
	new_proc=allocPcb();
	if (new_proc==NULL){
			(int*)*parent_state[4*size]=-1;
			return;
		}
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
