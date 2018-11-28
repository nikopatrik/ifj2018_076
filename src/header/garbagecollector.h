/* Předmět: Algoritmy (IAL) - FIT VUT v Brně
 * Hlavičkový soubor pro c201.c (Jednosměrně vázaný lineární seznam)
 * Vytvořil: Martin Tuček, září 2005
 * Upravil: Kamil Jeřábek, září 2018
 *
 *
 * Tento soubor, prosíme, neupravujte!
 * Please, do not modify this file!
 */

#ifndef _GARBAGE_H_
#define _GARBAGE_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#define TRUE 1
#define FALSE 0
                                           /* Indikace, zda byl příklad řešen */
                                   /* Detects whether the task is implemented */
extern int solved;
                                                            /* Indikace chyby */
                                                                /* Error flag */
extern int errflg;
                                                             /* Prvek seznamu */
                                                /* Definition of list element */
typedef struct tElem {
    struct tElem *ptr;
    void *allocated_buffer;
} *tElemPtr;
                                                            /* Vlastní seznam */
                                                        /* Definition of list */
typedef struct {
    tElemPtr Act;
    tElemPtr First;
} tList;



/*Garbage collector Interface*/
void garbage_collector_init();
void gb_register_pointer(void*);
void* gb_malloc(size_t size);
void* gb_realloc(void*,size_t);
void gb_free(void*);
void gb_release_resources();
void gb_exit_process(int);

                                                   /* Funkce pro implementaci */
void InitList ();
void DisposeList ();
void InsertFirst (void*);
void First ();
void Succ ();
void* Copy ();	
void Actualize (void*);
int  Active ();
void DeleteActive();

/* Konec hlavičkového souboru c201.h */

#endif
