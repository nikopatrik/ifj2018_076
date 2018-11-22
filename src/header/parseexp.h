#ifndef _PARSE_EXP_H_
#define _PARSE_EXP_H_

#define TERM_COUNT 14

#include <stdbool.h>

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

void es_init(TEvalStack *t);

bool expression_parse(TEvalStack *t, FILE *f, tokenType end,
        char* end_token_attribute);


#endif
