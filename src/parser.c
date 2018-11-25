#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "header/parser.h"

bool parse(htab_t *sym_tab)
{
    char *buffer = NULL;
    tokenType token = getNextToken(&buffer);
    return st_list(&token, &buffer);
}

bool st_list(tokenType* token, char** buffer)
{
    //////////////////////////////////////////////////// 01
    if(*token != TYPE_EOF)
        return (stat(token, buffer) && st_list(token, buffer));  //<STAT> <ST-LIST>
    else
    //////////////////////////////////////////////////// 02
        return true;                        //EOF
}

bool stat(tokenType* token, char** buffer)
{
    //////////////////////////////////////////////////// 03
    if(*token == TYPE_ID){                   //ID
        if(id_item(token, buffer))        //<ID-ITEM>
            return true;
        else
            return false;
    }
    //////////////////////////////////////////////////// 04
    if(*token == TYPE_FUNC_ID || *token == TYPE_PRE_FUNC){              //FUNC_ID
        htab_call_func(*buffer);        // Vloz do htab FUNC_ID undefined
        *token = getNextToken(buffer);
        if(func(token, buffer))          //<FUNC>
            return true;
        else
            return false;
        }

    ///////////////////////////////////////////////////// 05
    if(*token == TYPE_KEYWORD && !strcmp(*buffer, "if")){     // IF
        if(expression_parse(TYPE_KEYWORD, "then"))       //<EXPRESSION> expecting THEN
            *token = getNextToken(buffer);
            if(*token == TYPE_EOL){      //EOL
                *token = getNextToken(buffer);
                if(else_st_list(token, buffer))     //<ELSE-ST-LIST>
                    if(*token == TYPE_EOL){      //EOL
                        *token = getNextToken(buffer);
                        if(end_st_list(token, buffer))    //<END-ST-LIST>
                            if(*token == TYPE_EOL){      //EOL
                                *token = getNextToken(buffer);
                                return true;
                            }
                    }
            }
        return false;
    }
    ////////////////////////////////////////////////////// 06
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "while")){      //WHILE 
        if(expression_parse(TYPE_KEYWORD, "do")){       //<EXPRESSION>
            *token = getNextToken(buffer);
            if(*token == TYPE_EOL){      //EOL
                *token = getNextToken(buffer);
                if(end_st_list(token, buffer))     //<END-ST-LIST>
                    if(*token == TYPE_EOL){      //EOL
                        *token = getNextToken(buffer);
                        return true;
                    }
            }
        }
        return false;
    }
    ////////////////////////////////////////////////////// 07
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "def")){      //DEF
        *token = getNextToken(buffer);
        if(*token == TYPE_ID){       //ID
            htab_def_func(*buffer);
            *token = getNextToken(buffer);
            if(func(token, buffer) && end_st_list(token, buffer))   // <FUNC> && <END_ST_LIST>
                if(*token == TYPE_EOL)   //EOL
                    return true;
        }
        return false;
    }
    else if(*token == TYPE_EOL){
        *token = getNextToken(buffer);
        return true;
    }
    return false;
}

bool else_st_list(tokenType* token, char** buffer)
{
    //////////////////////////////////////////////////////// 08
    if(*token == TYPE_KEYWORD && !strcmp(*buffer, "else")){   //ELSE
        *token = getNextToken(buffer);
        return true;
    }   
    //////////////////////////////////////////////////////// 09
    else{
        if(stat(token, buffer) && else_st_list(token, buffer))   //<STAT> && <ELSE_ST_LIST>
            return true;
        else
            return false;
    }
}

bool end_st_list(tokenType* token, char** buffer)
{
    //////////////////////////////////////////////////////// 10
    if(*token == TYPE_KEYWORD && !strcmp(*buffer, "end")){     //END
        *token = getNextToken(buffer);
        return true;
    }       
    //////////////////////////////////////////////////////// 11
    else{ 
        if(stat(token, buffer) && end_st_list(token, buffer))   //<STAT> && <END_ST_LIST>
            return true;
        else
            return false;
    }
}

bool id_item(tokenType* token, char** buffer)
{
    
    tokenType temp = *token;
    char* temp_buff = *buffer;
    *token = getNextToken(buffer);
    //////////////////////////////////////// 12
    if(*token == TYPE_ASSIGN){                   // =
        htab_add_id(temp_buff, 27);
        *token = getNextToken(buffer);
        if(assign(token, buffer))            //<ASSIGN>
            return true;
        else
            return false;
    }
    //////////////////////////////////////// 13
    else if(*token == TYPE_EOL){                 //EOL
        htab_add_id(temp_buff, 27);
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 14
    else{
        if(func(token, buffer))              //<FUNC>
            return true;
        else
            return false;
    }
}

bool assign(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 15
    if(*token == TYPE_ID){                       //ID
        if(next(token, buffer))            //<NEXT>
            return true;
        else
            return false;
    }
    //////////////////////////////////////// 16
    else if(*token == TYPE_FUNC_ID || *token == TYPE_PRE_FUNC){             //FUNC_ID
        htab_call_func(*buffer);
        *token = getNextToken(buffer);
        if(func(token, buffer))              //<FUNC>
            return true;
        else
            return false;
    }
    //////////////////////////////////////// 17
    else if(*token == TYPE_L_BRE){
        ungetToken(*token, *buffer);
        if(expression_parse(TYPE_EOL, NULL)){                //<EXPR>
            *token = getNextToken(buffer);
            return true;
        }
        else
            return false;
    }
    else if(*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO){
        tokenType temp = *token;
        char* temp_buff = *buffer;
        *token = getNextToken(buffer);
        if(*token == TYPE_EOL){
            htab_set_type(temp);        // nastavi TYPE vlozeneho ID s 03 -> 12
            return true;
        }
        else{
            ungetToken(temp, temp_buff);
            ungetToken(*token, *buffer);
            if(expression_parse(TYPE_EOL, NULL)){
                *token = getNextToken(buffer);
                return true;
            }
            else
                return false;
            }
    }
    else{
        ungetToken(*token, *buffer);
        if(expression_parse(TYPE_EOL, NULL)){                //<EXPR>
            *token = getNextToken(buffer);
            return true;
        }
        else
            return false;
    }
    
}

bool next(tokenType* token, char** buffer)
{
    tokenType temp = *token;
    char* temp_buff = *buffer;
    *token = getNextToken(buffer);
    //////////////////////////////////////// 18
    if(*token == TYPE_EOL){                      //EOL
        htab_find_id(temp_buff);
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 19
    else if(*token >= TYPE_PLUS && *token <= TYPE_NEG_EQUAL){     //SIGN
        ungetToken(temp, temp_buff);
        ungetToken(*token, *buffer);
        if(expression_parse(TYPE_EOL, NULL)){                  //<EXPR>
            *token = getNextToken(buffer);
            return true;
        }
        else
            return false;
    }
    //////////////////////////////////////// 20
    else if(*token == TYPE_ID){  //ID|VALUE
        htab_call_func(temp_buff);
        htab_def_param(*buffer);
        *token = getNextToken(buffer);
        if(next_param(token, buffer))             //<PARAM>
            return true;
        else
            return false;
    }
    //////////////////////////////////////// 21
    else if(*token == TYPE_L_BRE){               // (
        *token = getNextToken(buffer);
        if(bracket(token, buffer))         //<BRACKET>
            if(*token == TYPE_EOL){             //EOL
                *token = getNextToken(buffer);
                return true;
            }
        return false;
    }
    else
        return false;
}

bool func(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 22
    if(*token == TYPE_L_BRE){               // (
        *token = getNextToken(buffer);
        if(bracket(token, buffer))         //<BRACKET
            if(*token == TYPE_EOL){             //EOL
                *token = getNextToken(buffer);
                return true;
            }
        return false;
    //////////////////////////////////////// 23
    }
    else if(*token == TYPE_EOL){            // EOL
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 24
    else{
        if(param(token, buffer))             //<PARAM>
            return true;
        else
            return false;
    }
}

bool param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 25
    if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO) || *token == TYPE_QUOT || *token == TYPE_QUOT_EMPTY){   // PARAMETER
        *token = getNextToken(buffer);
        if(next_param(token, buffer))        //<NEXT-PARAM>
            return true;
        else
            return false;
    }
    else
        return false;
}

bool next_param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 26
    if(*token == TYPE_EOL){                      //EOL
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 27
    else if(*token == TYPE_COMMA){               // ,
        *token = getNextToken(buffer);
        if(param(token, buffer))             //<PARAM>
            return true;
        else
            return false;
    }
    else
        return false;
}

bool bracket(tokenType* token, char** buffer){
    //////////////////////////////////////// 28
    if(*token == TYPE_R_BRE){                    // )
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 29
    else
        if(brc_param(token, buffer))         //<BRC-PARAM>
            return true;
        else
            return false;
}

bool brc_param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 30
    if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO) || *token == TYPE_QUOT || *token == TYPE_QUOT_EMPTY){   // PARAMETER
        *token = getNextToken(buffer);
        if(next_brc_param(token, buffer))    //<NEXT-BRC-PARAM>
            return true;
        else
            return false;
    }
    else
        return false;
}

bool next_brc_param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 31
    if(*token == TYPE_R_BRE){                    // )
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 32
    else if(*token == TYPE_COMMA){              // ,
        *token = getNextToken(buffer);
        if(brc_param(token, buffer))         //<BRC-PARAM>
            return true;
        else
            return false;
    }
    else
        return false;
}
