#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>

#include "c201.h"

FILE *f;




int scanner(tList *L){
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
                L->Act->buffer = (char*)malloc((L->Act->length + 1) * sizeof(char));  //nevedel som ako to lepsie vymysliet
                L->Act->buffer[0] = c;
                L->Act->buffer[1] = '\0';
                return 0; //este domyslim co returnovat
                break;

            case 234:
                L->Act->buffer = (char*)malloc((L->Act->length + 1) * sizeof(char));
                L->Act->buffer[L->Act->length - 1] = c;
                if((c = fgetc(f)) == '='){
                    case = 9;
                    break;
                }
                else{
                    L->Act->buffer[L->Act->length] = '\0';
                    ungetc(c,f);
                    return 0;
                    break;
                }

            case 5:
                

            case 9:
                L->Act->length ++;
                L->Act->buffer = L->Act->buffer = (char*)realloc((L->Act->length+1) * sizeof(char));
                L->Act->buffer[L->Act->length - 1] = c;
                L->Act->buffer[L->Act->length] = '\0';
                return 0;
                break;

    }   //switch
    }   //cyklus

    }
