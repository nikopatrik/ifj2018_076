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
	STATE_INT_DOT,
	//nove stavy pre EXPO bez desatinnej casti
    STATE_FLOAT,
    STATE_EXPO,
    STATE_EXPO_FLOAT,
    STATE_ID,
    STATE_FUNC_ID,
    STATE_LCOM,
    STATE_BCOM,
	STATE_ASSIGN,
	STATE_LESS,
	STATE_GREAT,
	STATE_NEG,

} automataState;

typedef enum tokenType{
    TYPE_PLUS,          //  +
    TYPE_MINUL,         //  -
    TYPE_MULT,          //  *
    TYPE_DIV,           //  /
    TYPE_ASSIGN,        //  =
    TYPE_EQUAL,         //  ==
    TYPE_GREAT,         //  >
    TYPE_LESS,          //  <
    TYPE_NEG,           //  !
    TYPE_GREAT_EQUAL,   //  >=
    TYPE_LESS_EQUAL,    //  <=
    TYPE_NEG_EQUAL,     //  !=
    TYPE_L_BRE,         //  (
    TYPE_R_BRE,         //  )
    TYPE_L_COM,
    TYPE_B_COM
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_EOF,

} tokenType;

void firstChar(tList *L, char c);

void addChar(tList *L, char c);

int scanner(tList *L);
