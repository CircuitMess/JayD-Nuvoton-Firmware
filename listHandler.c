#include <stdio.h>
#include <stdlib.h>
#include "N76E616.h"
#include "SFR_Macro.h"
#include "list.h"
#include "bool.h"

struct Node *rootNode = NULL;
uint8_t nodeNum = 0;
struct Node *sliderNode[3] = {NULL};
struct Node *lastNode = NULL;


void addNewNode(uint8_t devID, uint8_t val){
		
		clr_EA;
	
		if(nodeNum > 10){
			set_EA;
			return;
		}

		if((sliderNode[0] != NULL) && (devID == 0x20)){
			sliderNode[0]->value = val;
		}
		else if((sliderNode[1] != NULL) && (devID == 0x21)){
			sliderNode[1]->value = val;
		}
		else if((sliderNode[2] != NULL) && (devID == 0x22)){
			sliderNode[2]->value = val;
		}
		else{
			
			struct Node *newNode;
			
			newNode = (struct Node*)malloc(sizeof(struct Node));
			
			newNode->deviceID = devID;
			newNode->value = val;
			newNode->nextNode = NULL;
			
			nodeNum++;
		
			if(!rootNode){
				rootNode = lastNode = newNode;
			}
			
			else{
			
				lastNode->nextNode = newNode;
				lastNode = newNode;
			}
		
			
			if(sliderNode[0] == NULL && devID == 0x20){
				sliderNode[0] = newNode;
			}
			else if(sliderNode[1] == NULL && devID == 0x21){
				sliderNode[1] = newNode;
			}
			else if(sliderNode[2] == NULL && devID == 0x22){
				sliderNode[2] = newNode;
			}
		
	}
	
	set_EA;
}

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