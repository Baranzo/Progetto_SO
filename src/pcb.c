
#include "libuarm.h"
#include "types.h"
#include "pcb.h"

pcb_t *pcbFree_h; //head pointer to pcb_free list


static pcb_t array[MAXPROC]; //procblks array

/* List View Functions */

/* Insert the element pointed by p onto the pcbFree list */
void freePcb(pcb_t *p){

	if (pcbFree_h != NULL){
		/* The list has 1 or more elements */
		p->p_next = pcbFree_h;
		pcbFree_h = p;
	}
	else {
		/* The list is empty */
		p->p_next = NULL;
		pcbFree_h = p;
	}
	
}

/* allocPcb removes an element from the PcbFree list and return a pointer to the removed element;
	return NULL if PcbFree list is empty */
pcb_t *allocPcb (){

	if (pcbFree_h != NULL){
		/* The pcbFree list is not empty */
		pcb_t *p;
		p = pcbFree_h;
		pcbFree_h = p->p_next;
		p->p_next = NULL;
		p->p_prnt = NULL;
		p->p_child = NULL;
		p->p_sib = NULL;
		p->p_s = NULL; 
		p->p_semAdd = NULL;
		return p;
	}
	
	/* If the pcbFree list is empty return NULL */
	return NULL;
}

/* Initialize the pcbFree list to contain all the elements of the static array of MAXPROC ProcBlk's
	This method should be called only once during data structure initialization */
void initPcbs(void){
	int i;
	pcb_t *p;	
	p = pcbFree_h = &array[0]; /* Initialize the head pointer, to pcbFree list, to the first element of the array */
	
	for (i = 0; i < MAXPROC-1; i++) {
		/* Links every element to his successor */
		p->p_next = &array[i + 1];
		p = p->p_next;
	}
	
	/* Makes the list NULL terminated */
	p->p_next = NULL;	
}


/* Process Queue Maintenance */

/* This method is used to initialize a variable to be a tail pointer to a process queue.
	Return NULL */
pcb_t *mkEmptyProcQ(void){
	return NULL;
}

/* Return TRUE if the queue whose tail is pointed to by tp is empty.
	Return FALSE otherwise */
int emptyProcQ(pcb_t *tp){

	if (tp == NULL) {
		return TRUE;
	}
	
	return FALSE;
}

/* Insert the ProcBlk pointed to by p into the process queue whose tail-pointer is pointed to by tp. */
void insertProcQ(pcb_t **tp, pcb_t *p){
	/* The list is circularly linked */
	
	if ((*tp) == NULL) {
		p->p_next = p;
	}
	else {
		p->p_next = (*tp)->p_next;
		(*tp)->p_next = p;
	}
	
	/* Updating the tail-pointer */
	(*tp) = p;
}

/* Remove the head element of the process queue whose tail-pointer is pointed to by tp.
	Return NULL if the queue is initially empty, if not return the pointer to the removed element.
	The tail-pointer might be modified */
pcb_t *removeProcQ(pcb_t **tp){
	pcb_t *p;
	
	if (*tp == NULL) {
		return NULL;
	}
	
	p = (*tp)->p_next;
	
	if (*tp == p) {
		/* If there's only one element update the tail-pointer to NULL */
		*tp = NULL;
		return p;
	}
	
	(*tp)->p_next = p->p_next;
	return p;
}

/* Remove the ProcBlk pointed to by p from the process queue whose tail-pointer is pointed to by tp.
	If the entry is not in the queue return NULL otherwise return p */
pcb_t *outProcQ(pcb_t **tp, pcb_t *p){
	pcb_t *tmp;
	tmp = (*tp);
	
	while (tmp->p_next != (*tp)) {
		/* If p is found remove it otherwise jump to the next element */
		if (tmp->p_next == p) {		
			tmp->p_next = p->p_next;
			p->p_next = NULL;
			return p;
		}
		else {		
			tmp = tmp->p_next;			
			}
			
	}
	
	/* If there is only one element in the list */
	if (tmp->p_next == p) {		
		*tp = NULL;
		p->p_next = NULL;		
		return p;
	}
	
	return NULL;	
}
/* Return the head to the list whose tail is pointed to by tp */
pcb_t *headProcQ(pcb_t *tp){

	if (tp == NULL) {
		return NULL;
	}
	
	return tp->p_next;
}

/* Process Tree Maintainance */

/* Return TRUE if the ProcBlk pointed to by p has no children. Return FALSE otherwise. */
int emptyChild(pcb_t *p){

	if (p->p_child == NULL) {
		return TRUE;
	}
	
	return FALSE;
}

/* Make the ProcBlk pointed to by p a child of the ProcBlk pointed to by prnt */
void insertChild(pcb_t *prnt, pcb_t *p){
    p->p_sib = prnt->p_child; /* Update the siblings */
    prnt->p_child = p;
    p->p_prnt = prnt;
}

/* Remove the firs child of the ProcBlk pointed to by p */
pcb_t *removeChild(pcb_t *p){

	if (!emptyChild(p)) {
		pcb_t *temp;
		temp = p->p_child;
		p->p_child = temp->p_sib;
		/* Update the pointers of the removed child */
		temp->p_prnt = NULL;
		temp->p_sib = NULL;
		return temp;
	}
	
	return NULL;
}

/* Make the ProcBlk pointed to by p no longer a child of its parent.
	If p has no parent return NULL; otherwise return p */
pcb_t *outChild(pcb_t *p){

	if (p->p_prnt != NULL){
		
		/* If procBlk p is the firs child, we can use removeChild function */
		if ((p->p_prnt)->p_child == p ) {
			return removeChild(p->p_prnt);
		}
		else {
			/* Otherwise we'll search it in the child list */
			pcb_t *tmp = (p->p_prnt)->p_child;
			
			while (tmp->p_sib != p) {
				tmp = tmp->p_sib;
			}
			
			tmp->p_sib = p->p_sib;
			p->p_prnt = NULL;
			p->p_sib = NULL;
			return p;
		}
		
	}
	
	// if procBlk p has no parent, NULL is returned
    return NULL;
}
