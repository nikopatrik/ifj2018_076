
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "header/instrlist.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

  //Nastavi ukazatele na NULL

  L->First = NULL;
  L->Act = NULL;
  L->Last = NULL;

}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/

  //Posuva First a cez Act uvolnuje paměť
  while(L->First!= NULL){
      L->Act = L->First;
      L->First = L->First->rptr;
      free(L->Act->instruction);
      L->Act->instruction = NULL;
      free(L->Act);
    }

    L->Act = NULL;
    L->Last = NULL;
}

void DLPostInsertList (tDLList *L, tDLList *M)
{
    if(!L || !M || !L->Act || !M->Act){
        DLError();
        return;
    }

    if(L->Act == L->Last){
        L->Last = M->Last;
        M->First->lptr = L->Act;
        L->Act->rptr = M->First;
    }
    else{
        M->Last->rptr = L->Act->rptr;
        L->Act->rptr->lptr = M->Last;
        L->Act->rptr = M->First;
        M->First->lptr = L->Act;
    }
}

void DLPreInsertList (tDLList *L, tDLList *M)
{
    if(!L || !M || !L->Act || !M->Act){
        DLError();
        return;
    }

    if(L->Act == L->First){
        L->First = M->First;
        L->Act->lptr = M->Last;
        M->Last->rptr = L->Act;
    }
    else{
        M->First->lptr = L->Act->lptr;
        M->Last->rptr = L->Act;
        L->Act->lptr->rptr = M->First;
        L->Act->lptr = M->Last;
    }
}


void DLInsertFirst (tDLList *L, char *instruction) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

  //Naalokujem prvok a nastavim hodnoty
  tDLElemPtr New = malloc(sizeof(struct tDLElem));

  if(New == NULL){
      DLError();
      return;
  }

  New->instruction = malloc(sizeof(char)*(strlen(instruction)+1));
  if(!New->instruction){
      DLError();
      return;
  }

  strcpy(New->instruction,instruction);
  New->lptr = NULL;
  New->rptr = L->First;

  //Ak je zoznam prazdny nastavi aj ukazovatel Last na novy prvok
  if(L->First == NULL)
      L->Last = New;
  else
      L->First->lptr = New;
  L->First = New;
}

void DLInsertLast(tDLList *L, char *instruction) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

  //Naalokujem novy prcok a nastavim hodnoty
  tDLElemPtr New = malloc(sizeof(struct tDLElem));
  if(New == NULL){
      DLError();
      return;
  }

  New->instruction = malloc(sizeof(char)*(strlen(instruction)+1));
  if(!New->instruction){
      DLError();
      return;
  }

  strcpy(New->instruction,instruction);
  New->rptr = NULL;
  New->lptr = L->Last;

  //Ak je zoznam prazdny nastavi novy prvok ako First
  if(L->First == NULL)
      L->First = New;
  else
      L->Last->rptr = New;
  L->Last = New;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
  L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
  L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, char **val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
  if(L->First == NULL){
      DLError();
      return;
  }
  else{
      *val = malloc(sizeof(char)*(strlen(L->First->instruction)+1));
      strcpy(*val,L->First->instruction);
  }

}

void DLCopyLast (tDLList *L, char **val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
  if(L->First == NULL){
      DLError();
      return;
  }
  else{
      *val = malloc(sizeof(char)*(strlen(L->Last->instruction)+1));
      strcpy(*val,L->Last->instruction);
  }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

  //Ak je zoznam prazdny nic sa nedeje
  if(L->First == NULL)
      return;

  //Ak je aktivny prvy prvok aktivita sa straca
  if(L->First == L->Act)
      L->Act = NULL;

  //Pomocny ukazatel na mazany prvok
  tDLElemPtr Pom = L->First;

  //Ak je prvy prvok jediny, zrusi sa zoznam
  if(L->First->rptr == NULL){
    L->First = NULL;
    L->Last = NULL;
  }
  else{
  L->First = L->First->rptr;
  L->First->lptr = NULL;
  }

  //Uvolni prvy prvok
  free(Pom->instruction);
  free(Pom);
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

  //Ak je zoznam prazdny nic sa nedeje
  if(L->First == NULL)
      return;

  //Ak je aktivny posledny prvok aktivita sa straca
  if(L->Last == L->Act)
      L->Act = NULL;

  //Pomocny ukazatel na mazany prvok
  tDLElemPtr Pom = L->Last;

  //Ak je posledny prvok jediny, zrusi sa zoznam
  if(L->Last->lptr == NULL){
      L->First = NULL;
      L->Last = NULL;
  }
  else{
      L->Last = L->Last->lptr;
      L->Last->rptr = NULL;
  }
    //Uvolni posledny prvok
    free(Pom->instruction);
    free(Pom);
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

  if(L->Act == NULL || L->Last == L->Act)
      return;

  //Pomocny ukazatel na mazany prvok
  tDLElemPtr Pom = L->Act->rptr;

  //Ak je mazany prvok posledny nastavi Last na Act
  if(L->Last == Pom){
      L->Last = L->Act;
      L->Act->rptr = NULL;
  }
  else{  //Ak nie je nakonci nastavi spravne pointre
      L->Act->rptr = Pom->rptr;
      Pom->rptr->lptr = L->Act;
  }

  //Uvolni mazany prvok
  free(Pom->instruction);
  free(Pom);
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

  if(L->Act == NULL || L->First == L-> Act)
      return;

  //Pomocny ukazatel na mazany prvok
  tDLElemPtr Pom = L->Act->lptr;

  //Ak je mazany prvok prvy nastavy First na Act
  if(L->First == Pom){
      L->First = L->Act;
      L->Act->lptr = NULL;
  }
  else{  //Ak nie je nakonci nastavi spravne pointre
      L->Act->lptr = Pom->lptr;
      Pom->lptr->rptr = L->Act;
  }
  //Uvolni prvok
  free(Pom->instruction);
  free(Pom);
}

void DLPostInsert (tDLList *L,  char *instruction) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if(L->Act == NULL)
      return;

  //Vytvorenie noveho prvku
  tDLElemPtr New = malloc(sizeof(struct tDLElem));
  if(New == NULL){
      DLError();
      return;
  }
  //Inicializujem data noveho prvku
  New->instruction = malloc(sizeof(char)*(strlen(instruction)+1));
  if(!New->instruction){
      DLError();
      return;
  }

  strcpy(New->instruction,instruction);
  New->lptr = L->Act;

  //Ak je Act posledny nastavy Last na Novy prvok a nastavi pointre
  if(L->Act == L->Last){
      L->Last = New;
      New->rptr = NULL;
  }
  else{
      New->rptr = L->Act->rptr;
      L->Act->rptr->lptr = New;
  }
    L->Act->rptr = New;
}

void DLPreInsert (tDLList *L, char *instruction) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  if(L->Act == NULL)
      return;
  //Vytvorenie noveho prvku
  tDLElemPtr New = malloc(sizeof(struct tDLElem));
  if(New == NULL){
      DLError();
      return;
  }
  //Inicializujem data noveho prvku
  New->instruction = malloc(sizeof(char)*(strlen(instruction)+1));
  if(!New->instruction){
      DLError();
      return;
  }

  strcpy(New->instruction,instruction);
  New->rptr = L->Act;

  //Ak je Act prvy nastavy First na Novy prvok a nastavi pointre
  if(L->Act == L->First){
      L->First = New;
      L->Act->lptr = New;
  }
  else{
      New->lptr = L->Act->lptr;
      L->Act->lptr->rptr = New;
  }
    L->Act->lptr = New;
}

void DLCopy (tDLList *L, char **val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
  if(L->Act != NULL){
      *val = malloc(sizeof(char)*(strlen(L->Act->instruction)+1));
      strcpy(*val,L->Act->instruction);
  }
  else{
      DLError();
      return;
  }
}

void DLActualize (tDLList *L, char  *val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
  if(L->Act != NULL){
    L->Act->instruction = realloc(L->Act->instruction,(sizeof(char)*(strlen(val)+1)));
    strcpy(L->Act->instruction,val);
  }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
  if(L->Act != NULL){
    L->Act = L->Act->rptr;
  }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
  if(L->Act != NULL){
    L->Act = L->Act->lptr;
  }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/

  return L->Act == NULL ? 0 : 1;
}

void DLPrintList (tDLList *L){
    tDLElemPtr TMP;
    TMP = L->First;
    while(TMP){
        printf("%s",TMP->instruction);
        TMP = TMP->rptr;
    }
}

/* Konec c206.c*/
