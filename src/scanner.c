#include "header/queue.h"
#include "header/scanner.h"
#include "header/garbagecollector.h"


static tQueue* my_queue;

unsigned length;

void myQueueInit(){
    my_queue = (tQueue*) gb_malloc(sizeof(tQueue));
    if(my_queue == NULL)
        return;

    queueInit(my_queue);
}

void myQueueFree(){
    gb_free(my_queue);
}

void firstChar(char **buffer, char c)
{
    *buffer = (char*)gb_malloc(2 * sizeof(char));
    (*buffer)[0] = c;
    (*buffer)[1] = '\0';
}

void addChar(char **buffer, char c, unsigned length)
{
	*buffer = (char*)gb_realloc(*buffer, (length+1) * sizeof(char));
    (*buffer)[length - 1] = c;
    (*buffer)[length] = '\0';
}

void ungetToken(int type, char *buffer)
{
    queueUp(my_queue, buffer, type);
}


int getNextToken(char **buffer)
{
    FILE* f = stdin;

	if(!queueEmpty(my_queue)){
        tokenType temp = my_queue->temp_arr[my_queue->f_index];
        queueGet(my_queue, buffer);
		return temp;
	}

    *buffer = NULL;

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
						if(isdigit(c)){
							state = STATE_INT;
                        }else if((c >= 'a' && c <= 'z') || c == '_'){
							state = STATE_ID;
                        }
                        else
                            gb_exit_process(1);
                        break;
                }
            break;

            case STATE_QUOT :
                while((c=fgetc(f)) != '"'){
					//	If there is no second quote, end at EOF with error
					if(c == EOF)
						gb_exit_process(1);
					//	'\"' does not end string
					else if(c == '\\'){
						c = fgetc(f);
						//  All posible escape sequentions
						switch (c) {
							//	quote
							case '"' :
								if(length == 1){
									firstChar(buffer, '"');
									length++;
								}else
									addChar(buffer, '"', length++);
								continue;
							//	new line	
							case 'n' :
								if(length == 1){
									firstChar(buffer, '\n');
									length++;
								}else
									addChar(buffer, '\n', length++);
								continue;
							//	tab
							case 't' :
								if(length == 1){
									firstChar(buffer, '\t');
									length++;
								}else
									addChar(buffer, '\t', length++);
								continue;
							// space
							case 's' :
								if(length == 1){
									firstChar(buffer, ' ');
									length++;
								}else
									addChar(buffer, ' ', length++);
								continue;
							// backslash
							case '\\' :
								if(length == 1){
									firstChar(buffer, '\\');
									length++;
								}else
									addChar(buffer, '\\', length++);
								continue;
							// hex
							case 'x' :
								//	Check if char == {0-9} || {A-F} 
								if(((c = fgetc(f)) >= 48 && c <= 57) || (c >= 65 && c <= 70)){
									char hex_escape[3];		// hexadecimal number
									int char_escape;		// decimal value of char

									hex_escape[0] = c;		//	add first hex number
									//	Check if next number is also hex
									if(((c = fgetc(f)) >= 48 && c <= 57) || (c >= 65 && c <= 70)){
										hex_escape[1] = c;
										hex_escape[2] = '\0';
									}else{
										ungetc(c, f);
										hex_escape[1] = '\0';
									}
									//	Convert hex to decimal
									char_escape = strtol(hex_escape, NULL, 16);
									if(length == 1){
										firstChar(buffer, char_escape);
										length++;
									}else
										addChar(buffer, char_escape, length++);
								}else{
									ungetc(c, f);
									gb_exit_process(1);
								}
								continue;
							//	If char after '\' is something else, skip '\' and save only next char
							default :
								c = fgetc(f);
						}
                    }
                    if(length == 1){
                        firstChar(buffer, c);
                        length++;
                    }else{
                        addChar(buffer,c,length);
                        length ++;
                    }
                }
                if(c == '"' && length == 1)
                    return TYPE_QUOT_EMPTY;
                else
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


			case STATE_NEG :
				if((c = fgetc(f)) == '='){
					return TYPE_NEG_EQUAL;
				}else{
					ungetc(c, f);
					gb_exit_process(1);
				}


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
					gb_exit_process(1);
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
					gb_exit_process(1);
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
					gb_exit_process(1);
				}
				break;


			case STATE_FLOAT_EXPO_PLUS :
				if(isdigit(c = fgetc(f))){
					length ++;
					addChar(buffer, c , length);
					state = STATE_FLOAT_EXPO;
				}else
					gb_exit_process(1);
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
                            strcmp(*buffer,"then")  == 0 ||
                            strcmp(*buffer,"while") == 0 )
                            return TYPE_KEYWORD;

                        if(strcmp(*buffer,"nil") == 0)
                            return TYPE_NIL;

                        if( strcmp(*buffer,"print") == 0 ||
                            strcmp(*buffer,"length")== 0 ||
                            strcmp(*buffer,"inputi")== 0 ||
                            strcmp(*buffer,"inputs")== 0 ||
                            strcmp(*buffer,"inputf")== 0 ||
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
					gb_exit_process(1);
				break;


			case STATE_BCOM_B :
				if(c == 'e')
					state = STATE_BCOM_E;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_E :
				if(c == 'g')
					state = STATE_BCOM_G;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_G :
				if(c == 'i')
					state = STATE_BCOM_I;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_I :
				if(c == 'n')
					state = STATE_BCOM_N;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_N :
				if(isspace(c))
					state = STATE_BCOM_COM;
				else
					gb_exit_process(1);
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
					gb_exit_process(1);
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
