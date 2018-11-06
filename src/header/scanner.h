#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>


typedef enum automataState{
    STATE_BEGIN,				// DONE
    STATE_INT,					// DONE
	STATE_INT_DOT,				// DONE
	STATE_INT_E,				// TODO ERROR
	STATE_INT_EXPO,				// DONE
    STATE_FLOAT,				// DONE
    STATE_EXPO,					// TO REMOVE
    STATE_FLOAT_E,				// TODO ERROR
	STATE_FLOAT_EXPO_PLUS,		// TODO ERROR
	STATE_FLOAT_EXPO,			// DONE
    STATE_ID,
    STATE_FUNC_ID,
    STATE_LCOM,
    STATE_BCOM,
	STATE_ASSIGN,				// DONE
	STATE_LESS,					// DONE
	STATE_GREAT,				// DONE
	STATE_NEG,					// DONE

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
	TYPE_INT_EXPO,
    TYPE_FLOAT,
	TYPE_FLOAT_EXPO,
	TYPE_ID,
	TYPE_FUNC_ID,
    TYPE_EOF,

} tokenType;

void firstChar(tList *L, char c);

void addChar(tList *L, char c);

int scanner(tList *L);
