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

//Macros for better code readability
#define F_ELEM list->firstElement
#define A_ELEM list->activeElement
#define L_ELEM list->lastElement
#define LEN list->currentLength

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
	F_ELEM = NULL;
	A_ELEM = NULL;
	L_ELEM = NULL;
	LEN = 0;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	while (LEN != 0){
		DLLElementPtr tmp = F_ELEM; //store first element
		F_ELEM = F_ELEM->nextElement; //move first element pointer to next one
		free(tmp); //free stored element
		LEN--;
	}
	L_ELEM = NULL;
	A_ELEM = NULL;
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
	DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
	if (newElement == NULL){
		DLL_Error();
		return; //end the function if malloc failed
	}

	newElement->data = data;
	newElement->previousElement = NULL;
	newElement->nextElement = F_ELEM;

	if (LEN == 0) L_ELEM = newElement; //if the list is empty, set the last element to the new one
	else F_ELEM->previousElement = newElement; //set the previous element of the first element to the new one

	F_ELEM = newElement; //set the first element to the new one
	LEN++;
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
	DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
	if (newElement == NULL){
		DLL_Error();
		return; //end the function if malloc failed
	}

	newElement->data = data;
	newElement->nextElement = NULL;
	newElement->previousElement = L_ELEM;

	if (LEN == 0) F_ELEM = newElement; //if the list is empty, set the last element to the new one
	else L_ELEM->nextElement = newElement; //set the previous element of the first element to the new one

	L_ELEM = newElement; //set the first element to the new one
	LEN++;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	A_ELEM = F_ELEM; //can be NULL
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	A_ELEM = L_ELEM; //can be NULL
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, long *dataPtr ) {
	if (LEN == 0){
		DLL_Error();
		return; //end the function if the list is empty
	}

	*dataPtr = F_ELEM->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, long *dataPtr ) {
	if (LEN == 0){
		DLL_Error();
		return; //end the function if the list is empty
	}

	*dataPtr = L_ELEM->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (LEN == 0) return; //no element to delete
	if (A_ELEM == F_ELEM) A_ELEM = NULL; //if the active element is the first one, set it to NULL

	F_ELEM = F_ELEM->nextElement; //set the first element to the next one, can be NULL
	if (F_ELEM != NULL) {
		free(F_ELEM->previousElement); //free the previous element
		F_ELEM->previousElement = NULL; //set the previous element to NULL
	} else {
		free(L_ELEM); //free the first element
		L_ELEM = NULL; //set the last element to NULL
	}

	LEN--;
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (LEN == 0) return; //no element to delete
	if (A_ELEM == L_ELEM) A_ELEM = NULL; //if the active element is the last one, set it to NULL

	L_ELEM = L_ELEM->previousElement; //set the next element to the previous one, can be NULL
	if (L_ELEM != NULL) {
		free(L_ELEM->nextElement); //free the next element
		L_ELEM->nextElement = NULL; //set the next element to NULL
	} else {
		free(F_ELEM); //free the last element
		F_ELEM = NULL; //set the first element to NULL
	}

	LEN--;
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if (A_ELEM == NULL || A_ELEM == L_ELEM) return; //no element to delete

	A_ELEM->nextElement = A_ELEM->nextElement->nextElement; //set the next element to the next one, can be NULL
	if (A_ELEM->nextElement != NULL) {
		free(A_ELEM->nextElement->previousElement); //free the element
		A_ELEM->nextElement->previousElement = A_ELEM; //set the previous element of next element to the active one
	} else {
		free(L_ELEM); //free the last element
		L_ELEM = A_ELEM; //set the last element to the active one
	}

	LEN--;
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if (A_ELEM == NULL || A_ELEM == F_ELEM) return; //no element to delete

	A_ELEM->previousElement = A_ELEM->previousElement->previousElement; //set the previous element to the previous one, can be NULL
	if (A_ELEM->previousElement != NULL) {
		free(A_ELEM->previousElement->nextElement); //free the element
		A_ELEM->previousElement->nextElement = A_ELEM; //set the previous element of next element to the active one
	} else {
		free(F_ELEM); //free the last element
		F_ELEM = A_ELEM; //set the last element to the active one
	}

	LEN--;
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
	newElement->nextElement = A_ELEM->nextElement; //can be NULL
	newElement->previousElement = A_ELEM;
	if (A_ELEM->nextElement != NULL) A_ELEM->nextElement->previousElement = newElement; //set the previous element of next element to the new one
	else L_ELEM = newElement; //set the last element to the new one

	A_ELEM->nextElement = newElement; //set the next element of the active one to the new one
	
	LEN++;
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
	newElement->previousElement = A_ELEM->previousElement; //can be NULL
	newElement->nextElement = A_ELEM;
	if (A_ELEM->previousElement != NULL) A_ELEM->previousElement->nextElement = newElement; //set the previous element of next element to the new one
	else F_ELEM = newElement; //set the last element to the new one

	A_ELEM->previousElement = newElement; //set the next element of the active one to the new one
	
	LEN++;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, long *dataPtr ) {
	if (A_ELEM == NULL){
		DLL_Error();
		return; //end the function if the list isn't active
	}

	*dataPtr = A_ELEM->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, long data ) {
	if (A_ELEM == NULL) return; //no active element to set it's value

	A_ELEM->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (A_ELEM == NULL) return; //no active element to move to the next one

	A_ELEM = A_ELEM->nextElement; //set the active element to the next one, can be NULL 
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (A_ELEM == NULL) return; //no active element to move to the previous one

	A_ELEM = A_ELEM->previousElement; //set the active element to the previous one, can be NULL
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
	return A_ELEM != NULL;
}

/* Konec c206.c */
