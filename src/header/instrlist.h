
/* Předmět: Algoritmy (IAL) - FIT VUT v Brně
 * Hlavičkový soubor pro c206.c (Dvousměrně vázaný lineární seznam)
 * Vytvořil: Martin Tuček, září 2005
 * Upravil: Kamil Jeřábek, září 2018
 *
 * Tento soubor, prosíme, neupravujte!
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FALSE 0
#define TRUE 1

extern int errflg;
extern int solved;

typedef struct tDLElem {                 /* prvek dvousměrně vázaného seznamu */
        char *instruction;                                            /* užitečná data */
        struct tDLElem *lptr;          /* ukazatel na předchozí prvek seznamu */
        struct tDLElem *rptr;        /* ukazatel na následující prvek seznamu */
} *tDLElemPtr;

typedef struct {                                  /* dvousměrně vázaný seznam */
    tDLElemPtr First;                      /* ukazatel na první prvek seznamu */
    tDLElemPtr Act;                     /* ukazatel na aktuální prvek seznamu */
    tDLElemPtr Last;                    /* ukazatel na posledni prvek seznamu */
} tDLList;

                                             /* prototypy jednotlivých funkcí */
void DLInitList (tDLList *);
void DLDisposeList (tDLList *);
void DLPostInsertList (tDLList *, tDLList *);
void DLPreInsertList (tDLList *, tDLList *);
void DLInsertFirst (tDLList *, char *);
void DLInsertLast(tDLList *, char *);
void DLFirst (tDLList *);
void DLLast (tDLList *);
void DLCopyFirst (tDLList *, char **);
void DLCopyLast (tDLList *, char **);
void DLDeleteFirst (tDLList *);
void DLDeleteLast (tDLList *);
void DLPostDelete (tDLList *);
void DLPreDelete (tDLList *);
void DLPostInsert (tDLList *, char *);
void DLPreInsert (tDLList *, char *);
void DLCopy (tDLList *, char **);
void DLActualize (tDLList *, char *);
void DLSucc (tDLList *);
void DLPred (tDLList *);
int DLActive (tDLList *);
void DLPrintList (tDLList *);

/* Konec hlavičkového souboru c206.h */
