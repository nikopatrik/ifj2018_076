#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifndef _GENERATE_H_
#define _GENERATE_H_

#include "instrlist.h"
#include "generate.h"
#include "symtable.h"
#include "garbagecollector.h"
#include "stack.h"

void myStackInit();
void addString(char **buffer, char *add);
void fillString(char **buffer, char *formatted_string, ...);
void convertCharToString(char *array, char dumbchar);
void convertToString(char **buffer);
void getVarInt(char **buffer,char *val);
void getVarFloat(char **buffer,char *val);
void getVarBool(char **buffer,bool value);
void getVarString(char **buffer,char *value);
void printHead();
void printParam(tDLList *L, TYPES type, char *value);
void callFunc(tDLList *L, char *name);
void printFuncLength(tDLList *L);
void printFuncSubstr(tDLList *L);
void printFuncChr(tDLList *L);
void printFuncOrd(tDLList *L);
void printFuncBegin(char *fID);
void printFuncEnd();
void printMainBegin();
void printMainEnd();
void printIf(tDLList *L);
void printElse(tDLList *L);
void printEndif(tDLList *L);

#endif
