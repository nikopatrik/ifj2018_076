#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum automataState{
    STATE_BEGIN,
    STATE_INT,
	STATE_INT_DOT,
	STATE_INT_E,
	STATE_INT_EXPO,
    STATE_FLOAT,
    STATE_FLOAT_E,
	STATE_FLOAT_EXPO_PLUS,
	STATE_FLOAT_EXPO,
    STATE_ID,
    STATE_LCOM,
    STATE_BCOM,
	STATE_BCOM_EQUALS,
	STATE_BCOM_B,
	STATE_BCOM_E,
	STATE_BCOM_G,
	STATE_BCOM_I,
	STATE_BCOM_N,
	STATE_BCOM_COM,
	STATE_BCOM_COM_EOL,
	STATE_BCOM_COM_EQUALS,
	STATE_BCOM_COM_E,
	STATE_BCOM_COM_N,
	STATE_BCOM_COM_D,
	STATE_BCOM_COM_2,
	STATE_ASSIGN,
	STATE_LESS,
	STATE_GREAT,
	STATE_NEG,
    STATE_QUOT,
} automataState;

typedef enum tokenType{
    TYPE_PLUS,          //  +
    TYPE_MINUS,         //  -
    TYPE_MULT,          //  *
    TYPE_DIV,           //  /
    TYPE_ASSIGN,        //  =
    TYPE_EQUAL,         //  ==
    TYPE_GREAT,         //  >
    TYPE_LESS,          //  <
    // TYPE_NEG,           //  !
    TYPE_GREAT_EQUAL,   //  >=
    TYPE_LESS_EQUAL,    //  <=
    TYPE_NEG_EQUAL,     //  !=
    TYPE_L_BRE,         //  (
    TYPE_R_BRE,         //  )
    TYPE_COMMA,         //  ,
    TYPE_QUOT,          //  "	Return string between quotes in 'buffer'
    TYPE_QUOT_EMPTY,    //  ""
    TYPE_INT,			//  Integer
	TYPE_INT_EXPO,		//	Integer with exponential
    TYPE_FLOAT,			//	Float (decimal number)
	TYPE_FLOAT_EXPO,	//	Float with exponential (decimal number)
	TYPE_ID,			//	ID of variable or function
	TYPE_FUNC_ID,		//	ID of function ( ? or ! at end )
    TYPE_KEYWORD,       //  Keyword
    TYPE_PRE_FUNC,      //  Predefined function
    TYPE_EOL,           //  End of line
    TYPE_EOF,			// 	End of file
	TYPE_ERROR,			//	Some Error

} tokenType;


/**
 *	Function allocate memory for buffer, where it adds first char from reading text
 *
 *	@brief	Function for initializing buffer
 *
 *	@param	*buffer	Pointer to ...
 *			c		Char to be inserted to buffer
 */
void firstChar(char **buffer, char c);


/**
 *	Function reallocate memory for bigger size and insert other char from reading text
 *
 *	@brief	Insert another char to buffer
 *
 *	@param	*buffer Pointer to ...
 *			c		Char to be inserted to buffer
 *			length	New length of buffer
 */
void addChar(char **buffer, char c, unsigned length);

/**
 *	Function remembers last token and returt it next time again
 *
 *	@brief	Receive token and return it at next calling
 *
 *	@param	type	Type of returned token
 */
void ungetToken(int type);

/**
 *	Function read text and parse it to different types
 *
 *	@param	*buffer	Pointer to ...
 *	@return	Number from enum tokenType.
 */
int getNextToken(char **buffer,FILE *f);
