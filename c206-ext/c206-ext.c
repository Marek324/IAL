/*
 *  Předmět: Algoritmy (IAL) - FIT VUT v Brně
 *  Rozšíření pro příklad c206.c (Dvousměrně vázaný lineární seznam)
 *  Vytvořil: Daniel Dolejška, září 2024
 */

#include "c206-ext.h"

bool error_flag;
bool solved;

//Helper functions

void init_prio_list(DLList *list, PacketPtr packet){
	QosPacketListPtr newList = (QosPacketListPtr) malloc(sizeof(QosPacketList));
		if(newList == NULL){
			//not sure what to do if malloc fails here
			return;
		}

		newList->priority = packet->priority;
		DLL_Init(newList->list);
		DLL_InsertFirst(newList->list, (long) packet);

		DLL_InsertFirst(list, (long) newList);
}


/**
 * Tato metoda simuluje příjem síťových paketů s určenou úrovní priority.
 * Přijaté pakety jsou zařazeny do odpovídajících front dle jejich priorit.
 * "Fronty" jsou v tomto cvičení reprezentovány dvousměrně vázanými seznamy
 * - ty totiž umožňují snazší úpravy pro již zařazené položky.
 * 
 * Parametr `packetLists` obsahuje jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Pokud fronta s odpovídající prioritou neexistuje, tato metoda ji alokuje
 * a inicializuje. Za jejich korektní uvolnení odpovídá volající.
 * 
 * V případě, že by po zařazení paketu do seznamu počet prvků v cílovém seznamu
 * překročil stanovený MAX_PACKET_COUNT, dojde nejdříve k promazání položek seznamu.
 * V takovémto případě bude každá druhá položka ze seznamu zahozena nehledě
 * na její vlastní prioritu ovšem v pořadí přijetí.
 * 
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param packet Ukazatel na strukturu přijatého paketu
 */
void receive_packet( DLList *packetLists, PacketPtr packet ) {
	DLL_First(packetLists); //set activeElement to first
	if(!DLL_IsActive(packetLists)){
		init_prio_list(packetLists, packet);
	} 

	DLLElementPtr ePtr = packetLists->activeElement;
	while (ePtr != NULL)
	{
		QosPacketListPtr pListPtr = (QosPacketListPtr) ePtr->data;
		
		if(pListPtr->priority == packet->priority){
			if (pListPtr->list->currentLength >= MAX_PACKET_COUNT){
				DLL_First(pListPtr->list);
				int c = 0;
				while (DLL_IsActive(pListPtr->list))
				{
					if(!(c++)&1)
						DLL_DeleteAfter(pListPtr); 
				}
			}
			DLL_InsertLast(pListPtr->list, (long) packet);
		}
	}
	

}

/**
 * Tato metoda simuluje výběr síťových paketů k odeslání. Výběr respektuje
 * relativní priority paketů mezi sebou, kde pakety s nejvyšší prioritou
 * jsou vždy odeslány nejdříve. Odesílání dále respektuje pořadí, ve kterém
 * byly pakety přijaty metodou `receive_packet`.
 * 
 * Odeslané pakety jsou ze zdrojového seznamu při odeslání odstraněny.
 * 
 * Parametr `packetLists` obsahuje ukazatele na jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Parametr `outputPacketList` obsahuje ukazatele na odeslané pakety (`PacketPtr`).
 * 
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param outputPacketList Ukazatel na seznam paketů k odeslání
 * @param maxPacketCount Maximální počet paketů k odeslání
 */
void send_packets( DLList *packetLists, DLList *outputPacketList, int maxPacketCount ) {
	solved = false; /* V případě řešení, smažte tento řádek! */
}
