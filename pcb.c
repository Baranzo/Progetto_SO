#include "pcb.h"
/* pcbFree_h punta alla testa della lista pcbFree */


/* >>>>> Funzione 1 */
/* >>>>>>>>>>>>>>>>>>>>>>> */

/* inserisce l'elemento puntato da p nella lista freePcb, in coda */
void freePcb (pcb_t *p){
   
  if (pcbFree_h != NULL){
    p->next = pcbFree_h->next;
    (pcbFree_h->next)->prev = p;
    p->prev = pcbFree_h;
    pcbFree_h->next = p;
  }

  else {
    p->next = p;
    p->prev = p;
    pcbFree_h = p;
  }
}

/* >>>>> Funzione 2 */
/* >>>>>>>>>>>>>>>>>>>>> */

pcb_t *allocPcb (){

  if (pcbFree_h != NULL){
    pcb_t *p;
   
    /* Rimozione del ProcBlk */ 
    p=pcbFree_h;
    pcbFree_h = pcbFree_h->prev;
    pcbFree_h->next = p->next;
    (pcbFree_h->next)->prev = pcbFree_h;
    
    /* ProcBlk rimosso inizializzato*/
    
/* ????? (ho un dubbio sull'inizializzazione di questi due campi: vanno  NULL o considerati come una lista di un solo elemento? */
/* Non dovrebbe cambiare nulla, perche' tanto appena viene inserito in una delle liste, questi due campi vengono sicuramente riscritti, quindi e' indifferente ai fini della funzionalita' IMHO. Lascio l'interrogativo per quanto riguarda lo "stile"*/
    p->prev = NULL;
    p->next = NULL;
    /*     */
      
    p->p_prnt = NULL;
    p->p_child = NULL;
    p->p_sib = NULL;
    p->p_s = 0;
    p->p_semAdd = NULL;
    
    return p;
  } 
  
  return NULL;
}

/* >>>>> Funzione 3 */
/* >>>>>>>>>>>>>>>>>> */

initPcbs(){

  static pcb_t array[MAXPROC];

  /* inizializzazione lista pcbFree */
  for (int i = 0; i < MAXPROC; i++){
        
    if (i > 0 && i < MAXPROC-1){
      array[i]->prev = array[i+1];
      array[i]->next = array[i-1];
    }
    else if (i == 0){
      array[i]->prev = array[i+1];
      array[i]->next = array[MAXPROC-1];
    }
    else if (i == MAXPROC){
      array[i]->prev = array[0];
      array[i]->next = array[i-1];
    }
  }
  
  *pcbFree_h = &array[0];
}


/* >>>>>> Funzione 4* */
/* >>>>>>>>>>>>>>>>>>>>>> */
/* ----> rendere la lista doppia? <----- */

/* Return TRUE if the ProcBlk pointed to by p has no children. Return FALSE otherwise */

int emptyChild (pcb_t *p){
  
  /* 1 = TRUE, 0 = FALSE */
  if (p->p_child == NULL) return 1;
  else return 0;
}

/* >>>>> Funzione 5* */
/* >>>>>>>>>>>>>>>>>> */

insertChild (pcb_t *prnt, pcb_t *p){
  /* Make the ProcBlk pointed to by p a child of the ProcBlk pointed to by prnt */
  if (emptyChild){
    prnt->p_child = p;
    p->p_prnt = prnt;
  }
  else { /* QUI ci starebbe la doppia lista volendo */
    /* PRIMO METODO: INSERZIONE IN TESTA */
    p->p_sib = prnt->p_child
    prnt->p_child = p;
    p->p_prnt = prnt;

    /* SECONDO METODO: INSERZIONE IN CODA */
    // pcb_t *tmp = prnt->p_child;
    // while (tmp->p_sib != NULL) tmp = tmp->p_sib;
    // tmp->p_sib = p;
    // p->p_prnt = prnt;
  }
}

/* >>>>>Funzione 6* */
/* >>>>>>>>>>>>>>>> */

pcb_t *removeChild (pcb_t *p){
  /* Make the first child of the ProcBlk pointed to by p no longer a child of p. Return NULL if initially there were no children of p. Otherwise, return a pointer to this removed firse child ProcBlk */

  if (!emptyChild){ // Non superfluo altrimenti "p->p_child = tmp->p_sib" da seg fault
    pcb_t *tmp = p->p_child;
    p->p_child = tmp->p_sib; //or p->p_child = (p->p_child)->p_sib
    
    tmp->p_prnt = NULL;
    tmp->p_sib = NULL;
    
    return tmp;  
  }
  
  return NULL;
}

/* >>>>> Funzione 7* */
/* >>>>>>>>>>>>>>>> */

pcb_t *outChild (pcb_t *p){
  /* Make the ProcBlk pointed to by p no longer a child of its parent. If the procBlk pointed to by p has no parent, return NULL. Otherwise, return p. Note that the element pointed to by p need not be the first child of its parent */


  //QUI e' davvero comoda la doppia lista
  if (p->p_prnt != NULL){
    /* LISTA UNIDIREZIONALE */
    if ( (p->p_prnt)->p_child == p ) return removeChild(p->p_prnt);
    else {
      pcb_t *tmp = (p->p_prnt)->p_child;
      while (tmp->p_sib != p) tmp = tmp->p_sib;
      tmp->p_sib = p->p_sib;
      p->p_prnt = NULL;

      return p;

/* LISTA BIDIREZIONALE */
      // if /* ... */
      // else
      // (p->PREV-SIB)->NEXT-SIB = p->NEXT-SIB;
      // (p->NEXT-SIB)->PRV-SIB = p->PREV-SIB;
      // return p;

}

  return NULL;

}

