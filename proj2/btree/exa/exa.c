/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný 
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 * 
 */
#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) 
{
    char c;
    for (int i = 0; (c = input[i]) != '\0'; i++) {
        if (!(c >= 'a' && c <= 'c') && c != ' ') c = '_';

        bst_node_content_t *count = &(bst_node_content_t){.value = NULL, .type = INTEGER};
        if (bst_search(*tree, c, &count)) (*((int *)count->value))++;
        else{
            count->value = malloc(sizeof(int));
            (*((int *)count->value)) = 1;
            bst_insert(tree, c, (bst_node_content_t){.value = count->value, .type = INTEGER});
        }
    }
}
