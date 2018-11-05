#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>

#include "header/list.h"
#include "header/scanner.h"

FILE *f;

void firstChar(char *buffer,char c)
{
    buffer = (char*)malloc(2 * sizeof(char));
    buffer[0] = c;
    buffer[1] = '\0';
}

void addChar(char *buffer,char c,unsigned length){
{
    buffer = buffer = (char*)realloc((length+1) * sizeof(char));
    buffer[length - 1] = c;
    buffer[length] = '\0';
}

int scanner(char *buffer){
    automataState state = STATE_BEGIN;
    char c;
    unsigned length = 1;

    while(1){
        if((c = fgetc(f)) == EOF)
            return END_OF_FILE;
        switch (state) {
            case STATE_BEGIN:
                if(isspace(c))
                    state = STATE_BEGIN;
                switch(c){
                    case '+' :
                        firstChar(buffer,c);
                        return TYPE_PLUS;
                        break;
                    case '-' :
                        firstChar(buffer,c);
                        return TYPE_MINUS;
                        break;
                    case '*' :
                        firstChar(buffer,c);
                        return TYPE_MULT;
                        break;
                    case '/' :
                        firstChar(buffer,c);
                        return TYPE_DIV;
                        break;
                    case '(' :
                        firstChar(buffer,c);
                        return TYPE_L_BRE;
                        break;
                    case ')' :
                        firstChar(buffer,c);
                        return TYPE_L_BRE;
                        break;
                }

                // else if(c == '=' || c == '<' || c== '>' || c == '!')
                //     state = STATE_LOGIC;
                else if(isdigit(c))
                    state = STATE_INT;
                else if((c >= 'a' && c <= 'z') || c == '_')
                    state = STATE_ID;
                else if(c == '#')
                    state = STATE_LCOM;
                else if(c == '\n')
                    state = STATE_BCOM;
                break;

            // case STATE_LOGIC:
            //     firstChar(buffer,c);
            //     if((c = fgetc(f)) == '='){
            //         state = STATE_LOGIC_EQUAL;
            //         break;
            //     }
            //     else{
            //         ungetc(c,f);
            //         return 0;
            //         break;
            //     }

            case STATE_INT:
                firstChar(buffer,c);
                while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == '.')
                            length ++;
                            addChar(buffer,c,length)
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
                        length ++;
                        addChar(buffer,c,length);
                }
                break;

            case STATE_LOGIC_EQUAL:
                length ++;
                addChar(buffer,c,length);
                return 0;
                break;

            case STATE_FLOAT:
                while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == 'e' || c == 'E'){
                            length ++;
                            addChar(buffer,c,length)
                            state = STATE_EXPO;
                            break;
                        }
                        else
                        {
                            ungetc(c,f);
                            return 0;
                            break;
                        }
                    }
                    else{
                        length ++;
                        addChar(buffer,c,length);
                    }
                }

            case STATE_EXPO:
                if((c=fgetc(f)) == '+' || c == '-' || isdigit(c)){
                    length ++;
                    addChar(buffer,c,length);
                    state = STATE_EXPO_FLOAT;
                    break;
                }
                else{

                }


                while(1){
                    if(isdigit(c = fgetc(f))){
                        length ++;
                        addChar(buffer,c,length);
                    }
                    else{
                        ungetc(c,f);
                        return 0;
                        break;
                    }
                }

    }   //switch
    }   //cyklus

    }
