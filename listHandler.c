#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct Node *rootNode = NULL;
uint8_t nodeNum = 0;

uint8_t getNodeNumber(){

	struct Node *temp = rootNode;
	uint8_t nodeNumber = 0;
	
	if(!rootNode)
		return 0;

	
	while(temp != NULL){
		
		nodeNumber++;
		
		temp = temp->nextNode;
	}
	
	return nodeNumber;
	
}

void addNewNode(uint8_t devID, uint8_t val){
		
		struct Node *newNode;
	
		if(nodeNum > 5)
			return;
	
		newNode = (struct Node*)malloc(sizeof(struct Node));

		newNode->deviceID = devID;
		newNode->value = val;
		newNode->nextNode = NULL;
			
		if(!rootNode){
			rootNode = newNode;
		}
		
		else{
			struct Node *currentNode = rootNode;
			
			while(currentNode->nextNode != NULL){
				currentNode = currentNode->nextNode;
			}
			currentNode->nextNode = newNode;
				
		}
		
		//nodeNum = getNodeNumber();
		nodeNum++;	
}		

/*	Search List
int searchList(uint8_t searchID){

	struct Node *temp = rootNode;
	struct Node *prev = rootNode;
	
	if(!rootNode)
		return 0;
	
	while(temp){
	
		if(temp->deviceID == searchID){
		
			if(temp == rootNode)
				rootNode = temp->nextNode;
			else
				prev->nextNode = temp->nextNode;
			
			return 1;
		}
		
		prev = temp;
		temp = temp->nextNode;
		
	}
	
	return 0;
}
*/