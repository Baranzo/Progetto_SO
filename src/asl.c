#include "asl.h"

semd_t *semdFree_h; // head pointer to semdFree list
semd_t *semd_h; //active sema4 list head pointer
static semd_t semdTable[MAXPROC + 1]; // semaphore descriptors array "+1" for the dummy block

/* File with all the functions related to semaphore */ 

/* Insert of the ProcBLk into the semaphore queue  */
int insertBlocked ( int *semAdd, pcb_t *p ) {
  semd_t *tmp = semd_h;

  /* Find the correct element */
  while( tmp->s_next != NULL && ( ( tmp->s_next )->s_semAdd ) < semAdd ){
    tmp = tmp->s_next;
  }
  
  /* Control if we must create a new semaphore */
  if( tmp->s_next == NULL || ( tmp->s_next )->s_semAdd > semAdd ){
    if( semdFree_h == NULL ){
      return TRUE;
    }
    else{	
      semd_t *add = semdFree_h;
      semdFree_h = semdFree_h->s_next;
      add->s_next = tmp->s_next;	
      tmp->s_next = add;	
      add->s_semAdd = semAdd;
      add->s_procQ = mkEmptyProcQ();
      add->s_procQ = p;
      p->p_next = p;
    }
  }	
  else {
    if ( tmp->s_next->s_semAdd == semAdd ){
      insertProcQ( &((tmp->s_next)->s_procQ ), p);	
    }
  }
  p->p_semAdd = semAdd;
  return FALSE;
}


/* Remove the head of the PCB queue of the given semaphore*/
pcb_t *removeBlocked (int *semAdd){
  semd_t *prec;
  prec = semd_h;
  
  /* Find the correct element */
  while(prec->s_next != NULL && (prec->s_next)->s_semAdd != semAdd){
    prec=prec->s_next;
  }

  if(prec->s_next == NULL){
    return NULL;
  }
  
  /* Coltrol if we find the element and remove it if we did */
  if((prec->s_next)->s_semAdd==semAdd){
    pcb_t *ris , *tp;
    semd_t *tmp;
    tp=(prec->s_next)->s_procQ;
    ris=removeProcQ(&tp);
    if(!emptyProcQ(tp)){
      return ris;
    }
    else{
      tmp=prec->s_next;
      prec->s_next=tmp->s_next;
      tmp->s_next=semdFree_h;
      semdFree_h=tmp;
      return ris;
    }
  }
  return NULL;
}	


/* Remove the procBlc pointed by p */
pcb_t *outBlocked (pcb_t *p){
  semd_t *tmp = semd_h->s_next;
	semd_t *prec = semd_h;

  /* Search for the semaphore descriptor equals to semaaphore value of p */
  while ((tmp->s_semAdd) != (p->p_semAdd) && tmp != NULL){
	prec = tmp;    
	tmp = tmp->s_next;
  }
 
  /* Remove the pcb if we find it */
  if (tmp != NULL){
    pcb_t *tp;
    tp = outProcQ(&(tmp->s_procQ), p);
	if (emptyProcQ(tmp->s_procQ)) {
		prec->s_next = tmp->s_next;
		tmp->s_next= semdFree_h->s_next;
		semdFree_h->s_next = tmp;
	}
    return tp;	
  }
  
  /* */

  return NULL;
}


/* Return a pointer to the Head of the process queue with the same semaphore */
pcb_t *headBlocked (int *semAdd){
  semd_t *tmp = semd_h->s_next;

  /* search for the semaphore descriptor with the same semaphore value */
  while (tmp != NULL && (tmp->s_semAdd) != semAdd ){
    tmp = tmp->s_next;
  }

  /* if we find the right semaphore descriptor and it is not empty, it has a head.  */
  if (tmp != NULL && tmp->s_procQ != NULL){
    return tmp->s_procQ->p_next;
  }
  else
  // maybe there isn't a corresponding sema4 descriptor or maybe there is but it's empty
  return NULL;
}


/* Initialization of the semdFree list */
initASL(){
  int i;
  semd_t *p;
  p = semdFree_h = &semdTable[1];
  semd_h =&semdTable[0];
  
  /* DUMMY Initialization */
  semd_h->s_semAdd = NULL;
  semd_h->s_procQ = NULL;
  semd_h->s_next = NULL;

  /* adding semdTable blocks allocated in the semdFree list */
  for (i = 1; i < MAXPROC + 1; i++){
    if (i < MAXPROC){
      p->s_next = &semdTable[i + 1];
    }
    else{
      p->s_next = NULL;
    }
    p = p->s_next;
  }
}
