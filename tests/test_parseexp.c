#include <stdio.h>
#include <stdlib.h>

#include "../src/header/scanner.h"
#include "../src/header/parseexp.h"

int main()
{
    TEvalStack stack;
    es_init(&stack);

    if(expression_parse(&stack, stdin, TYPE_EOL, NULL))
        puts("Syntax correct!!!");
    else
        puts("Syntax not correct!!!");
    return 0;
}
