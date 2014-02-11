//`#include "pcb.h"
#include "const.h"
#include "libuarm.h"
#include "types.h"
#include "asl.h"
//head pointer to pcb_free list
pcb_t *pcbFree_h;
static pcb_t array[MAXPROC]; //array dei procblk

/* List view functions */

void freePcb(pcb_t *p){
	pcb_t *app;
	app=pcbFree_h;
	//scorro fino a fine lista
	while (app->p_next != NULL) {
		app=app->p_next;
	}
	//aggiungo p in coda
	app->p_next=p;
	p->p_next=NULL;
	
}
pcb_t *allocPcb (){
	//se la lista non e' vuota allora restituisco in pcb con i valori inizializzati
	if (pcbFree_h != NULL){
		pcb_t *p;
		p=pcbFree_h;
		pcbFree_h->p_next = p->p_next;
		p->p_next = NULL;
		p->p_prnt = NULL;
		p->p_child = NULL;
		p->p_sib = NULL;
		//p->p_s = ??;
		//p->p_semAdd = ??;
		return p;
	} 
	//se la lista e' vuota ritorno NULL
	return NULL;
}

void initPcbs(void){
	int i;
	pcb_t *p;
	for (i = 0; i < MAXPROC; i++){
		if (i==0){
			pcbFree_h = &array[i];
			p=pcbFree_h;
		}
		if (i < MAXPROC-1){
			p->p_next = &array[i];
		}else{
			p->p_next = NULL;
		}
		p=p->p_next;
	}
}

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
	p->p_next=(*tp)->p_next;
	(*tp)->p_next=p;
	*tp=p;
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
pcb_t *outProcQ(pcb_t **tp, pcb_t *p){
	if( *tp ==NULL){
		return NULL;
	}
	
	pcb_t *tmp;
	tmp=p;
	while (tmp->p_next!=p){
		tmp=tmp->p_next;
	}
	tmp->p_next=p->p_next;
	if( p == *tp){
		*tp=tmp;
	}
	return p;
}

pcb_t *headProcQ(pcb_t *tp){
	if (tp=NULL){
		return NULL;
	}
	return tp->p_next;
}
//tree functions
int emptyChild(pcb_t *p){
	if (p->p_child == NULL) { 
			return TRUE;
	}
	return FALSE;
}

/*
void initASL(void);
*/
void insertChild(pcb_t *prnt, pcb_t *p){
//non serve controllare che prnt->p_child != NULL perche' al piu' assegniamo a
//p->p_sib il valore NULL
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
/*pcb_t *outChild(pcb_t *p);
*/
