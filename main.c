#include <stdio.h>
#include <stdlib.h>
#include "N76E616.h"
#include "Version.h"
#include "Typedef.h"
#include "Define.h"
#include "SFR_Macro.h"
#include "Common.h"
#include "Delay.h"
#include "Input.h"


#ifndef bool
	typedef uint8_t bool;
	#define true 1
	#define false 0
#endif

#define		I2C_ACK					0
#define		I2C_NACK				1

#define		SLAVE_ADDR			0x43			//	67_(10)

#define		STATUS_UPDATE		0x12			//	18_(10)
#define		EVENT_HANDLER		0x13			//	19_(10)


typedef struct Node{
	
	uint8_t deviceID;
	uint8_t value;
	struct Node *nextNode;
	
};

struct Node *rootNode = NULL;
struct Node *eventNode = NULL;
uint8_t nodeNum = 0;

int searchList(uint8_t searchID);
uint8_t getNodeNumber();	
void addNewNode(uint8_t devID, uint8_t val);

void initI2C();
	
void checkSwitchStatus(void);
void checkEncoderPosition(void);
void checkSliderPosition(void);
	
unsigned int sliderRead(void);

uint8_t dataReceived = 0;
uint8_t dummyDataReceived = 0;
uint8_t statusReceived = 0;
uint8_t eventReceived = 0;



uint8_t rxDataCnt = 0;
uint8_t txDataCnt = 0;
bool lastDataTx = false;
bool lastDataRx = false;
uint8_t expectedEventsInQueue = 0;

uint8_t switchStatus = 0;
uint8_t encoderSwitchStatus = 0;

bool previousEncoder0State = false;
bool previousEncoder1State = false;
bool previousEncoder2State = false;
bool previousEncoder3State = false;
bool previousEncoder4State = false;
bool previousEncoder5State = false;
bool previousEncoder6State = false;

uint8_t encoder0Pos = 0;
uint8_t encoder0LastPos = 0;
uint8_t encoder1Pos = 0;
uint8_t encoder1LastPos = 0;
uint8_t encoder2Pos = 0;
uint8_t encoder2LastPos = 0;
uint8_t encoder3Pos = 0;
uint8_t encoder3LastPos = 0;
uint8_t encoder4Pos = 0;
uint8_t encoder4LastPos = 0;
uint8_t encoder5Pos = 0;
uint8_t encoder5LastPos = 0;
uint8_t encoder6Pos = 0;
uint8_t encoder6LastPos = 0;

signed int deltaEnc = 0;

uint8_t slider0Value = 0;
uint8_t slider0LastValue = 0;
uint8_t slider1Value = 0;
uint8_t slider1LastValue = 0;
uint8_t slider2Value = 0;
uint8_t slider2LastValue = 0;

bool switchEvent[9] = {false,false,false,false,false,false,false,false,false};
bool sliderEvent[3] = {false,false,false};


void I2C_ISR(void) interrupt 6
{
		struct Node *tempNode = NULL;
	
	
    switch (I2STAT)
    {
        case 0x00:											//	bus error
            STO = 1;
						
            break;

        case 0x60:											//	slave receive address ACK
            AA = 1;
            break;
				
        case 0x68:											//	slave receive arbitration lost
						AA = 0;
						STA = 1;
            break;

        case 0x80:											//	slave receive data ACK
            dataReceived = I2DAT;
						
						if(dataReceived == STATUS_UPDATE){

							statusReceived = dataReceived;
							lastDataRx = true;
						}
						else if(dataReceived == EVENT_HANDLER){
								
							eventReceived = dataReceived;
							lastDataRx = false;
						}
						
						if (eventReceived == EVENT_HANDLER){
								
								lastDataRx = true;
								expectedEventsInQueue = dataReceived;
						}					
						
								
						if(lastDataRx){
							AA = 0;
							lastDataRx = false;
						}
						else
							AA = 1;
						
            break;

        case 0x88:											//	slave receive data NACK
            dummyDataReceived = I2DAT;
            AA = 1;
            break;

        case 0xA0:											//	slave transmit repeat start or stop
            AA = 1;
            break;

        case 0xA8:											//	slave transmit address ACK
            I2DAT = SLAVE_ADDR;
            AA = 1;
            break;
        
        case 0xB8:											//	slave transmit data ACK
					
						if(statusReceived == STATUS_UPDATE){
							
							I2DAT = nodeNum;	
							lastDataTx = true;
							statusReceived = 0;
						}
						else if(eventReceived == EVENT_HANDLER){
						
							eventNode = rootNode;
							
							if(txDataCnt >= expectedEventsInQueue*2){
								lastDataTx = true;
								txDataCnt = 0;
								rootNode = eventNode;
								eventReceived = 0;
							}
							else{
								lastDataTx = false;
								
								if(++txDataCnt % 2 == 1)
									I2DAT = eventNode->deviceID;
								else{ 
									I2DAT = eventNode->value;
							
									tempNode = eventNode;
									eventNode = eventNode->nextNode;
									free(tempNode);
									nodeNum--;
								}
							}
							
						}
						
						if(lastDataTx){
							AA = 0;
							lastDataTx = false;
						}
						else
							AA = 1;
						
            break;

        case 0xC0:											//	slave transmit data NACK
						AA = 1;
            break; 

        case 0xC8:											//	slave transmit last data ACK
            AA = 1;
            break;        
    }

    SI = 0;
		while(STO);
}

void main(void){
	
	Set_All_GPIO_Quasi_Mode;

	SW_INPUT_MODE_INIT();
	ENCODER_SW_INPUT_MODE_INIT();
	
	initI2C();														// Initialize i2c communication
	/*
	INIT_ADC_SLIDER0();
	slider0Value = sliderRead();
	slider0LastValue = slider0Value;
	INIT_ADC_SLIDER1();
	slider1Value = sliderRead();
	slider1LastValue = slider1Value;
	INIT_ADC_SLIDER2();
	slider2Value = sliderRead();
	slider2LastValue = slider2Value;
	*/
	
	previousEncoder0State = ENCODER0_A;
	previousEncoder1State = ENCODER1_A;
	previousEncoder2State = ENCODER2_A;
	previousEncoder3State = ENCODER3_A;
	previousEncoder4State = ENCODER4_A;
	previousEncoder5State = ENCODER5_A;
	previousEncoder6State = ENCODER6_A;
	
	while(1){
		
		checkSwitchStatus();
		checkEncoderPosition();
		//checkSliderPosition();
	}
}


void initI2C(){

		P23_OpenDrain_Mode;
    P24_OpenDrain_Mode;
	
    P23 = 1;
    P24 = 1;
    
    //set_P0SR_6;                            //set SCL (P06) is  Schmitt triggered input select.
 
    set_EI2C;                               //	enable I2C interrupt
    set_EA;																	//	enable interrupts - global

    I2ADDR = SLAVE_ADDR<<1;                 //define own slave address
    
		set_I2CEN;                              //	enable I2C
		set_AA;																	//	for the first ACK
}


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

		struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
	
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
		


unsigned int sliderRead(){

	uint16_t sliderValue = 0x0000;
	
	clr_ADCF;
	set_ADCS;
	
	while(!ADCF);
	
	sliderValue = ADCRH;
	sliderValue <<= 2;
	sliderValue |= ADCRL;
	
	return sliderValue/4;
}


void checkSwitchStatus(){

	if(SW0 == 0 && !switchEvent[0]){
		
		addNewNode(0, 1);
		
		switchEvent[0] = true;

	}
	else{
		if(switchEvent[0] && SW0){
			
			addNewNode(0, 0);
			
			switchEvent[0] = false;
		}
	}

	
	if(SW1 == 0 && !switchEvent[1]){
		
		addNewNode(1, 1);
		
		switchEvent[1] = true;
		//switchStatus|=SET_BIT1;
	}else{
		if(switchEvent[1] && SW1 == 1){
			
			addNewNode(1, 0);
			
			switchEvent[1] = false;
		}
		//switchStatus&=~SET_BIT1;
	}
						
	if(ENCODER_SW_0 == 0 && !switchEvent[2]){
		
		addNewNode(2, 1);
		
		switchEvent[2] = true;
		//encoderSwitchStatus|=SET_BIT0;
	}else{
		if(switchEvent[2] && ENCODER_SW_0 == 1){
			
			addNewNode(2, 0);
			
			switchEvent[2] = false;
		}
		//encoderSwitchStatus&=~SET_BIT0;
	}
	if(ENCODER_SW_1 == 0 && !switchEvent[3]){
		
		addNewNode(3, 1);
		
		switchEvent[3] = true;
		//encoderSwitchStatus|=SET_BIT1;
	}else{
		if(switchEvent[3] && ENCODER_SW_1 == 1){
			
			addNewNode(3, 0);
			
			switchEvent[3] = false;
		}
		//encoderSwitchStatus&=~SET_BIT1;
	}
	if(ENCODER_SW_2 == 0 && !switchEvent[4]){
		
		addNewNode(4, 1);
		
		switchEvent[4] = true;
		//encoderSwitchStatus|=SET_BIT2;
	}else{
		if(switchEvent[4] && ENCODER_SW_2 == 1){
			
			addNewNode(4, 0);
			
			switchEvent[4] = false;
		}
		//encoderSwitchStatus&=~SET_BIT2;
	}
	if(ENCODER_SW_3 == 0 && !switchEvent[5]){
		
		addNewNode(5, 1);
		
		switchEvent[5] = true;
		//encoderSwitchStatus|=SET_BIT3;
	}else{
		if(switchEvent[5] && ENCODER_SW_3 == 1){
			
			addNewNode(5, 0);
			
			switchEvent[5] = false;
		}
		//encoderSwitchStatus&=~SET_BIT3;
	}
	if(ENCODER_SW_4 == 0 && !switchEvent[6]){
		
		addNewNode(6, 1);
		
		switchEvent[6] = true;
		//encoderSwitchStatus|=SET_BIT4;
	}else{
		if(switchEvent[6] && ENCODER_SW_4 == 1){
			
			addNewNode(6, 0);
			
			switchEvent[6] = false;
		}
		//encoderSwitchStatus&=~SET_BIT4;
	}
	//if(((P4>>5 && 0x01) != 0x01)){							//	register P4 only byte addressable
	
	if(((P4>>5 & 0x01) != 0x01)  && !switchEvent[7]){							//	register P4 only byte addressable
		
		addNewNode(7, 1);
		
		switchEvent[7] = true;
		//encoderSwitchStatus|=SET_BIT5;
	}else{
		if(switchEvent[7] && ((P4>>5 & 0x01) == 0x01)){
			
			addNewNode(7, 0);
			
			switchEvent[7] = false;
		}
		//encoderSwitchStatus&=~SET_BIT5;
	}
	if(ENCODER_SW_6 == 0 && !switchEvent[8]){
		
		addNewNode(8, 1);
		
		switchEvent[8] = true;
		//encoderSwitchStatus|=SET_BIT6;
	}else{
		if(switchEvent[8] && ENCODER_SW_6 == 1){
			
			addNewNode(8, 0);
			
			switchEvent[8] = false;
		}
		//encoderSwitchStatus&=~SET_BIT6;
	}
}

void checkSliderPosition(){

	INIT_ADC_SLIDER0();
	slider0Value = sliderRead();
	if(slider0Value != slider0LastValue){
	
		//searchList(0x20);
		addNewNode(0x20, slider0Value);
		
		slider0LastValue = slider0Value;
	}

	INIT_ADC_SLIDER1();
	slider1Value = sliderRead();
	if(slider1Value != slider1LastValue){
		
		//searchList(0x21);
		addNewNode(0x21, slider0Value);
		
		slider1LastValue = slider1Value;
	}

	INIT_ADC_SLIDER2();
	slider2Value = sliderRead();
	if(slider2Value != slider2LastValue){
		
		//searchList(0x22);
		addNewNode(0x22, slider0Value);
		
		slider2LastValue = slider2Value;
	}

}


void checkEncoderPosition(void){

	/*	TODO -> SEARCH LIST FOR ENCODERS NEEDS TO ADD LAST VALUE TO THE NEW VALUE	*/
	
	//if(previousEncoder0State == 0 && ENCODER0_A == 1){
	
	if(previousEncoder0State != ENCODER0_A){
		
		if(ENCODER0_B == 0)
			encoder0Pos++;
		else
			encoder0Pos--;
		
		//searchList(0x10);
		addNewNode(0x10, (signed int)(encoder0Pos-encoder0LastPos));
	
		encoder0LastPos = encoder0Pos;
	
		previousEncoder0State = ENCODER0_A;
	}
	
	
	
	if(previousEncoder1State != ENCODER1_A){
	
		if(ENCODER1_B == 0)
			encoder1Pos++;
		else
			encoder1Pos--;
		
		//searchList(0x11);
		addNewNode(0x11, (signed int)(encoder1Pos-encoder1LastPos));
		
		encoder1LastPos = encoder1Pos;
	
		previousEncoder1State = ENCODER1_A;
	}
	

	
	
	if(previousEncoder2State != ENCODER2_A){
	
		if(ENCODER2_B == 0)
			encoder2Pos++;
		else
			encoder2Pos--;
		
		//searchList(0x12);
		addNewNode(0x12, (signed int)(encoder2Pos-encoder2LastPos));
		
		encoder2LastPos = encoder2Pos;
	
		previousEncoder2State = ENCODER2_A;
	}
	

	
	if(previousEncoder3State != ENCODER3_A){
	
		if(ENCODER3_B == 0)
			encoder3Pos++;
		else
			encoder3Pos--;
		
		//searchList(0x13);
		addNewNode(0x13, (signed int)(encoder3Pos-encoder3LastPos));
		
		encoder3LastPos = encoder3Pos;
	
		previousEncoder3State = ENCODER3_A;
	}
	

	
	
	if(previousEncoder4State != ENCODER4_A){
	
		if(ENCODER4_B == 0)
			encoder4Pos++;
		else
			encoder4Pos--;
		
		//searchList(0x14);
		addNewNode(0x14, (signed int)(encoder4Pos-encoder4LastPos));
		
		encoder4LastPos = encoder4Pos;
	
		previousEncoder4State = ENCODER4_A;
	}
	
	
	
	//if(previousEncoder5State == 0 && ((P4>>6 && 0x01) == 0x01)){
	
	if(previousEncoder5State != ((P4>>6 && 0x01))){
	
		if(ENCODER5_B == 0)
			encoder5Pos++;
		else
			encoder5Pos--;
		
		//searchList(0x15);
		addNewNode(0x15, (signed int)(encoder5Pos-encoder5LastPos));
		
		encoder5LastPos = encoder5Pos;
	
		previousEncoder5State = (P4>>6 && 0x01);
	}
	
	
	if(previousEncoder6State != ENCODER6_A){
	
		if(ENCODER6_B == 0)
			encoder6Pos++;
		else
			encoder6Pos--;
		
		//searchList(0x16);
		addNewNode(0x16, (signed int)(encoder6Pos-encoder6LastPos));
		
		encoder6LastPos = encoder6Pos;
	
		previousEncoder6State = ENCODER6_A;
	}
}

