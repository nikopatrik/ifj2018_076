#include <stdio.h>
#include <stdlib.h>
#include <symtable.h>
#include <string.h>

#include "header/list.h"
#include "header/scanner.h"

FILE *f;

void firstChar(char *buffer, char c)
{
    buffer = (char*)malloc(2 * sizeof(char));
    buffer[0] = c;
    buffer[1] = '\0';
}

void addChar(char *buffer, char c, unsigned length){
{
	buffer = (char*)realloc((length+1) * sizeof(char));
    buffer[length - 1] = c;
    buffer[length] = '\0';
}

int scanner(char *buffer){
    automataState state = STATE_BEGIN;
    char c;
    unsigned length = 1;

    while(1){
        if((c = fgetc(f)) == EOF)
            return TYPE_EOF;
        switch (state) {
            case STATE_BEGIN:
                if(isspace(c))
                    state = STATE_BEGIN;
                switch(c){
                    case '+' :
                        return TYPE_PLUS;
                        break;
                    case '-' :
                        return TYPE_MINUS;
                        break;
                    case '*' :
                        return TYPE_MULT;
                        break;
                    case '/' :
                        return TYPE_DIV;
                        break;
                    case '(' :
                        return TYPE_L_BRE;
                        break;
                    case ')' :
                        return TYPE_L_BRE;
                        break;
					case '=' :
						state = STATE_ASSIGN;
						break;
					case '<' :
						state = STATE_LESS;
						break;
					case '>' :
						state = STATE_GREAT;
						break;
					case '!' :
						state = STATE_NEG;
						break;
					case '#' :
						state = STATE_LCOM;
						break;
					case '\n' :
						state = STATE_BCOM;
						break;
					default :
						if(isdigit(c))
							state = STATE_INT;
						else if((c >= 'a' && c <= 'z') || c == '_')
							state = STATE_ID;
                }


			case STATE_ASSIGN : 
				if(c == '=')
					length++;
					addChar(buffer, c, length);
					return TYPE_EQUAL;
				else{
					ungetc(c, f);
					return TYPE_ASSIGN;
				}


			case STATE_LESS :
				if(c == '=')
					length++;
					addChar(buffer, c, length);
					return TYPE_LESS_EQUAL;
				else{
					ungetc(c, f);
					return TYPE_LESS;
				}


			case STATE_GREAT :
				if(c == '='){
					length++;
					addChar(buffer, c, length);
					return TYPE_GREAT_EQUAL;
				else{
					ungetc(c, f);
					return TYPE_GREAT;
				}


			case STATE_NEG :
				if(c == '=')
					length++;
					addChar(buffer, c, length);
					return TYPE_NEG_EQUAL;
				else{
					ungetc(c, f);
					return TYPE_NEG;
				}


			case STATE_INT:
                firstChar(buffer,c);
                while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == '.')
                            length ++;
                            addChar(buffer,c,length)
                            state = STATE_INT_DOT;
                            break;
                        else if(c == 'e' || c == 'E'){
							state = STATE_INT_E;
							break;
						} else
                        {
                            ungetc(c,f);
                            return TYPE_INT;
                            break;
                        }
                    }
                    else{
                        length ++;
                        addChar(buffer,c,length);
                	}
                break;
				}


			case STATE_INT_E :
                if((c = fgetc(f)) == '+' || c == '-' || isdigit(c)){
                    length ++;
                    addChar(buffer,c,length);
                    state = STATE_INT_EXPO;
                } else {
					// ERROR
				}
				break;


			case STATE_INT_EXPO :
				while(1){
					if(isdigit(c = fgetc(f))){
						length++;
						addChar(buffer, c, length);
					}else{
						ungetc(c, f);
						return TYPE_INT_EXPO;
					}
				}


			case STATE_INT_DOT :
				if(isdigit(c)){
					state = STATE_FLOAT;
					break;
				}else{
					// ERROR
					break;
				}


            case STATE_FLOAT:
				while(1){
                    if(!isdigit(c = fgetc(f)))
                    {
                        if(c == 'e' || c == 'E'){
                            length ++;
                            addChar(buffer,c,length)
                            state = STATE_FLOAT_E;
                            break;
                        }
                        else
                        {
                            ungetc(c,f);
                            return TYPE_FLOAT;
                            break;
                        }
                    }
                    else{
                        length ++;
                        addChar(buffer,c,length);
                    }
                }


            case STATE_FLOAT_E:
                if((c = fgetc(f)) == '+' || c == '-' || isdigit(c)){
                    length ++;
                    addChar(buffer,c,length);
                    state = STATE_FLOAT_EXPO_PLUS;
                }else{
	                ungetc(c,f);
  	                
					// ERROR
					
               	}
				break;


			case STATE_FLOAT_EXPO_PLUS :
				if(isdigit(c = fgetc(f))){
					length ++;
					addChar(buffer, c , length);
					state = STATE_FLOAT_EXPO;
				}else{
					// ERROR
				}
				break;


			case STATE_FLOAT_EXPO :
				while(1){
					if(isdigit(c = fgetc(f))){
						length++;
						addChar(buffer, c, length);
					}else{
						ungetc(c, f);
						return TYPE_FLOAT_EXPO;
					}
				}


			case STATE_ID :
				while(1){
					c = fgetc(f);
					if(isalnum(c) || c = '_'){
						length++;
						addChar(buffer, c, length);
					}else if(c == '?' || c == '!'){
						length++;
						addChar(buffer, c, length);
						return STATE_FUNC_ID;
					}else{
						ungetc(c, f);
						return TYPE_ID;
					}
				}


				

	    }   //switch
    }   //cyklus

}
