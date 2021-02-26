#include "Typedef.h"

typedef struct Node{
    
    uint8_t deviceID;
    uint8_t value;
    struct Node *nextNode;
    
};

//uint8_t getNodeNumber();  
void addNewNode(uint8_t devID, uint8_t val);
//int searchList(uint8_t searchID);

void sendNodeNum();
void sendDevID();
void sendDevValue();
void freeElement();
