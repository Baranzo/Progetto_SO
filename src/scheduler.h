#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "types.h"
#include "const.h"
#include "pcb.h"

EXTERN pcb_t *readyQueue;
EXTERN pcb_t *currentProcess;
EXTERN void scheduler(int control);

#endif
