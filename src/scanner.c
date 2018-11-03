#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>

#include "header/list.h"
#include "header/scanner.h"

FILE *f;

void firstChar(tList *L,char c)
{
    L->Act->buffer = (char*)malloc((L->Act->length + 1) * sizeof(char));
    L->Act->buffer[L->Act->length - 1] = c;
    L->Act->buffer[L->Act->length] = '\0';
}

void addChar(tList *L,char c)
{
    L->Act->length ++;
    L->Act->buffer = L->Act->buffer = (char*)realloc((L->Act->length+1) * sizeof(char));
    L->Act->buffer[L->Act->length - 1] = c;
    L->Act->buffer[L->Act->length] = '\0';
}

int scanner(tList *L){
    automataState state = STATE_BEGIN;
    char c;

    while(1){
        if((c = fgetc(f)) == EOF)
            return END_OF_FILE;
        switch (state) {
            case STATE_BEGIN:
                if(isspace(c))
                    state = STATE_BEGIN;
                else if(c == '+' || c == '-' || c == '*' || c == '/')
                    state = STATE_OPERATOR;
                else if(c == '=' || c == '<' || c== '>' || c == '!')
                    state = STATE_LOGIC;
                else if(isdigit(c))
                    state = STATE_INT;
                else if((c >= 'a' && c <= 'z') || c == '_')
                    state = STATE_ID;
                else if(c == '#')
                    state = STATE_LCOM;
                else if(c == '\n')
                    state = STATE_BCOM;
                break;

            case STATE_OPERATOR:
                firstChar(L,c);
                return 0;
                break;

            case STATE_LOGIC:
                firstChar(L,c);
                if((c = fgetc(f)) == '='){
                    state = STATE_LOGIC_EQUAL;
                    break;
                }
                else{
                    ungetc(c,f);
                    return 0;
                    break;
                }

            case STATE_INT:
                firstChar(L,c);
                while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == '.')
                            addChar(L,c)
                            state = STATE_FLOAT;
                            break;
                        else
                        {
                            ungetc(c,f);
                            return 0;
                            break;
                        }
                    }
                    else
                        addChar(tList *L);
                }
                break;

            case STATE_LOGIC_EQUAL:
                addChar(L,c);
                return 0;
                break;

            case STATE_FLOAT:
                while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == 'e' || c == 'E')
                            addChar(L,c)
                            state = STATE_EXPO;
                            break;
                        else
                        {
                            ungetc(c,f);
                            return 0;
                            break;
                        }
                    }
                    else
                        addChar(L,c);
                }

            case STATE_EXPO_FLOAT:  //TODO: Nacitavat aj +/-
                while(1){
                    if(isdigit(c = fgetc(f)))
                        addChar(L,c);
                    else{
                        ungetc(c,f);
                        return 0;
                        break;
                    }
                }

    }   //switch
    }   //cyklus

    }
