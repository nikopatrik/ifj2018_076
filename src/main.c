#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "header/parser.h"
#include "header/symtable.h"
#include "header/scanner.h"

int main(){
    
    unsigned i = 10;
    htab_setup();
    myQueueInit();
    bool corr = true;
    corr =  parse();

    if(corr == true)
        printf("correct!\n");
    else
        printf("wrong!\n");

    myQueueFree();
}
