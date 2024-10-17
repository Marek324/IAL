/*
 *  Předmět: Algoritmy (IAL) - FIT VUT v Brně
 *  Rozšíření pro příklad c206.c (Dvousměrně vázaný lineární seznam)
 *  Vytvořil: Daniel Dolejška, září 2024
 */

#include "c206-ext.h"

bool error_flag;
bool solved;

//Helper functions

typedef enum {
	FIRST,
	BEFORE,
	AFTER
} InsertPosition;

void insert_prio_list(DLList *list, PacketPtr packet, InsertPosition pos){
	QosPacketListPtr newList = (QosPacketListPtr) malloc(sizeof(QosPacketList));
	if(newList == NULL){
		error_flag = true;
		return;
	}

	newList->list = (DLList *) malloc(sizeof(DLList));
	if(newList->list == NULL){
		error_flag = true;
		free(newList);
		return;
	}

	DLL_Init(newList->list);
	newList->priority = packet->priority;
	DLL_InsertFirst(newList->list, (long) packet);

	switch (pos)
	{
		case FIRST:
			DLL_InsertFirst(list, (long) newList);
			break;
		case BEFORE:
			DLL_InsertBefore(list, (long) newList);
			break;
		case AFTER:
			DLL_InsertAfter(list, (long) newList);
			break;
	}
}

// as packets with the same priority are stored as they come in, every second (even indexes) needs to be deleted
void delete_even_packets(DLList *list){ 
	DLL_First(list);
	int c = 0;
	while (DLL_IsActive(list))
	{
		if(!((c++)&1))
			DLL_DeleteAfter(list); 
		DLL_Next(list);
	}
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

	// List is empty when inserting first packet
	if (!DLL_IsActive(packetLists)){
		insert_prio_list(packetLists, packet, FIRST);
		return;
	}

	while (DLL_IsActive(packetLists)){
		long prioList;
		DLL_GetValue(packetLists, &prioList);
		long prio = ((QosPacketListPtr) prioList)->priority;

		if (prio == packet->priority) 
		{
			if (((QosPacketListPtr) prioList)->list->currentLength >= MAX_PACKET_COUNT){
				delete_even_packets(((QosPacketListPtr) prioList)->list);
			}
			DLL_InsertLast(((QosPacketListPtr) prioList)->list, (long) packet);
			return;
		} 
		
		else if (prio > packet->priority) 
		{
			if (packetLists->activeElement->nextElement != NULL){
				DLL_Next(packetLists);
			} else {
				insert_prio_list(packetLists, packet, FIRST);
				return;
			}
		} 
		
		else 
		{
			insert_prio_list(packetLists, packet, BEFORE);
			return;
		}
			
	}
	
}

void copy_packet_out(DLList *outList, PacketPtr packet){
	PacketPtr newPacket = (PacketPtr) malloc(sizeof(Packet));
	if(newPacket == NULL){
		error_flag = true;
		return;
	}

	newPacket->id = packet->id;
	newPacket->priority = packet->priority;
	
	DLL_InsertLast(outList, (long) newPacket);
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
void send_packets( DLList *packetLists, DLList *outputPacketList, int maxPacketCount ){
	DLL_First(packetLists);
	if(!DLL_IsActive(packetLists)){ // No queued packets
		return;
	}

	while (DLL_IsActive(packetLists) && maxPacketCount > 0)
	{
		QosPacketListPtr prioList = (QosPacketListPtr) packetLists->activeElement->data;

		DLL_First(prioList->list);
		while (DLL_IsActive(prioList->list) && maxPacketCount > 0)
		{
			copy_packet_out(outputPacketList, (PacketPtr) prioList->list->activeElement->data);
			DLL_DeleteFirst(prioList->list);
			DLL_First(prioList->list);	
			maxPacketCount--;
		}


		DLL_Next(packetLists);
	}
	

}
