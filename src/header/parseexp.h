#ifndef _PARSE_EXP_H_
#define _PARSE_EXP_H_

#define TERM_COUNT 14

#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"
#include "instrlist.h"

typedef struct stack_item {
    bool is_terminal;
    bool is_nonterminal;
    tokenType token;
    char *token_attribute;
    char info;
    struct stack_item *prev;
}TStackItem;

typedef TStackItem *TStackItemPtr;


typedef struct eval_stack {
    struct stack_item *stack_top;
    unsigned count;
}TEvalStack;

bool expression_parse(tokenType end, char* end_token_attribute, htab_t *symtab, 
        tDLList *l_main);


#endif
