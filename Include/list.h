#include "Typedef.h"

typedef struct Node{
	
	uint8_t deviceID;
	uint8_t value;
	struct Node *nextNode;
	
};

uint8_t getNodeNumber();	
void addNewNode(uint8_t devID, uint8_t val);

extern struct Node *rootNode;
extern uint8_t nodeNum;
extern struct Node *sliderNode[3];
extern struct Node *lastNode;

