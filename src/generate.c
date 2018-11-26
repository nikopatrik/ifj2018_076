#include <stdarg.h>

#include "header/generate.h"
#include "header/instrlist.h"
#include "header/symtable.h"
#include "header/garbagecollector.h"

void addString(char **buffer, char *add)
{
    if(add){
        if(*buffer){
            unsigned old = strlen(*buffer);v
            unsigned new = old + strlen(add);
            *buffer = realloc(*buffer, new +1);
            strcat(*buffer,add);
            (*buffer)[new] = '\0';
        }
        else{
            unsigned size = strlen(add);
            *buffer = malloc(sizeof(char)*(size+1));
            strcpy(*buffer,add);
        }
    }

}

void fillString(char **buffer,char *formatted_string,...)
{
    va_list l,l2;
    va_start(l,formatted_string);
    va_copy(l2,l);
    unsigned size = vsnprintf(NULL,0,formatted_string,l);
    *buffer = malloc(size + sizeof(char));
    vsnprintf(*buffer,size + sizeof(char),formatted_string,l2);
    va_end(l);
    va_end(l2);
}

void convertCharToString(char *array, char dumbchar)
{
    int dumbnumber = dumbchar;
    array[0]='\\';
    array[1]='0' + dumbnumber/100;
    array[2]='0' + dumbnumber/10 %10;
    array[3]='0' + dumbnumber%10;
}


void convertToString(char **buffer)
{
    int count_of_replaceable_char;
    for(unsigned i=0; i< strlen(*buffer);i++)
        if((*buffer)[i] < 33 || ((*buffer)[i] == 35) || (*buffer)[i] == 92)
            count_of_replaceable_char++;
    char *formatedstring = malloc(strlen(*buffer)+count_of_replaceable_char*4 + 1);
    unsigned j=0;
    for(unsigned i=0; i< strlen(*buffer);i++){
        if((*buffer)[i] < 33 || ((*buffer)[i] == 35) ||(*buffer)[i] == 92){
            convertCharToString(&formatedstring[j],(*buffer)[i]);
            j+=4;
        }
        else{
            formatedstring[j]=(*buffer)[i];
            j++;
        }
    }
    unsigned length= strlen(formatedstring)+1;n
    *buffer = realloc(*buffer,length);
    strcpy(*buffer,formatedstring);
    *buffer[length]= '\0';
    free(formatedstring);

}

void getVarInt(char **buffer,char *val)
{
    int value = atoi(val);
    unsigned size = snprintf(NULL, 0, "int@%d", value);
    *buffer = (char *)malloc(size + 1);
    snprintf(*buffer, size+1, "int@%d", value);
}

void getVarFloat(char **buffer,char *val)
{
    double value = atof(val);
    printf("%lf\n",value );
    unsigned size = snprintf(NULL, 0, "float@%a", value);
    *buffer = (char *)malloc(size + 1);
    snprintf(*buffer, size+1, "float@%a", value);
}

void getVarBool(char **buffer,bool value)
{
    unsigned size = strlen("bool@TRUE");
    if(value){
        *buffer = (char *)malloc(size + 1);
        strcpy(*buffer,"bool@TRUE");
    }
    else{
        *buffer = (char *)malloc(size + 2);
        strcpy(*buffer,"bool@FALSE");
    }
}

void getVarString(char **buffer,char *value)
{
    unsigned size = strlen(value);
    if(size == 0)
    {
        *buffer = malloc(sizeof(char)*8);
        strcpy(*buffer,"string@");
    }
    else
    {
        char *string;
        string = malloc(size + 2);
        strcpy(string,value);
        convertToString(&string);

        size = snprintf(NULL, 0, "string@%s", string);
        *buffer = (char *)malloc(size + 1);
        snprintf(*buffer, size+1, "string@%s", string);
        free(string);
    }

}

void printHead()
{
    fprintf(stdout, ".IFJcode18\nJUMP $MAIN\n");
}

static int id_Param = 0;
//nefunguje
void printParam(tDLList *L,TYPES type,char *value)
{
    char *buffer = NULL;
    switch (type) {
        case NIL:
            unsigned size = snprintf(NULL, 0, "DEFVAR @%a", id_Param);
            *buffer = (char *)malloc(size + 1);


    }
}


void printFuncLength(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer,"LABEL LENGTH\n"
                      "PUSHFRAME\n"
                      "DEFVAR LF@retval\n"
                      "STRLEN LF@retval LF@param1\n"
                      "POPFRAME\n"
                      "RETURN\n");
    DLInsertFirst(L,buffer);
    free(buffer);
}

void printFuncSubstr(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer,  "LABEL $SUBSTR\n"
                        "PUSHFRAME\n"
                        "DEFVAR LF@retval\n"
                        "MOVE LF@retval string@\n"
                        "DEFVAR LF@length_str\n"
                        "CREATEFRAME\n"
                        "DEFVAR TF@param1\n"
                        "MOVE TF@param1 LF@param1\n"
                        "CALL $length\n"
                        "MOVE LF@length_str TF@retval\n"
                        "DEFVAR LF@ret_cond\n"
                        "LT LF@ret_cond LF@length_str int@0\n"
                        "JUMPIFEQ $substr$return LF@ret_cond bool@true\n"
                        "EQ LF@ret_cond LF@length_str int@0\n"
                        "JUMPIFEQ $substr$return LF@ret_cond bool@true\n"
                        "LT LF@ret_cond LF@param2 int@0\n"
                        "JUMPIFEQ $substr$return LF@ret_cond bool@true\n"
                        "EQ LF@ret_cond LF@param2 int@0\n"
                        "JUMPIFEQ $substr$return LF@ret_cond bool@true\n"
                        "GT LF@ret_cond LF@param2 LF@length_str\n"
                        "JUMPIFEQ $substr$return LF@ret_cond bool@true\n"
                        "EQ LF@ret_cond LF@param3 int@0\n"
                        "JUMPIFEQ $substr$return LF@ret_cond bool@true\n"
                        "DEFVAR LF@max_n\n"
                        "MOVE LF@max_n LF@length_str\n"
                        "SUB LF@max_n LF@max_n LF@param2\n"
                        "ADD LF@max_n LF@max_n int@1\n"
                        "DEFVAR LF@edit_n_cond\n"
                        "LT LF@edit_n_cond LF@param3 int@0\n"
                        "JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true\n"
                        "GT LF@edit_n_cond LF@param3 LF@max_n\n"
                        "JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true\n"
                        "JUMP $substr$process\n"
                        "LABEL $substr$edit_n\n"
                        "MOVE LF@param3 LF@max_n\n"
                        "LABEL $substr$process\n"
                        "DEFVAR LF@index\n"
                        "MOVE LF@index LF@param2\n"
                        "SUB LF@index LF@index int@1\n"
                        "DEFVAR LF@char\n"
                        "DEFVAR LF@process_loop_cond\n"
                        "LABEL $substr$process_loop\n"
                        "GETCHAR LF@char LF@param1 LF@index\n"
                        "CONCAT LF@retval LF@retval LF@char\n"
                        "ADD LF@index LF@index int@1\n"
                        "SUB LF@param3 LF@param3 int@1\n"
                        "GT LF@process_loop_cond LF@param3 int@0\n"
                        "JUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true\n"
                        "LABEL $substr$return\n"
                        "POPFRAME\n"
                        "RETURN\n" );
    DLInsertFirst(L,buffer);
    free(buffer);
}


void printFuncChr(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer,  "LABEL $CHR\n"
                        "PUSHFRAME\n"
                        "DEFVAR LF@retval\n"
                        "DEFVAR LF@vysl\n"
                        "LT LF@vysl LF@param1 int@0\n"
                        "JUMPIFEQ $ENDIF$CHR LF@vysl bool@TRUE\n"
                        "LT LF@vysl LF@param1 int@256\n"
                        "JUMPIFEQ $ENDIF$CHR LF@vysl bool@FALSE\n"
                        "INT2CHAR LF@retval LF@param1\n"
                        "LABEL $ENDIF$CHR\n"
                        "POPFRAME\n"
                        "RETURN\n");
    DLInsertFirst(L,buffer);
    free(buffer);
}

void printFuncOrd(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer, "LABEL $ORD\n"
                        "PUSHFRAME\n"
                        "DEFVAR LF@retval\n"
                        "MOVE LF@retval nil@nil\n"
                        "DEFVAR LF@vysl\n"
                        "DEFVAR LF@length\n"
                        "STRLEN LF@length LF@param2\n"
                        "LT LF@vysl LF@param1 int@0\n"
                        "JUMPIFEQ $ENDIF$ORD LF@vysl bool@TRUE\n"
                        "LT LF@vysl LF@param1 LF@length\n"
                        "JUMPIFEQ $ENDIF$ORD LF@vysl bool@FALSE\n"
                        "STRI2INT LF@retval LF@param2 LF@param1\n"
                        "LABEL $ENDIF$ORD\n"
                        "POPFRAME\n"
                        "RETURN\n" );
    DLInsertFirst(L,buffer);
    free(buffer);
}

void printParam(tDLList *L,TYPES type,)
{

}

void printFuncBegin(char *fID)
{
    fprintf(stdout, "LABEL $%s\nPUSHFRAME\nDEFVAR LF@retval\nMOVE LF@retval nil@nil",fID);
}

void printFuncEnd()
{
    fprintf(stdout, "POPFRAME\nRETURN\n");
}


void printMainBegin()
{
    fprintf(stdout, "LABEL $MAIN\nCREATEFRAME\nPUSHFRAME");
}

void printMainEnd()
{
    fprintf(stdout, "CLEARS\nPOPFRAME\n");
}




    // fprintf(f, "JUMP $$ENDIF%d\n",id_if);
    // fprintf(f, "LABEL $$ENFIF%d\n",id_if);
    //
    // fprintf(f, "JUMP $$ENDIF%d\n",id_if);
    // fprintf(f, "LABEL $$ELSE%d\n",id_if);
    // fprintf(f, "LABEL $$ENDIF%d\n",id_if);
    //
    //
    // fprintf(f, "LABEL $$WHILE%d\n",id_while);
    // fprintf(f, "JUMP $$WHILE%d\n", id_while);
    // fprintf(f, "LABEL $$ENDIF%d\n", id_while);
