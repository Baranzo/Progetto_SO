//`#include "pcb.h"
#include "const.h"
#include "libuarm.h"
#include "types.h"
#include "asl.h"
//head pointer to pcb_free list
pcb_t *pcbFree_h;

/* List view functions */

void freePcb(pcb_t *p){
	pcb_t *app;
	app=pcbFree_h;
	while (app->p_next != NULL) {
		app=app->p_next;
	}
	app->p_next=p;
	p->next=NULL;
}
pcb_t *allocPcb(void){
	pcb_t app;
};
/*EXTERN pcb_t *allocPcb(void);
EXTERN void initPcbs(void);
*/
//EXTERN pcb_t *mkEmptyProcQ(void);
/*EXTERN int emptyProcQ(pcb_t *tp);
EXTERN void insertProcQ(pcb_t **tp, pcb_t *p);
EXTERN pcb_t *removeProcQ(pcb_t **tp);
EXTERN pcb_t *outProcQ(pcb_t **tp, pcb_t *p);
EXTERN pcb_t *headProcQ(pcb_t *tp);
*/
/* Tree view functions */
/*
void initASL(void);
int emptyChild(pcb_t *p);
void insertChild(pcb_t *prnt, pcb_t *p);
pcb_t *removeChild(pcb_t *p);
pcb_t *outChild(pcb_t *p);
*/
