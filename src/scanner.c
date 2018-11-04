#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>

#include "c201.h"

FILE *f;

void firstChar(tList *L)
{
    L->Act->buffer = (char*)malloc((L->Act->length + 1) * sizeof(char));
    L->Act->buffer[L->Act->length - 1] = c;
    L->Act->buffer[L->Act->length] = '\0';
}

void addChar(tList *L)
{
    L->Act->length ++;
    L->Act->buffer = L->Act->buffer = (char*)realloc((L->Act->length+1) * sizeof(char));
    L->Act->buffer[L->Act->length - 1] = c;
    L->Act->buffer[L->Act->length] = '\0';
}

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
                firstChar(L);
                return 0;
                break;

            case 234:
                firstChar(L);
                if((c = fgetc(f)) == '='){
                    state = 9;
                    break;
                }
                else{
                    L->Act->buffer[L->Act->length] = '\0';
                    ungetc(c,f);
                    return 0;
                    break;
                }

            case 5:
                firstChar(L);
                while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == '.')
                            state = 10;
                            break;
                        else
                        {
                            L->Act->buffer[L->Act->length] = '\0';
                            ungetc(c,f);
                            return 0;
                            break;
                        }
                    }
                    else
                        addChar(tList *L);
                }
                break;

            case 9:
                addChar(tList *L);
                return 0;
                break;

            case 10:
                while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == 'e' || c == 'E')
                            state = 11;
                            break;
                        else
                        {
                            L->Act->buffer[L->Act->length] = '\0';
                            ungetc(c,f);
                            return 0;
                            break;
                        }
                    }
                    else
                        addChar(tList *L);
                }   //TODO Dokoncit posledny stav


    }   //switch
    }   //cyklus

    }
