//`#include "pcb.h"
#include "const.h"
#include "/usr/include/uarm/libuarm.h"
#include "types.h"
#include "asl.h"

typedef struct semd_t{
	struct semd_t *s_next;
	int *s_semAdd;
	pcb_t *s_procQ;
}semd_t;

semd_t *semd_h;

pcb_t *pcbFree_h; //head pointer to pcb_free list
semd_t *semdFree_h; // head pointer to semdFree list
static pcb_t array[MAXPROC]; //procblks array
static semd_t semdTable[MAXPROC + 1]; // semaphore descriptors array
				     //"+1" because of the dummy block

/* List View Functions */

void freePcb(pcb_t *p){
	
	 if (pcbFree_h != NULL){
	    p->p_next = pcbFree_h;
	    pcbFree_h = p;
	  }
	  else {
	    p->p_next = NULL;
	    pcbFree_h = p;
	  }
}


pcb_t *allocPcb (){
	//se la lista non e' vuota allora restituisco in pcb con i valori inizializzati
	if (pcbFree_h != NULL){
		pcb_t *p;
		p = pcbFree_h;
		pcbFree_h = p->p_next;
		p->p_next = NULL;
		p->p_prnt = NULL;
		p->p_child = NULL;
		p->p_sib = NULL;
		//p->p_s = NULL; Poi ci penseremo in fase 2 ad inizializzarlo
		p->p_semAdd = NULL;
		return p;
	} 
	//se la lista e' vuota ritorno NULL
	return NULL;
}

void initPcbs(void){
	int i;
	pcb_t *p;
	
	p = pcbFree_h = &array[0];
	
	for (i = 0; i < MAXPROC; i++){
		if (i < MAXPROC-1)
			p->p_next = &array[i + 1];
		else
			p->p_next = NULL;
		p = p->p_next;
	}
}


/* Process Queue Maintainance */

pcb_t *mkEmptyProcQ(void){
	pcb_t *tp;
	tp = NULL;
	return tp;
}


int emptyProcQ(pcb_t *tp){
	if (tp == NULL){
		return TRUE;
	}
	return FALSE;
}


void insertProcQ(pcb_t **tp, pcb_t *p){
		if ((*tp) == NULL)
	{
		(*tp)=p;
		p->p_next=p;
	}
	else
	{
		p->p_next=(*tp)->p_next;
		(*tp)->p_next=p;
		(*tp)=p;
	}
}

pcb_t *removeProcQ(pcb_t **tp){
	pcb_t *p;
	if (*tp == NULL){
		return NULL;
	}
	p=(*tp)->p_next;
	if (*tp == p){
		*tp=NULL;
		return p;
	}
	(*tp)->p_next = p->p_next;
	return p;
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
pcb_t *outProcQ(pcb_t **tp, pcb_t *p){
pcb_t *tmp;
	tmp=(*tp);

	while(tmp->p_next!=(*tp)){
	
		if (tmp->p_next==p){
			
			tmp->p_next=p->p_next;
			return p;
		}else{
			
			tmp=tmp->p_next;
		
		}
		
	}
	
	if(tmp->p_next==p){
	
		*tp=tmp;
		tmp->p_next=p->p_next;
		return p;
	}
	
	return NULL;		
}

pcb_t *headProcQ(pcb_t *tp){
	if (tp == NULL){
		return NULL;
	}
	return tp->p_next;
}

/* Process Tree Maintainance */

int emptyChild(pcb_t *p){
	if (p->p_child == NULL) { 
			return TRUE;
	}
	return FALSE;
}


void insertChild(pcb_t *prnt, pcb_t *p){
    p->p_sib = prnt->p_child;
    prnt->p_child = p;
    p->p_prnt = prnt;
}

pcb_t *removeChild(pcb_t *p){
	if(!emptyChild(p)){
		pcb_t *temp;
		temp=p->p_child;
		p->p_child=temp->p_sib;
		temp->p_prnt=NULL;
		temp->p_sib=NULL;
		return(temp);
	}
	return NULL;
}

pcb_t *outChild(pcb_t *p){
  if (p->p_prnt != NULL){
  	// if procBlk p is the firs child, we can use removeChild function
    if ( (p->p_prnt)->p_child == p ) 
    	return removeChild(p->p_prnt);
    	// otherwise we'll have to search it in the child list
    else {
      pcb_t *tmp = (p->p_prnt)->p_child;
      while (tmp->p_sib != p) 
      	tmp = tmp->p_sib;
      tmp->p_sib = p->p_sib;
      p->p_prnt = NULL;
      p->p_sib = NULL;
      return p;
    }
  }
  // if procBlk p has no parent, NULL is returned
    return NULL;
}


/* Active Semaphore List */

int insertBlocked (int *semAdd, pcb_t *p) {
	semd_t *tmp = semd_h;

	// search for the sema4 descriptor with the correct sema4 value
	while( tmp->s_next != NULL && *((tmp->s_next)->s_semAdd) < *semAdd)
		tmp = tmp->s_next;

	// if we cannot find the corresponding we have to allocate a new block from the semdFree list
	if ( tmp->s_next == NULL || *((tmp->s_next)->s_semAdd) != *semAdd ){
		semd_t *app = semdFree_h;
		
		// but only if the semdFree list is not empty
		if (semdFree_h == NULL) 
			return TRUE;
		else 
			semdFree_h = semdFree_h->s_next;

		app->s_next = tmp->s_next;
		tmp->s_next = app;
		app->s_semAdd = semAdd;
		app->s_procQ = mkEmptyProcQ();
	}
	
	//if we found the correct sema4 descriptor, we can add procBlk p to that list upgrading its p_semAdd value
	tmp = tmp->s_next;
	p->p_next = (tmp->s_procQ)->p_next;
	(tmp->s_procQ)->p_next = p;
	tmp->s_procQ = p;
	p->p_semAdd = semAdd;
	
	// return FALSE in any case exept in case semdFree list is empty and we need to allocate a new semd block
	return FALSE;
}


pcb_t *removeBlocked (int *semAdd){
	semd_t *tmp = semd_h;
	//search for the sema4 descriptor with the correct sema4 value
	while (tmp->s_next != NULL && (tmp->s_next)->s_semAdd != semAdd)
		tmp = tmp->s_next;

	// if we can find the corresponding we can remove the head of the list 
	if (tmp->s_next != NULL && (tmp->s_next)->s_semAdd == semAdd){
		semd_t *rem = tmp->s_next;
		pcb_t *p;
		
		
		// p = removeProcQ (&(rem->s_procQ));
		

		p = (rem->s_procQ)->p_next;
		rem->s_procQ->p_next = p->p_next;
		p->p_next = p;
	
		//and in case that semd list becomes empty, we can deallocate it
		if ( emptyProcQ (rem->s_procQ) ){
			rem->s_procQ = NULL;
			tmp->s_next = rem->s_next;
			rem->s_next = semdFree_h->s_next;
			semdFree_h->s_next = rem;
		}
		
		// return the pointer to the procBlk we was able to remove from the list
		return p;
	}
	else
		// return NULL if we weren't able to find the right sema4 descriptor block
		return NULL;
	
}


pcb_t *outBlocked (pcb_t *p){ 
	semd_t *tmp = semd_h;
	//search for the sema4 descriptor with the same sema4 value as p
	while (tmp != NULL && *(tmp->s_semAdd) != *(p->p_semAdd) )
		tmp = tmp->s_next;
	
	// if we found the corresponding sema4 descriptor we can search for p and remove it from the list
	if (tmp != NULL){
		pcb_t *search = tmp->s_procQ;
	
		while ( search->p_next != p || search->p_next != tmp->s_procQ )
			search = search->p_next;
	// if we can find p in the sema4 descriptor's process queue, we can remove it
		if (search->p_next == p)
		
		// ---->>> Non sono convinto sia corretto l'uso della funzione: a mente fresca ci riguardo, 
		// ma non datelo per funzionante
			return removeProcQ (&search->p_next);
			
	// return NULL in either case we cannot find the right sema4 descriptor or p procBlk in the right sema4 descriptor list
		return NULL;
	}
	return NULL;
}


pcb_t *headBlocked (int *semAdd){
	semd_t *tmp = semd_h;
	
	// search for the sema4 descriptor with the correct sema4 value
	while (tmp != NULL && *(tmp->s_semAdd) != *semAdd )
		tmp = tmp->s_next;

	// if we can find the right sema4 descriptor and it is not empty, it has a head. Let's find it!
	if (tmp != NULL && tmp->s_procQ != NULL){
		pcb_t *search = tmp->s_procQ;

		while(search->p_next != tmp->s_procQ)
			search = search->p_next;

		return search;
	}
	// maybe there isn't a corresponding sema4 descriptor or maybe there is but it's empty
	return NULL;
}



initASL(){
	int i;
	semd_t *p;

	p = semdFree_h = &semdTable[0];
	
	// DUMMY Initialization
	semdFree_h->s_semAdd = NULL;
	semdFree_h->s_procQ = NULL;

	// adding semdTable blocks allocated in the semdFree list
	for (i = 0; i < MAXPROC + 1; i++){
		if (i < MAXPROC)
			p->s_next = &semdTable[i + 1];
		else	
			p->s_next = NULL;
		p = p->s_next;
	}
}
