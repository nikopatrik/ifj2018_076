#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>

#include "c201.h"

FILE *f;

<<<<<<< Updated upstream
void firstChar(tList *L)
=======

void firstChar(tList *L,char c)
>>>>>>> Stashed changes
{
    L->Act->buffer = (char*)malloc((L->Act->length + 1) * sizeof(char));
    L->Act->buffer[L->Act->length - 1] = c;
    L->Act->buffer[L->Act->length] = '\0';
}

<<<<<<< Updated upstream
void addChar(tList *L)
=======
void addChar(tList *L,char c)
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
                else if(isalpha(c) || c == '_')
=======
                else if((c >= 'a' && c <= 'z') || c == '_')
>>>>>>> Stashed changes
                    state = 6;
                else if(c == '#')
                    state = 7;
                else if(c == '\n')
                    state = 8;
                break;

            case 1:
<<<<<<< Updated upstream
                firstChar(L);
=======
                firstChar(L,c);
>>>>>>> Stashed changes
                return 0;
                break;

            case 234:
<<<<<<< Updated upstream
                firstChar(L);
=======
                firstChar(L,c);
>>>>>>> Stashed changes
                if((c = fgetc(f)) == '='){
                    state = 9;
                    break;
                }
                else{
<<<<<<< Updated upstream
                    L->Act->buffer[L->Act->length] = '\0';
=======
>>>>>>> Stashed changes
                    ungetc(c,f);
                    return 0;
                    break;
                }

            case 5:
<<<<<<< Updated upstream
                firstChar(L);
=======
                firstChar(L,c);
>>>>>>> Stashed changes
                while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == '.')
<<<<<<< Updated upstream
=======
                            addChar(L,c)
>>>>>>> Stashed changes
                            state = 10;
                            break;
                        else
                        {
<<<<<<< Updated upstream
                            L->Act->buffer[L->Act->length] = '\0';
=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
                addChar(tList *L);
=======
                addChar(L,c);
>>>>>>> Stashed changes
                return 0;
                break;

            case 10:
                while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == 'e' || c == 'E')
<<<<<<< Updated upstream
=======
                            addChar(L,c)
>>>>>>> Stashed changes
                            state = 11;
                            break;
                        else
                        {
<<<<<<< Updated upstream
                            L->Act->buffer[L->Act->length] = '\0';
=======
>>>>>>> Stashed changes
                            ungetc(c,f);
                            return 0;
                            break;
                        }
                    }
                    else
<<<<<<< Updated upstream
                        addChar(tList *L);
                }   //TODO Dokoncit posledny stav
=======
                        addChar(L,c);
                }

            case 11:
                while(1){
                    if(isdigit(c = fgetc(f)))
                        addChar(L,c);
                    else{
                        ungetc(c,f);
                        return 0;
                        break;
                    }
                }
>>>>>>> Stashed changes


    }   //switch
    }   //cyklus

    }
