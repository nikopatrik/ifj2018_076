#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "header/parser.h"
#include "header/symtable.h"
#include "header/scanner.h"
#include "header/generate.h"

int main(){
    
    int i = -1;
    htab_setup();
    myQueueInit();
    myStackInit();
    bool corr = true;
    corr =  parse();
    //myQueueFree();
    gb_release_resources();
    if(corr == true){
        printf("correct!\n");
        return 0;
    }
    else
        printf("wrong!\n");
}
