#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>


FILE *f;

typedef struct list{
    char *buffer;
    unsigned length = 1;
    struct *list next;


int scanner(){
    int state = 0;
    char c;

    while(1){
        if((c = fgetc(f)) == EOF)
            return END_OF_FILE;
        switch (state) {
            case 0:
                if(isspace(c))
                    state = 0;
                else if(c == '+' || c == '-' || c == '*' || c == '/')
                    state = 1;
                else if(c == '=' || c == '<' || c== '>' || c == '!')
                    state = 234;
                else if(isdigit(c))
                    state = 5;
                else if(isalpha(c) || c == '_')
                    state = 6;
                else if(c == '#')
                    state = 7;
                else if(c == '\n')
                    state = 8;
                break;

            case 1:
                list->buffer = (char*)malloc((list->length + 1) * sizeof(char));  //nevedel som ako to lepsie vymysliet
                list->buffer[0] = c;
                list->buffer[1] = '\0';
                return 0; //este domyslim co returnovat
                break;

            case 234:
                list->buffer = (char*)malloc((list->length + 1) * sizeof(char));
                list->buffer[list->length - 1] = c;
                if((c = fgetc(f)) == '='){
                    case = 9;
                    break;
                }
                else{
                    list->buffer[list->length] = '\0';
                    ungetc(c,f);
                    return 0;
                    break;
                }

            case 9:
                list->length ++;
                list->buffer = list->buffer = (char*)realloc((list->length+1) * sizeof(char));
                list->buffer[list->length - 1] = c;
                list->buffer[list->length] = '\0';
                return 0;
                break;

    }   //switch
    }   //cyklus

    }
