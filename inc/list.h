#include "Typedef.h"

typedef struct Node{
    
    uint8_t deviceID;
    uint8_t value;
    struct Node *nextNode;
    
};

void addNewNode(uint8_t devID, uint8_t val);
void sendNodeNum();
void sendDevID();
void sendDevValue();
void freeElement();

void sendPotValue(uint8_t potID);
