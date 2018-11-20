#include "header/list.h"
#include "header/scanner.h"

FILE *f;

static int type_of_token = -1;

unsigned length;

void firstChar(char **buffer, char c)
{
    *buffer = (char*)malloc(2 * sizeof(char));
    (*buffer)[0] = c;
    (*buffer)[1] = '\0';
}

void addChar(char **buffer, char c, unsigned length)
{
	*buffer = (char*)realloc(*buffer, (length+1) * sizeof(char));
    (*buffer)[length - 1] = c;
    (*buffer)[length] = '\0';
}

void ungetToken(int type)
{

	type_of_token = type; 

}


int getNextToken(char **buffer, FILE *f)
{
    

	if(type_of_token <> -1){
		int tmp = type_of_token;
		type_of_token = -1;
		return tmp;
	}

	free(buffer);

	automataState state = STATE_BEGIN;
    char c;
    length = 1;
    c=fgetc(f);
    while(1){
        switch (state) {
            case STATE_BEGIN:
                while(isspace(c) && c != '\n'){
                    c=fgetc(f);
                }
                    state = STATE_BEGIN;
                switch(c){
                    case EOF :
                        return TYPE_EOF;
                        break;
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
                        return TYPE_R_BRE;
                        break;
                    case '"' :
                        state = STATE_QUOT;
                        break;
                    case ',' :
                        return TYPE_COMMA;
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
					// case '!' :
						// state = STATE_NEG;
						// break;
					case '#' :
						state = STATE_LCOM;
						break;
					case '\n' :
						state = STATE_BCOM;
						break;
					default :
						if(isdigit(c)){
							state = STATE_INT;
                        }else if((c >= 'a' && c <= 'z') || c == '_'){
							state = STATE_ID;
                        }
                        break;
                }
            break;

            case STATE_QUOT :
                if((c=fgetc(f)) == '"')
                    return TYPE_QUOT_EMPTY;
                else{
                    firstChar(buffer,c);
                }

                while((c=fgetc(f)) != '"'){
                    length ++;
                    addChar(buffer,c,length);
                }
                return TYPE_QUOT;


			case STATE_ASSIGN :
				if((c = fgetc(f))== '='){
					return TYPE_EQUAL;
				}else{
					ungetc(c, f);
					return TYPE_ASSIGN;
				}


			case STATE_LESS :
				if((c = fgetc(f)) == '='){
					return TYPE_LESS_EQUAL;
				}else{
					ungetc(c, f);
					return TYPE_LESS;
				}


			case STATE_GREAT :
				if((c = fgetc(f)) == '='){
					return TYPE_GREAT_EQUAL;
				}else{
					ungetc(c, f);
					return TYPE_GREAT;
				}


			// case STATE_NEG :
				// if((c = fgetc(f)) == '='){
					// return TYPE_NEG_EQUAL;
				// }else{
					// ungetc(c, f);
					// return TYPE_NEG;
				// }


			case STATE_INT:
                firstChar(buffer,c);
                while(1){
                    if(!isdigit(c = fgetc(f))){
                        if(c == '.'){
                            length ++;
                            addChar(buffer,c,length);
                            state = STATE_INT_DOT;
                            break;
						}else if(c == 'e' || c == 'E'){
							state = STATE_INT_E;
							break;
						}else{
                            ungetc(c,f);
                            return TYPE_INT;
                            break;
                        }
                    }else{
                        length ++;
                        addChar(buffer,c,length);
                	}
				}


			case STATE_INT_E :
                if((c = fgetc(f)) == '+' || c == '-' || isdigit(c)){
                    length ++;
                    addChar(buffer,c,length);
                    state = STATE_INT_EXPO;
                } else
					return TYPE_ERROR;
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
				if(isdigit(c = fgetc(f)))
					state = STATE_FLOAT;
				else
					return TYPE_ERROR;
				break;


            case STATE_FLOAT:
				while(1){
                    if(!isdigit(c = fgetc(f))){
                        if(c == 'e' || c == 'E'){
                            length ++;
                            addChar(buffer,c,length);
                            state = STATE_FLOAT_E;
                            break;
                        }else{
                            c=fgetc(f);
                            ungetc(c,f);
                            return TYPE_FLOAT;
                            break;
                        }
                    }else{
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
					return TYPE_ERROR;
				}
				break;


			case STATE_FLOAT_EXPO_PLUS :
				if(isdigit(c = fgetc(f))){
					length ++;
					addChar(buffer, c , length);
					state = STATE_FLOAT_EXPO;
				}else
					return TYPE_ERROR;
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
                firstChar(buffer,c);
				while(1){
					c = fgetc(f);
					if(isalnum(c) || c == '_'){
						length++;
						addChar(buffer, c, length);
					}else if(c == '?' || c == '!'){
						length++;
						addChar(buffer, c, length);
						return TYPE_FUNC_ID;
					}else{
						ungetc(c, f);
                        if( strcmp(*buffer,"def")   == 0 ||
                            strcmp(*buffer,"do")    == 0 ||
                            strcmp(*buffer,"else")  == 0 ||
                            strcmp(*buffer,"end")   == 0 ||
                            strcmp(*buffer,"if")    == 0 ||
                            strcmp(*buffer,"not")   == 0 ||
                            strcmp(*buffer,"nil")   == 0 ||
                            strcmp(*buffer,"then")  == 0 ||
                            strcmp(*buffer,"while") == 0 )
                            return TYPE_KEYWORD;

                        if( strcmp(*buffer,"print") == 0 ||
                            strcmp(*buffer,"length")== 0 ||
                            strcmp(*buffer,"substr")== 0 ||
                            strcmp(*buffer,"ord")   == 0 ||
                            strcmp(*buffer,"chr")   == 0 )
                            return TYPE_PRE_FUNC;
                        return TYPE_ID;
					}
				}


			case STATE_LCOM :
				while(1){
					if((c = fgetc(f)) == '\n'){
						state = STATE_BEGIN;
						break;
					}
				}
				break;


			case STATE_BCOM :
				if(c == '=')
					state = STATE_BCOM_EQUALS;
				else{
                    return TYPE_EOL;
				}
				break;


			case STATE_BCOM_EQUALS :
				if(c == 'b')
					state = STATE_BCOM_B;
				else
					return TYPE_ERROR;
				break;


			case STATE_BCOM_B :
				if(c == 'e')
					state = STATE_BCOM_E;
				else
					return TYPE_ERROR;
				break;


			case STATE_BCOM_E :
				if(c == 'g')
					state = STATE_BCOM_G;
				else
					return TYPE_ERROR;
				break;


			case STATE_BCOM_G :
				if(c == 'i')
					state = STATE_BCOM_I;
				else
					return TYPE_ERROR;
				break;


			case STATE_BCOM_I :
				if(c == 'n')
					state = STATE_BCOM_N;
				else
					return TYPE_ERROR;
				break;


			case STATE_BCOM_N :
				if(isspace(c))
					state = STATE_BCOM_COM;
				else
					return TYPE_ERROR;
				break;


			case STATE_BCOM_COM :
				while(1){
					if((c = fgetc(f)) == 10){
						state = STATE_BCOM_COM_EOL;
						break;
					}
				}
				break;


			case STATE_BCOM_COM_EOL :
				if(c == '=')
					state = STATE_BCOM_COM_EQUALS;
				else
					state = STATE_BCOM_COM;
				break;


			case STATE_BCOM_COM_EQUALS :
				if(c == 'e')
					state = STATE_BCOM_COM_E;
				else
					state = STATE_BCOM_COM;
				break;


			case STATE_BCOM_COM_E :
				if(c == 'n')
					state = STATE_BCOM_COM_N;
				else
					state = STATE_BCOM_COM;
				break;


			case STATE_BCOM_COM_N :
				if(c == 'd')
					state = STATE_BCOM_COM_D;
				else
					state = STATE_BCOM_COM;
				break;


			case STATE_BCOM_COM_D :
				if(isspace(c))
					state = STATE_BCOM_COM_2;
				else
					return TYPE_ERROR;
				break;


			case STATE_BCOM_COM_2 :
				while(1){
					if((c = fgetc(f)) == 10){
						state = STATE_BEGIN;
						break;
					}
				}
				break;



	    }   //switch
    }   //cyklus

}
