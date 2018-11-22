#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"

#ifndef _PARSERH_
#define _PARSERH_

bool parse(htab_t*, FILE*);

bool st_list(tokenType*, char**, FILE*);

bool stat(tokenType*, char**, FILE*);

bool else_st_list(tokenType*, char**, FILE*);

bool end_st_list(tokenType*, char**, FILE*);

bool id_item(tokenType*, char**, FILE*);

bool assign(tokenType*, char**, FILE*);

bool next(tokenType*, char**, FILE*);

bool func(tokenType*, char**, FILE*);

bool param(tokenType*, char**, FILE*);

bool next_param(tokenType*, char**, FILE*);

bool bracket(tokenType*, char**, FILE*);

bool brc_param(tokenType*, char**, FILE*);

bool next_brc_param(tokenType*, char**, FILE*);

bool expr(tokenType*, char**, FILE*);

#endif
