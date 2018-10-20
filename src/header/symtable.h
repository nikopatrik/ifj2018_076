/**
 * @file: symtable.h
 * @date: 20.10.2018
 * @Author: Peter Hornak - xhorna14
 * Collaborators:
 *          Nikolas Patrik - xpatri00
 *          Matej Jancek - xjance00
 *          Robert Hubinak - xhubin03
 *
 * @brief Hlavickovy subor tabulky symbolov
 */

#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct htab_listitem Item v hash tabulke
 */
struct htab_listitem{
    char* key;      /**< Nazov premennej pouzity na hashovanie */
    unsigned data;      
    struct htab_listitem *next;     /**< Ukazatel na dalsi prvok v indexe */
};

/**
 *  @struct hashtable Tabulka symoblov reprezentovana pomocou hashovacej
 *  tabulky
 */
typedef struct hashtable{
    unsigned arr_size;      /**< Predefinovany pocet indexov hashovacej tabulky */
    unsigned size;      /**< Celkovy pocet prvkov */
    struct htab_listitem* arr[];    /**< Pole ukazatelov na item */
} htab_t;


/**
 * @brief Hashovacia funkcia
 * @param *key Retazec
 */
unsigned htab_hash_function(const char *str);

/**
 * @brief Inicializacia tabulky o danej velkosti
 * @param arr_size Velkost pola
 */
htab_t* htab_init(unsigned arr_size);

/**
 * @brief Vytvorenie a inicializacia tabulky na zaklade dat z tabulky t2, t2 zostane prazdna
 * @param newsize Nova velkost
 */
htab_t* htab_move(unsigned newsize, htab_t* t2);

/**
 * @brief Vracia pocet prvkov zoznamu
 */
unsigned htab_size(htab_t* t); //size

/**
 * @brief Vrati pocet zoznamov v tabulke
 */
unsigned htab_bucket_count(htab_t* t); //arr_size

/**
 * @brief Hlada slovo v tabulke, ak ho najde zvacsi data, inak ho vlozi do tabulky
 * @param *key Retazec
 */
struct htab_listitem* htab_lookup_add(htab_t *t, const char* key);

/**
 * @brief Hlada slovo v tabulke, ak najde vrati pointer, inak NULL
 * @param *key Retazec
 */
struct htab_listitem* htab_find(htab_t *t, const char* key);

/**
 * @brief Zavola funkciu pre kazdy prvok tabulky
 * @param *func Ukazatel na funkciu
 */
void htab_foreach(htab_t* t, void (*func)(const char *, unsigned *));

/**
 * @brief Vyhlada a vymaze zadane slovo z tabulky
 * @param *key Retazec
 */
bool htab_remove(htab_t* t, const char* key);

/**
 * @brief Vymaze zoznam a jeho prvky
 */
void htab_clear(htab_t* t);

/**
 * @brief Uvolni alokovanu pamat pre tabulku
 */
void htab_free(htab_t* t);
