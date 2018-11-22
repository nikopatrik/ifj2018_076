#include <string.h>
#include <stdbool.h>

#include "header/scanner.h"
#include "header/parseexp.h"

// TODO
// Opravit precedencnu tabulku
// Zmenit pracu s buffrom aby bol schopny zistit nazov premenej
// Implementovat pracu s tabulkov symbolov
// Naucit sa generovat funkcie.

// Precedence table:
static char precedence_table[TERM_COUNT][TERM_COUNT] = {
//    + , - , * , / , ( , ) , < ,<= , > , <=, ==, !=, id, $
    {'>','>','<','<','<','>','>','>','>','>','>','>','<','>'}, //+
    {'>','>','<','<','<','>','>','>','>','>','>','>','<','>'}, //-
    {'>','>','>','>','<','>','>','>','>','>','>','>','<','>'}, //*
    {'>','>','>','>','<','>','>','>','>','>','>','>','<','>'}, ///
    {'<','<','<','<','<','=','<','<','<','<','<','<','<',' '}, //(
    {'>','>','>','>',' ','>','>','>','>','>','>','>',' ','>'}, //)
    {'<','<','<','<','<','>',' ',' ',' ',' ',' ',' ','<','>'}, //<
    {'<','<','<','<','<','>',' ',' ',' ',' ',' ',' ','<','>'}, //<=
    {'<','<','<','<','<','>',' ',' ',' ',' ',' ',' ','<','>'}, //>
    {'<','<','<','<','<','>',' ',' ',' ',' ',' ',' ','<','>'}, //>=
    {'<','<','<','<','<','>',' ',' ',' ',' ',' ',' ','<','>'}, //==
    {'<','<','<','<','<','>',' ',' ',' ',' ',' ',' ','<','>'}, //!=
    {'>','>','>','>',' ','>','>','>','>','>','>','>',' ','>'}, //id
    {'<','<','<','<','<',' ','<','<','<','<','<','<','<',' '}, //$
};

#define INFO_SIZE 256
#define BAD_MAPPING -1

void es_init(TEvalStack *t)
{
    t->stack_top = NULL;
    t->count = 0;
}

void es_push(TEvalStack *t, bool is_terminal, bool is_nonterminal,
       tokenType token, char *attribute, char info)
{
    TStackItemPtr tmp = malloc(sizeof(struct stack_item));
    if( tmp == NULL)
        return;
    tmp->is_terminal = is_terminal;
    tmp->is_nonterminal = is_nonterminal;
    tmp->token = token;
    tmp->token_attribute = attribute; //TODO skontrolovat
    tmp->info = info;

    tmp->prev = t->stack_top;
    t->stack_top = tmp;
    t->count++;
}

TStackItem es_top_pop(TEvalStack *t)
{
    TStackItem item = *(t->stack_top);
    TStackItemPtr tmp = t->stack_top;
    t->stack_top = tmp->prev;
    free(tmp);
    t->count--;
    return item;
}

TStackItemPtr es_top_terminal(TEvalStack *t)
{
    for(TStackItemPtr i = t->stack_top; i !=NULL; i = i->prev)
        if(i->is_terminal)
            return i;
    return NULL;
}

void es_terminal_preinsert(TEvalStack *t,bool is_terminal,
        bool is_nonterminal,tokenType token,char *attribute,
        char info)
{
    TStackItemPtr prev = NULL;
    TStackItemPtr i; 
    for(i = t->stack_top; i != NULL && !(i->is_terminal);
            i= i->prev)
        prev = i;

    if(prev == NULL)
        es_push(t,is_terminal,is_nonterminal,token,attribute,info);
    else {
        TStackItemPtr newitem = malloc(sizeof(
                    struct stack_item));
        if(newitem == NULL)
            return;

        newitem->is_terminal = is_terminal;
        newitem->is_nonterminal = is_nonterminal;
        newitem->token = token;
        newitem->token_attribute = attribute;
        newitem->info = info;
        newitem->prev = i;
        prev->prev = newitem;
    }
}
                

bool es_is_empty(TEvalStack *t)
{
    return (t->count == 0) && (t->stack_top == NULL);
}

bool es_endcondition(TEvalStack *t,tokenType end, char* attribute,
        tokenType got, char* gottenAttr)
{
    bool c_flag = false;
    if(end == got){
        if( attribute == NULL || gottenAttr == NULL){
            if(attribute == gottenAttr)
                c_flag = true;
            else
                return false;
        }
        else if(strcmp(attribute,gottenAttr) == 0)
            c_flag = true;
        else
            return false;
    }
    else
        return false;
    
    
    if(c_flag && t->stack_top->prev->info == '$' &&
            t->stack_top->info != 0)
        return true;
    else
        return false;
}

int get_table_index(tokenType type)
{
    int token_mapping_to_table[] = {0,1,2,3,-1,10,8,6,9,7,11,4,5,
        -1,-1,-1,12,12,12,12,12,-1,-1,-1,-1,-1,-1};
    return token_mapping_to_table[type];
}


void rule_operators(TEvalStack *t);
void rule_brackets(TEvalStack *t);
void rule_term_to_noterm(TEvalStack *t);

bool expression_parse(TEvalStack *t, FILE *f, tokenType end,
        char* end_token_attribute)
{
    tokenType token;
    char *buffer = NULL;

    //Initialization Part
    es_push(t, true, false, end, end_token_attribute,'$');
    TStackItemPtr a = es_top_terminal(t);
    token = getNextToken(&buffer,f);
    int mapped_index1, mapped_index2;
    TStackItemPtr top1,top2;

    do {
        //Mapping Part
        if( a->token == end && a->token_attribute == end_token_attribute)
            mapped_index1 = 13; //TODO define constant
        else
            mapped_index1 = get_table_index(a->token); 
        mapped_index2 = get_table_index(token); 
        if(mapped_index2 == BAD_MAPPING){
            if(token == end) // TODO dorob aj porovnanie attributov.
                mapped_index2 = 13;
            else
                return false;
        }
        
        //Searching in Precedence Table
        switch(precedence_table[mapped_index1][mapped_index2]){
            case '=':
                es_push(t, true, false, token, buffer, 0);
                a = es_top_terminal(t);
                token = getNextToken(&buffer,f);
                break;
            case '<':
                es_terminal_preinsert(t,false,false, -1,NULL,'<');
                es_push(t,true,false, token, buffer, 0);
                a = es_top_terminal(t);
                token = getNextToken(&buffer,f);
                break;
            case '>':
                top1= t->stack_top;
                top2= top1->prev;
                if(top2->info == '<')
                    if(top1->is_terminal)
                        rule_term_to_noterm(t);
                    else
                        return false;
                else{
                    TStackItemPtr top3 = top2->prev;
                    if(top2->is_terminal){
                        if(!(top1->is_nonterminal || top3->is_nonterminal))
                            return false;

                        rule_operators(t);
                    }
                    else if(top2->is_nonterminal){
                        if(!(top1->token == TYPE_R_BRE || 
                                    top3->token ==TYPE_L_BRE))
                            return false;

                        rule_brackets(t);
                    }
                }
                a = es_top_terminal(t);
                break;
            default:
                return false;
        }
    }while(!es_endcondition(t,end,end_token_attribute,token,buffer));
    return true;
}

void rule_operators(TEvalStack *t)
{
    //TODO opravit aby bolo schopne generovat kod.
    es_top_pop(t);
    es_top_pop(t);
    es_top_pop(t);
    es_top_pop(t);

    es_push(t,false,true,-1,NULL,'E');
}

void rule_term_to_noterm(TEvalStack *t)
{
    TStackItem term = es_top_pop(t);
    es_top_pop(t);
    
    es_push(t,false,true, term.token, term.token_attribute, term.info);
}

void rule_brackets(TEvalStack *t)
{
    es_top_pop(t);
    TStackItem nonterm = es_top_pop(t);
    es_top_pop(t);
    es_top_pop(t);
    
    es_push(t,nonterm.is_terminal,nonterm.is_nonterminal,nonterm.token,
            nonterm.token_attribute,nonterm.info);
}
    
