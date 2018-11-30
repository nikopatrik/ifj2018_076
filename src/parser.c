#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "header/parser.h"

int param_count = 0;
int def_count = 0;
int if_count = 0;
int while_count = 0;
static tDLList L;

bool parse()
{
    char *buffer = NULL;
    DLInitList(&L);
    printMainBegin(&L);
    tokenType token = getNextToken(&buffer);
    bool ret = st_list(&token, &buffer);
    DLPrintList(&L);
    //DLDisposeList(&L);
    check_defined();
    return ret;
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
            gb_exit_process(2);
    }
    //////////////////////////////////////////////////// 04
    else if(*token == TYPE_FUNC_ID || *token == TYPE_PRE_FUNC){              //FUNC_ID
        TGLOBTab* my_glob_obj = htab_call_func(*buffer);        // Vloz do htab FUNC_ID undefined
        callFunc(&L, *buffer);                                // Volanie funkcie, params preinsert
        DLPreInsert(&L, "CREATEFRAME\n");                      // CREATEFRAME pred parametrami
        *token = getNextToken(buffer);
        if(func(token, buffer)){          //<FUNC>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            return true;
        }
        else
            gb_exit_process(2);
        }

    ///////////////////////////////////////////////////// 05
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "if")){     // IF
        if_count++;
        printIf(&L);
        if(expression_parse(TYPE_KEYWORD, "then")){       //<EXPRESSION> expecting THEN
            *token = getNextToken(buffer);
            if(*token == TYPE_EOL){      //EOL
                *token = getNextToken(buffer);
                if(else_st_list(token, buffer))     //<ELSE-ST-LIST>
                    if(*token == TYPE_EOL){      //EOL
                        printElse(&L);
                        *token = getNextToken(buffer);
                        if(end_st_list(token, buffer))    //<END-ST-LIST>
                            if(*token == TYPE_EOL){      //EOL
                                printEndif(&L);
                                *token = getNextToken(buffer);
                                //endif
                                if_count--;
                                return true;
                            }
                    }
            }
        }
        gb_exit_process(2);
    }
    ////////////////////////////////////////////////////// 06
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "while")){      //WHILE 
        while_count++;                          //TODO: DEFVAR PRED WHILE
        printWhile(&L);
        if(expression_parse(TYPE_KEYWORD, "do")){       //<EXPRESSION>
            *token = getNextToken(buffer);
            if(*token == TYPE_EOL){      //EOL
                *token = getNextToken(buffer);
                if(end_st_list(token, buffer))     //<END-ST-LIST>
                    if(*token == TYPE_EOL){      //EOL
                        printEndwhile(&L);
                        *token = getNextToken(buffer);
                        while_count--;
                        return true;
                    }
            }
        }
        gb_exit_process(2);
    }
    ////////////////////////////////////////////////////// 07
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "def")){      //DEF
        def_count++;
        if(def_count > 1 || if_count > 0 || while_count > 0){
            printf("ERROR: definition inside if,while,def\n");
            gb_exit_process(2);
        }
        *token = getNextToken(buffer);
        if(*token == TYPE_ID || *token == TYPE_FUNC_ID){       //ID
            TGLOBTab* my_glob_obj = htab_def_func(*buffer);
            printFuncBegin(&L, *buffer);
            *token = getNextToken(buffer);
            if(func(token, buffer)){
                htab_set_param_count(my_glob_obj, param_count);
                param_count = 0;
                lastParam();
                if(end_st_list(token, buffer)){   // <FUNC> && <END_ST_LIST>
                    if(*token == TYPE_EOL){   //EOL
                        printFuncEnd(&L);
                        def_count--;
                        htab_set_main();
                        return true;
                    }
                }
            }
        }
        gb_exit_process(2);
    }
    else if(*token == TYPE_EOL){
        *token = getNextToken(buffer);
        return true;
    }
    else
        gb_exit_process(2);
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
            gb_exit_process(2);
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
            gb_exit_process(2);
    }
}

bool id_item(tokenType* token, char** buffer)
{
    
    tokenType temp = *token;
    char* temp_buff = *buffer;
    *token = getNextToken(buffer);
    //////////////////////////////////////// 12
    if(*token == TYPE_ASSIGN){                   // =
        htab_add_id(temp_buff);
        *token = getNextToken(buffer);
        if(assign(token, buffer))            //<ASSIGN>
            return true;
        else
            gb_exit_process(2);
    }
    //////////////////////////////////////// 13
    else if(*token == TYPE_EOL){                 //EOL
        htab_find_id(temp_buff);
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 14
    else{
        TGLOBTab* my_glob_obj = htab_call_func(temp_buff);        // Vloz do htab FUNC_ID undefined
        if(func(token, buffer)){              //<FUNC>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            return true;
        }
        else
            gb_exit_process(2);
    }
}

bool assign(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 15
    if(*token == TYPE_ID){                       //ID
        if(next(token, buffer))            //<NEXT>
            return true;
        else
            gb_exit_process(2);
    }
    //////////////////////////////////////// 16
    else if(*token == TYPE_FUNC_ID || *token == TYPE_PRE_FUNC){             //FUNC_ID
        TGLOBTab* my_glob_obj = htab_call_func(*buffer);
        *token = getNextToken(buffer);
        if(func(token, buffer)){              //<FUNC>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            return true;
        }
        else
            gb_exit_process(2);
    }
    //////////////////////////////////////// 17
    else if(*token == TYPE_L_BRE){
        ungetToken(*token, *buffer);
        if(expression_parse(TYPE_EOL, NULL)){                //<EXPR>
            *token = getNextToken(buffer);
            return true;
        }
        else
            gb_exit_process(2);
    }
    /* TODO: Mozno spojit s TYPE_L_BRE? Kedze precedence vyhodnocuje aj
     * expression o dlzke 1*/
    //////////////////////////////////////// 17
    else if(*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO){      //<EXPR> 
        ungetToken(*token, *buffer);
        if(expression_parse(TYPE_EOL, NULL)){
            *token = getNextToken(buffer);
            return true;
        }
        else
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
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
            gb_exit_process(2);
    }
    //////////////////////////////////////// 20
    else if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO)){  //ID|VALUE
        TGLOBTab* my_glob_obj = htab_call_func(temp_buff);
        param_count++;                      
        *token = getNextToken(buffer);
        if(next_param(token, buffer)){             //<PARAM>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            return true;
        }
        else
            gb_exit_process(2);
    }
    //////////////////////////////////////// 21
    else if(*token == TYPE_L_BRE){               // (
        TGLOBTab* my_glob_obj = htab_call_func(temp_buff);
        *token = getNextToken(buffer);
        if(bracket(token, buffer))         //<BRACKET>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            if(*token == TYPE_EOL){             //EOL
                *token = getNextToken(buffer);
                return true;
            }
        gb_exit_process(2);
    }
    else
        gb_exit_process(2);
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
        gb_exit_process(2);
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
            gb_exit_process(2);
    }
}

bool param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 25
    if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO)){   // PARAMETER
        if(def_count == 0){          // Ak volam funckiu
            if(*token == TYPE_ID){
                printParam(&L, NONE, *buffer);
                htab_check_param(*buffer);
            }
            else if(*token == TYPE_QUOT || *token == TYPE_QUOT_EMPTY){
                printParam(&L, STRING, *buffer);
            }
            else if(*token == TYPE_INT || *token == TYPE_INT_EXPO){
                printParam(&L, INT, *buffer);
            }
            else if(*token = TYPE_FLOAT || *token == TYPE_FLOAT_EXPO){
                printParam(&L, FLOAT, *buffer);
            }
        }
        else{
            printDefParam(&L, *buffer);
            if(*token == TYPE_ID){
                if(def_count != 0){
                    htab_def_param(*buffer);
                }
            }
        }
        param_count++;
        *token = getNextToken(buffer);
        if(next_param(token, buffer))        //<NEXT-PARAM>
            return true;
        else
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
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
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
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
            gb_exit_process(2);
}

bool brc_param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 30
    if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO)){   // PARAMETER
        if(def_count == 0){          // Ak volam funckiu
            if(*token == TYPE_ID){
                printParam(&L, NONE, *buffer);
                htab_check_param(*buffer);
            }
            else if(*token == TYPE_QUOT || *token == TYPE_QUOT_EMPTY){
                printParam(&L, STRING, *buffer);
            }
            else if(*token == TYPE_INT || *token == TYPE_INT_EXPO){
                printParam(&L, INT, *buffer);
            }
            else if(*token = TYPE_FLOAT || *token == TYPE_FLOAT_EXPO){
                printParam(&L, FLOAT, *buffer);
            }
        }
        else{
            printDefParam(&L, *buffer);
            if(*token == TYPE_ID){
                if(def_count != 0){
                    htab_def_param(*buffer);
                }
            }
        }
        param_count++;
        *token = getNextToken(buffer);
        if(next_brc_param(token, buffer))    //<NEXT-BRC-PARAM>
            return true;
        else
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
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
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
}
