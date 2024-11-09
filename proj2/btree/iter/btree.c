/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  (*tree) = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  while (tree != NULL) {
    if (tree->key == key) {
      *value = &(tree->content);
      return true;
    } else {
      if (key < tree->key) {
        tree = tree->left;
      } else {
        tree = tree->right;
      }
    }
  }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  while((*tree) != NULL) {
    if ((*tree)->key == key) {
      free((*tree)->content.value);
      (*tree)->content = value;
      return;
    } else {
      if (key < (*tree)->key) {
        tree = &((*tree)->left);
      } else {
        tree = &((*tree)->right);
      }
    }
  }
  (*tree) = malloc(sizeof(bst_node_t));
  (*tree)->key = key;
  (*tree)->content = value;
  (*tree)->left = NULL;
  (*tree)->right = NULL;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  while((*tree)->right != NULL){
    tree = &((*tree)->right);
  }

  target->key = (*tree)->key;
  free(target->content.value);
  target->content = (*tree)->content;
  bst_node_t *tmp = *tree;
  *tree = (*tree)->left;
  free(tmp);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  while((*tree) != NULL){ 
    if (key < ((*tree)->key)) { // go left
      tree = &((*tree)->left);
    } else if (key > ((*tree)->key)){ // go right
      tree = &((*tree)->right);
    } else { // found
      if ((*tree)->left == NULL && (*tree)->right == NULL ) { // no children
        bst_node_t *tmp = *tree;
        *tree = NULL;
        free(tmp->content.value);
        free(tmp);
      } else if ((*tree)->left == NULL) { // right child
        bst_node_t *tmp = *tree;
        *tree = (*tree)->right;
        free(tmp->content.value);
        free(tmp);
      } else if ((*tree)->right == NULL) { // left child
        bst_node_t *tmp = *tree;
        *tree = (*tree)->left;
        free(tmp->content.value);
        free(tmp);
      } else { // both children
        bst_replace_by_rightmost(*tree, &((*tree)->left));
      }
    }
  } 
  // key not found, nothing happens
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  stack_bst_t *s = malloc(sizeof(stack_bst_t));
  if (s == NULL) {
    return;
  }
  stack_bst_init(s);
  stack_bst_push(s, *tree);
  *tree = NULL;
  while(!stack_bst_empty(s)) {
    bst_node_t *node = stack_bst_pop(s);
    if(node == NULL) {
      continue;
    }
    stack_bst_push(s, node->left);
    stack_bst_push(s, node->right);
    
    free(node->content.value);  
    free(node);
  
  }
  free(s);
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  while(tree != NULL) {
    bst_add_node_to_items(tree, items);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
  
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t *s = malloc(sizeof(stack_bst_t));
  if (s == NULL) {
    return;
  }
  stack_bst_init(s);
  bst_leftmost_preorder(tree, s, items);

  while(!stack_bst_empty(s)){
    tree = stack_bst_pop(s);
    bst_leftmost_preorder(tree->right, s, items);
  }

  free(s);
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while(tree != NULL) {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t *s = malloc(sizeof(stack_bst_t));
  if (s == NULL) {
    return;
  }
  stack_bst_init(s);
  bst_leftmost_inorder(tree, s);

  while(!stack_bst_empty(s)){
    tree = stack_bst_pop(s);
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, s);
  }
  
  free(s);
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while(tree != NULL) {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t *s = malloc(sizeof(stack_bst_t));
  if (s == NULL) {
    return;
  }
  stack_bst_init(s);
  stack_bool_t *s_visit = malloc(sizeof(stack_bool_t));
  if (s_visit == NULL) {
    free(s);
    return;
  }
  stack_bool_init(s_visit);

  bst_leftmost_postorder(tree, s, s_visit);
  while(!stack_bst_empty(s)){
    bst_node_t *node = stack_bst_pop(s);
    bool first = stack_bool_pop(s_visit);
    if (first) {
      stack_bst_push(s, node);
      stack_bool_push(s_visit, false);
      bst_leftmost_postorder(node->right, s, s_visit);
      
    } else {
      bst_add_node_to_items(node, items);
    }
  }

  free(s);
  free(s_visit);
}
