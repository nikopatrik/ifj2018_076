#include <stdio.h>
#include <stdlib.h>

#include "../src/header/scanner.h"
#include "../src/header/parseexp.h"

int main()
{
    myQueueInit();
    if(expression_parse( TYPE_KEYWORD, "then"))
        puts("Syntax correct!!!");
    else
        puts("Syntax not correct!!!");

    myQueueFree();
    return 0;
}
