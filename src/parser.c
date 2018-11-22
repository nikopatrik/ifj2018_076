#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "header/parser.h"

bool parse(htab_t *sym_tab, FILE *f)
{
    char *buffer;
    tokenType token = getNextToken(&buffer, f);
    return st_list(&token, &buffer, f);
}

bool st_list(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////////////////// 01
    if(*token != TYPE_EOF)
        return (stat(token, buffer, f) && st_list(token, buffer, f));  //<STAT> <ST-LIST>
    else
    //////////////////////////////////////////////////// 02
        return true;                        //EOF
}

bool stat(tokenType* token, char** buffer,FILE* f)
{
    //////////////////////////////////////////////////// 03
    if(*token == TYPE_ID){                   //ID
        *token = getNextToken(buffer, f);
        if(id_item(token, buffer,f))        //<ID-ITEM>
            return true;
        else
            return false;
    }
    //////////////////////////////////////////////////// 04
    if(*token == TYPE_FUNC_ID || *token == TYPE_PRE_FUNC){              //FUNC_ID
        *token = getNextToken(buffer, f);
        if(func(token, buffer, f))          //<FUNC>
            return true;
        else
            return false;
        }

    ///////////////////////////////////////////////////// 05
    if(*token == TYPE_KEYWORD && !strcmp(*buffer, "if")){     // IF
        *token = getNextToken(buffer, f);
        if(expr(token, buffer, f))       //<EXPRESSION>
            if(*token == TYPE_KEYWORD && !strcmp(*buffer, "then")){    //THEN
                *token = getNextToken(buffer, f);
                if(*token == TYPE_EOL){      //EOL
                    *token = getNextToken(buffer, f);
                    if(else_st_list(token, buffer, f))     //<ELSE-ST-LIST>
                        if(*token == TYPE_EOL){      //EOL
                            *token = getNextToken(buffer, f);
                            if(end_st_list(token, buffer, f))    //<END-ST-LIST>
                                if(*token == TYPE_EOL){      //EOL
                                    *token = getNextToken(buffer, f);
                                    return true;
                                }
                        }
                }
            }
        return false;
    }
    ////////////////////////////////////////////////////// 06
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "while")){      //WHILE 
        *token = getNextToken(buffer, f);
        if(expr(token, buffer, f))       //<EXPRESSION>
            if(*token == TYPE_KEYWORD && !strcmp(*buffer, "do")){    //THEN
                *token = getNextToken(buffer, f);
                if(*token == TYPE_EOL){      //EOL
                    *token = getNextToken(buffer, f);
                    if(end_st_list(token, buffer, f))     //<END-ST-LIST>
                        if(*token == TYPE_EOL){      //EOL
                            *token = getNextToken(buffer, f);
                            return true;
                        }
                }
            }
        return false;
    }
    ////////////////////////////////////////////////////// 07
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "def")){      //DEF
        *token = getNextToken(buffer, f);
        if(*token == TYPE_ID){       //ID
            *token = getNextToken(buffer, f);
            if(func(token, buffer, f) && end_st_list(token, buffer, f))   // <FUNC> && <END_ST_LIST>
                if(*token == TYPE_EOL)   //EOL
                    return true;
        }
        return false;
    }
    else if(*token == TYPE_EOL){
        *token = getNextToken(buffer, f);
        return true;
    }
    return false;
}

bool else_st_list(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////////////////////// 08
    if(*token == TYPE_KEYWORD && !strcmp(*buffer, "else")){   //ELSE
        *token = getNextToken(buffer, f);
        return true;
    }   
    //////////////////////////////////////////////////////// 09
    else{
        if(stat(token, buffer, f) && else_st_list(token, buffer, f))   //<STAT> && <ELSE_ST_LIST>
            return true;
        else
            return false;
    }
}

bool end_st_list(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////////////////////// 10
    if(*token == TYPE_KEYWORD && !strcmp(*buffer, "end")){     //END
        *token = getNextToken(buffer, f);
        return true;
    }       
    //////////////////////////////////////////////////////// 11
    else{ 
        if(stat(token, buffer, f) && end_st_list(token, buffer, f))   //<STAT> && <END_ST_LIST>
            return true;
        else
            return false;
    }
}

bool id_item(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////// 12
    if(*token == TYPE_ASSIGN){                   // =
        *token = getNextToken(buffer, f);
        if(assign(token, buffer, f))            //<ASSIGN>
            return true;
        else
            return false;
    }
    //////////////////////////////////////// 13
    else if(*token == TYPE_EOL){                 //EOL
        *token = getNextToken(buffer, f);
        return true;
    }
    //////////////////////////////////////// 14
    else{
        if(func(token, buffer, f))              //<FUNC>
            return true;
        else
            return false;
    }
    
}

bool assign(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////// 15
    if(*token == TYPE_ID){                       //ID
        *token = getNextToken(buffer, f);
        if(next(token, buffer, f))            //<NEXT>
            return true;
        else
            return false;
    }
    //////////////////////////////////////// 16
    else if(*token == TYPE_FUNC_ID || *token == TYPE_PRE_FUNC){             //FUNC_ID
        *token = getNextToken(buffer, f);
        if(func(token, buffer, f))              //<FUNC>
            return true;
        else
            return false;
    }
    //////////////////////////////////////// 17
    else{
        if(expr(token,buffer,f))                //<EXPR>
            return true;
        else
            return false;
    }
    
}

bool next(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////// 18
    if(*token == TYPE_EOL){                      //EOL
        *token = getNextToken(buffer, f);
        return true;
    }
    //////////////////////////////////////// 19
    else if(*token >= TYPE_PLUS && *token <= TYPE_NEG_EQUAL){     //SIGN
        if(expr(token, buffer, f))                              //<EXPR>
            return true;
        else
            return false;
    }
    //////////////////////////////////////// 20
    else if(*token == TYPE_ID){  //ID|VALUE
        if(next_param(token, buffer, f))             //<PARAM>
            return true;
        else
            return false;
    }
    //////////////////////////////////////// 21
    else if(*token == TYPE_L_BRE){               // (
        *token = getNextToken(buffer, f);
        if(bracket(token, buffer, f))         //<BRACKET>
            if(*token == TYPE_EOL){             //EOL
                *token = getNextToken(buffer, f);
                return true;
            }
        return false;
    }
    else
        return false;
}

bool func(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////// 22
    if(*token == TYPE_L_BRE){               // (
        *token = getNextToken(buffer, f);
        if(bracket(token, buffer, f))         //<BRACKET
            if(*token == TYPE_EOL){             //EOL
                *token = getNextToken(buffer, f);
                return true;
            }
        return false;
    //////////////////////////////////////// 23
    }
    else if(*token == TYPE_EOL){            // EOL
        *token = getNextToken(buffer, f);
        return true;
    }
    //////////////////////////////////////// 24
    else{
        if(param(token, buffer, f))             //<PARAM>
            return true;
        else
            return false;
    }
}

bool param(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////// 25
    if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO) || *token == TYPE_QUOT || *token == TYPE_QUOT_EMPTY){   // PARAMETER
        *token = getNextToken(buffer, f);
        if(next_param(token, buffer, f))        //<NEXT-PARAM>
            return true;
        else
            return false;
    }
    else
        return false;
}

bool next_param(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////// 26
    if(*token == TYPE_EOL){                      //EOL
        *token = getNextToken(buffer, f);
        return true;
    }
    //////////////////////////////////////// 27
    else if(*token == TYPE_COMMA){               // ,
        *token = getNextToken(buffer, f);
        if(param(token, buffer, f))             //<PARAM>
            return true;
        else
            return false;
    }
    else
        return false;
}

bool bracket(tokenType* token, char** buffer, FILE* f){
    //////////////////////////////////////// 28
    if(*token == TYPE_R_BRE){                    // )
        *token = getNextToken(buffer, f);
        return true;
    }
    //////////////////////////////////////// 29
    else
        if(brc_param(token, buffer, f))         //<BRC-PARAM>
            return true;
        else
            return false;
}

bool brc_param(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////// 30
    if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO) || *token == TYPE_QUOT || *token == TYPE_QUOT_EMPTY){   // PARAMETER
        *token = getNextToken(buffer, f);
        if(next_brc_param(token, buffer, f))    //<NEXT-BRC-PARAM>
            return true;
        else
            return false;
    }
    else
        return false;
}

bool next_brc_param(tokenType* token, char** buffer, FILE* f)
{
    //////////////////////////////////////// 31
    if(*token == TYPE_R_BRE){                    // )
        *token = getNextToken(buffer, f);
        return true;
    }
    //////////////////////////////////////// 32
    else if(*token == TYPE_COMMA){              // ,
        *token = getNextToken(buffer, f);
        if(brc_param(token, buffer, f))         //<BRC-PARAM>
            return true;
        else
            return false;
    }
    else
        return false;
}


bool expr(tokenType* token, char** buffer, FILE* f)
{
    *token = getNextToken(buffer, f);
    return true;
}
