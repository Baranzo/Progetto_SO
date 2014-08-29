/*
#include "const.h"
#include "types.h"
#include "../libuarm.h"
*/
#ifndef ASL_H
#define ASL_H


#include "pcb.h"
#include "const.h"
/* Semaphore list handling functions */

typedef struct semd_t{
	struct semd_t *s_next;
	int *s_semAdd;
	pcb_t *s_procQ;
}semd_t;




EXTERN int insertBlocked(int *semAdd, pcb_t *p);
EXTERN pcb_t *removeBlocked(int *semAdd);
EXTERN pcb_t *outBlocked(pcb_t *p);
EXTERN pcb_t *headBlocked(int *semAdd);
EXTERN void initASL();
#endif
