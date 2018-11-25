#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifndef _GENERATE_H_
#define _GENERATE_H_

void addString(char **buffer, char *add);
void convertCharToString(char *array, char dumbchar);
void convertToString(char **buffer);
void printFuncLength();
void printFuncSubstr();
void printFuncChr();
void printFuncOrd();
void print_head();
void printFuncBegin(char *fID);
void printFuncEnd();
void printMainBegin();
void printMainEnd();
void getVarInt(char **buffer,char *val);
void getVarFloat(char **buffer,char *val);
void getVarBool(char **buffer,bool value);
void getVarString(char **buffer,char *value);

#endif
