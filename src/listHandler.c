#include <stdio.h>
#include <stdlib.h>
#include "N76E616.h"
#include "SFR_Macro.h"
#include "list.h"
#include "bool.h"

struct Node* __near rootNode = NULL;
__near uint8_t nodeNum = 0;
struct Node* __near sliderNode[3] = {NULL};
struct Node* __near lastNode = NULL;

struct Node nodes[10];
uint8_t head = 0;
uint8_t foot = 0;

struct Node* giveNode(){

	if((head == foot) && (foot != (head + 1) % 10) && head < 10){
		struct Node *node = &nodes[head];

		head = (head + 1) % 10;

		node->nextNode = NULL;
		return node;
	}
	else
		return NULL;
}

void addNewNode(uint8_t devID, uint8_t val){
        
        clr_EA;

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
            
            newNode = giveNode();
            if(!newNode){
            	set_EA;
				return;
			}


            newNode->deviceID = devID;
            newNode->value = val;
            newNode->nextNode = NULL;

        
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

void sendNodeNum(){
    
    I2DAT = abs(head - foot);
}

void sendDevID(){
    
    I2DAT = rootNode->deviceID;
}

void sendDevValue(){
    
    I2DAT = rootNode->value;
}

void freeElement(){

    struct Node *eventNode = NULL;
    
    if(!rootNode){
        return;
    }
    
    if(rootNode == lastNode){
        lastNode = NULL;
    }


    if(rootNode == sliderNode[0]){
        sliderNode[0] = NULL;
    }
    else if(rootNode == sliderNode[1]){
        sliderNode[1] = NULL;
    }
    else if(rootNode == sliderNode[2]){
        sliderNode[2] = NULL;
    }

    eventNode = rootNode->nextNode;

    rootNode = eventNode;

    foot = (foot + 1) % 10;
}
