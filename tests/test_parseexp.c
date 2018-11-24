#include <stdio.h>
#include <stdlib.h>

#include "../src/header/scanner.h"
#include "../src/header/parseexp.h"
#include "../src/header/garbagecollector.h"

int main()
{
    garbage_collector_init();
    myQueueInit();
    if(expression_parse( TYPE_KEYWORD, "then"))
        puts("Syntax correct!!!");
    else
        puts("Syntax not correct!!!");

    myQueueFree();
    gb_release_resources();
        return 0;
}
