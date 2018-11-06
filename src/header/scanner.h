#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>


typedef enum automataState{
    STATE_BEGIN,				// DONE
    STATE_INT,					// DONE
	STATE_INT_DOT,				// DONE
	STATE_INT_E,				// DONE
	STATE_INT_EXPO,				// DONE
    STATE_FLOAT,				// DONE
    STATE_FLOAT_E,				// DONE
	STATE_FLOAT_EXPO_PLUS,		// DONE
	STATE_FLOAT_EXPO,			// DONE
    STATE_ID,					// DONE
    STATE_LCOM,					// DONE
    STATE_BCOM,					// DONE
	STATE_BCOM_EQUALS,			// DONE
	STATE_BCOM_B,				// DONE
	STATE_BCOM_E,				// DONE
	STATE_BCOM_G,				// DONE
	STATE_BCOM_I,				// DONE
	STATE_BCOM_N,				// DONE
	STATE_BCOM_COM,				// DONE
	STATE_BCOM_COM_EOL,			// DONE
	STATE_BCOM_COM_EQUALS,		// DONE
	STATE_BCOM_COM_E,			// DONE
	STATE_BCOM_COM_N,			// DONE
	STATE_BCOM_COM_D,			// DONE
	STATE_BCOM_COM_2,			// DONE
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
	TYPE_ERROR,

} tokenType;

void firstChar(tList *L, char c);

void addChar(tList *L, char c);

int scanner(tList *L);
