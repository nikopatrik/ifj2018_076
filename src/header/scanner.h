#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>

typedef enum automataState{
    STATE_BEGIN,
    STATE_OPERATOR,
    STATE_LOGIC,
    STATE_LOGIC_EQUAL,
    STATE_INT,
    STATE_FLOAT,
    STATE_EXPO,
    STATE_EXPO_FLOAT,
    STATE_ID,
    STATE_FUNC_ID,
    STATE_LCOM,
    STATE_BCOM,
} automataState;

void firstChar(tList *L, char c);

void addChar(tList *L, char c);

int scanner(tList *L);
