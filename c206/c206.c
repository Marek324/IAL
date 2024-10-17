/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
** uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
	list->currentLength = 0;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	while (list->currentLength != 0){
		DLLElementPtr tmp = list->firstElement; //store first element
		list->firstElement = list->firstElement->nextElement; //move first element pointer to next one
		free(tmp); //free stored element
		list->currentLength--;
	}
	list->lastElement = NULL;
	list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, long data ) {
	DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement)); //create new element
	if (newElement == NULL){
		DLL_Error();
		return; //end the function if malloc failed
	}

	newElement->data = data; //assign data to the new element
	newElement->previousElement = NULL; //set the previous element to NULL
	newElement->nextElement = list->firstElement; //set the next element to the first element, can be NULL

	if (list->currentLength == 0) list->lastElement = newElement; //if the list is empty, set the last element to the new one
	else list->firstElement->previousElement = newElement; //set the previous element of the first element to the new one

	list->firstElement = newElement; //set the first element to the new one
	list->currentLength++;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, long data ) {
	DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement)); //create new element
	if (newElement == NULL){
		DLL_Error();
		return; //end the function if malloc failed
	}

	newElement->data = data; //assign data to the new element
	newElement->nextElement = NULL; //set the next element to NULL
	newElement->previousElement = list->lastElement; //set the previous element to the last element, can be NULL

	if (list->currentLength == 0) list->firstElement = newElement; //if the list is empty, set the last element to the new one
	else list->lastElement->nextElement = newElement; //set the previous element of the first element to the new one

	list->lastElement = newElement; //set the first element to the new one
	list->currentLength++;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement; //set the first element as active
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement; //set the last element as active
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, long *dataPtr ) {
	if (list->currentLength == 0){
		DLL_Error();
		return; //end the function if the list is empty
	}

	*dataPtr = list->firstElement->data; //assign the data of the first element to the dataPtr
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, long *dataPtr ) {
	if (list->currentLength == 0){
		DLL_Error();
		return; //end the function if the list is empty
	}

	*dataPtr = list->lastElement->data; //assign the data of the last element to the dataPtr
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->currentLength == 0) return; //no element to delete
	if (list->activeElement == list->firstElement) list->activeElement = NULL; //if the active element is the first one, set it to NULL

	list->firstElement = list->firstElement->nextElement; //set the first element to the next one, can be NULL
	if (list->firstElement != NULL) {
		free(list->firstElement->previousElement); //free the previous element
		list->firstElement->previousElement = NULL; //set the previous element to NULL
	} else {
		free(list->lastElement); //free the first element
		list->lastElement = NULL; //set the last element to NULL
	}

	list->currentLength--;
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (list->currentLength == 0) return; //no element to delete
	if (list->activeElement == list->lastElement) list->activeElement = NULL; //if the active element is the last one, set it to NULL

	list->lastElement = list->lastElement->previousElement; //set the next element to the previous one, can be NULL
	if (list->lastElement != NULL) {
		free(list->lastElement->nextElement); //free the next element
		list->lastElement->nextElement = NULL; //set the next element to NULL
	} else {
		free(list->firstElement); //free the last element
		list->firstElement = NULL; //set the first element to NULL
	}

	list->currentLength--;
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if (list->activeElement == NULL || list->activeElement == list->lastElement) return; //no element to delete

	list->activeElement->nextElement = list->activeElement->nextElement->nextElement; //set the next element to the next one, can be NULL
	if (list->activeElement->nextElement != NULL) {
		free(list->activeElement->nextElement->previousElement); //free the element
		list->activeElement->nextElement->previousElement = list->activeElement; //set the previous element of next element to the active one
	} else {
		free(list->lastElement); //free the last element
		list->lastElement = list->activeElement; //set the last element to the active one
	}

	list->currentLength--;
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if (list->activeElement == NULL || list->activeElement == list->firstElement) return; //no element to delete

	list->activeElement->previousElement = list->activeElement->previousElement->previousElement; //set the previous element to the previous one, can be NULL
	if (list->activeElement->previousElement != NULL) {
		free(list->activeElement->previousElement->nextElement); //free the element
		list->activeElement->previousElement->nextElement = list->activeElement; //set the previous element of next element to the active one
	} else {
		free(list->firstElement); //free the last element
		list->firstElement = list->activeElement; //set the last element to the active one
	}

	list->currentLength--;
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, long data ) {
	DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
	if (newElement == NULL){
		DLL_Error();
		return; //end the function if malloc failed
	}

	newElement->data = data;
	newElement->nextElement = list->activeElement->nextElement; //can be NULL
	newElement->previousElement = list->activeElement;
	if (list->activeElement->nextElement != NULL) list->activeElement->nextElement->previousElement = newElement; //set the previous element of next element to the new one
	else list->lastElement = newElement; //set the last element to the new one

	list->activeElement->nextElement = newElement; //set the next element of the active one to the new one
	
	list->currentLength++;
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, long data ) {
	DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
	if (newElement == NULL){
		DLL_Error();
		return; //end the function if malloc failed
	}

	newElement->data = data;
	newElement->previousElement = list->activeElement->previousElement; //can be NULL
	newElement->nextElement = list->activeElement;
	if (list->activeElement->previousElement != NULL) list->activeElement->previousElement->nextElement = newElement; //set the previous element of next element to the new one
	else list->firstElement = newElement; //set the last element to the new one

	list->activeElement->previousElement = newElement; //set the next element of the active one to the new one
	
	list->currentLength++;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, long *dataPtr ) {
	if (list->activeElement == NULL){
		DLL_Error();
		return; //end the function if the list isn't active
	}

	*dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, long data ) {
	if (list->activeElement == NULL) return; //no active element to set it's value

	list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement == NULL) return; //no active element to move to the next one

	list->activeElement = list->activeElement->nextElement; //set the active element to the next one, can be NULL 
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement == NULL) return; //no active element to move to the previous one

	list->activeElement = list->activeElement->previousElement; //set the active element to the previous one, can be NULL
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
bool DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL;
}

/* Konec c206.c */
