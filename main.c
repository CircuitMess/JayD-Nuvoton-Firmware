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

#define	SLAVE_ADDR	0x43	//	67_(10)

#define	STATUS_UPDATE	0x12	//	18_(10)
#define	EVENT_HANDLER	0x13	//	19_(10)
	
#define	DEBOUNCE_COUNTER	1000


typedef struct Node{
	
	uint8_t deviceID;
	uint8_t value;
	struct Node *nextNode;
	
};

struct Node *rootNode = NULL;
struct Node *eventNode = NULL;
uint8_t nodeNum = 0;

//int searchList(uint8_t searchID);
uint8_t getNodeNumber();	
void addNewNode(uint8_t devID, uint8_t val);

void initI2C();

void debounce(uint8_t num, bool val);
void encoders(uint8_t num);
void sliders();
	
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

uint8_t previousEncoderState[7] = {0};
signed int encoderPos[7] = {0};
signed int encoderLastPos[7] = {0};

uint8_t sliderValue[3] = {0};
uint8_t sliderLastValue[3] = {0};

bool switchEvent[9] = {false};
//bool sliderEvent[3] = {false};

uint16_t btnCount[9] = {0};

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

	INIT_ADC_SLIDER0();
	sliderLastValue[0] = sliderValue[0] = sliderRead();
	INIT_ADC_SLIDER1();
	sliderLastValue[1] = sliderValue[1] = sliderRead();
	INIT_ADC_SLIDER2();
	sliderLastValue[2] = sliderValue[2] = sliderRead();
	
	previousEncoderState[0] = ENCODER0_A;
	previousEncoderState[1] = ENCODER1_A;
	previousEncoderState[2] = ENCODER2_A;
	previousEncoderState[3] = ENCODER3_A;
	previousEncoderState[4] = ENCODER4_A;
	previousEncoderState[5] = ENCODER5_A;
	previousEncoderState[6] = ENCODER6_A;
	
	while(1){
		
		checkSwitchStatus();
		checkEncoderPosition();
		//checkSliderPosition();
		sliders();
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

/*
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
	sliderValue |= (ADCRL & 0x03);
	
	return sliderValue/4;
}

void debounce(uint8_t num, bool val){

	if(val){
		btnCount[num]++;
		
		if(btnCount[num] >= DEBOUNCE_COUNTER){
			
			switchEvent[num] = true;		
			addNewNode(num, val);

		}
	}
	else{
		btnCount[num]--;
		
		if(btnCount[num] <= 0){
		
			switchEvent[num] = false;
			addNewNode(num, val);
			
		}
	}
}

void checkSwitchStatus(){

	if(!SW0 && !switchEvent[0]){
		
		debounce(0, 1);
	}
	else if(SW0 && switchEvent[0]){
		
		debounce(0, 0);
	}
	
	
	if(!SW1 && !switchEvent[1]){
		
		debounce(1, 1);
		/*
		btnCount[1]++;
		
		if(btnCount[1] == DEBOUNCE_COUNTER){
		
			addNewNode(1, 1);
			switchEvent[1] = true;
		
		}
		*/
	}
	else{
		if(SW1 && switchEvent[1]){
			
			debounce(1, 0);
			/*
			btnCount[1]--;
			
			if(btnCount[1] == 0){
			
				addNewNode(1, 0);
				switchEvent[1] = false;
			}
			*/
		}
	}
						
	if(!ENCODER_SW_0 && !switchEvent[2]){
		
		debounce(2, 1);
	}
	else{
		if(ENCODER_SW_0 && switchEvent[2]){
			
			debounce(2, 0);
		}
	}
	
	if(!ENCODER_SW_1 && !switchEvent[3]){
		
		debounce(3, 1);
	}
	else{
		if(ENCODER_SW_1 && switchEvent[3]){
			
			debounce(3, 0);			
		}
	}
	
	if(!ENCODER_SW_2 && !switchEvent[4]){
		
		debounce(4, 1);
	}
	else{
		if(ENCODER_SW_2 && switchEvent[4]){
			
			debounce(4, 0);
		}
	}
	
	if(!ENCODER_SW_3 && !switchEvent[5]){
		
		debounce(5, 1);
	}
	else{
		if(ENCODER_SW_3 && switchEvent[5]){
			
			debounce(5, 0);
		}
	}
	
	if(!ENCODER_SW_4 && !switchEvent[6]){
		
		debounce(6, 1);
	}
	else{
		if(ENCODER_SW_4 && switchEvent[6]){
			
			debounce(6, 0);
		}
	}
		
	if(((P4>>5 & 0x01) != 0x01)  && !switchEvent[7]){							//	register P4 only byte addressable
		
		debounce(7, 1);
	}
	else{
		if(((P4>>5 & 0x01) == 0x01) && switchEvent[7]){
			
			debounce(7, 0);
		}
	}
	
	if(!ENCODER_SW_6 && !switchEvent[8]){
		
		debounce(8, 1);
	}
	else{
		if(ENCODER_SW_6 && switchEvent[8]){
			
			debounce(8, 0);
		}
	}
}

void sliders(){
	
	uint8_t i;
	
	for(i = 0; i < 3; i++){
	
		if(i == 0)
			INIT_ADC_SLIDER0();
		if(i == 1)
			INIT_ADC_SLIDER1();
		if(i == 2)
			INIT_ADC_SLIDER2();
		
		sliderValue[i] = sliderRead();
	
		if(abs(sliderValue[i] - sliderLastValue[i]) > 1 ){
	
			addNewNode((0x20 | i), sliderValue[i]);
		
			sliderLastValue[i] = sliderValue[i];
		}
		
		ADCCON1&=CLR_BIT0;
	}

}
/*
void checkSliderPosition(){

	INIT_ADC_SLIDER0();
	sliderValue[0] = sliderRead();
	
	if(abs(sliderValue[0] - sliderLastValue[0]) > 4 ){
	
		//searchList(0x20);
		addNewNode(0x20, sliderValue[0]);
		
		sliderLastValue[0] = sliderValue[0];
	}

	INIT_ADC_SLIDER1();
	sliderValue[1] = sliderRead();
	
	if(abs(sliderValue[1] - sliderLastValue[1]) > 4 ){
		
		//searchList(0x21);
		addNewNode(0x21, sliderValue[1]);
		
		sliderLastValue[1] = sliderValue[1];
	}

	INIT_ADC_SLIDER2();
	sliderValue[2] = sliderRead();
	
	if(abs(sliderValue[2] - sliderLastValue[2]) > 4 ){
		
		//searchList(0x22);
		addNewNode(0x22, sliderValue[2]);
		
		sliderLastValue[2] = sliderValue[2];
	}

}
*/

void checkEncoderPosition(void){

	if(previousEncoderState[0] != ENCODER0_A){
		
		if(ENCODER0_B != ENCODER0_A)
			encoderPos[0]++;
		else
			encoderPos[0]--;
		
		//searchList(0x10);
		addNewNode(0x10, (signed int)(encoderPos[0]-encoderLastPos[0]));
	
		encoderLastPos[0] = encoderPos[0];
	
		previousEncoderState[0] = ENCODER0_A;
	}
	
	
	if(previousEncoderState[1] != ENCODER1_A){
	
		if(ENCODER1_B != ENCODER1_A)
			encoderPos[1]++;
		else
			encoderPos[1]--;
		
		//searchList(0x11);
		addNewNode(0x11, (signed int)(encoderPos[1]-encoderLastPos[1]));
		
		encoderLastPos[1] = encoderPos[1];
	
		previousEncoderState[1] = ENCODER1_A;
	}
	
	
	if(previousEncoderState[2] != ENCODER2_A){
	
		if(ENCODER2_B != ENCODER2_A)
			encoderPos[2]++;
		else
			encoderPos[2]--;
		
		//searchList(0x12);
		addNewNode(0x12, (signed int)(encoderPos[2]-encoderLastPos[2]));
		
		encoderLastPos[2] = encoderPos[2];
	
		previousEncoderState[2] = ENCODER2_A;
	}
	
	
	if(previousEncoderState[3] != ENCODER3_A){
	
		if(ENCODER3_B != ENCODER3_A)
			encoderPos[3]++;
		else
			encoderPos[3]--;
		
		//searchList(0x13);
		addNewNode(0x13, (signed int)(encoderPos[3]-encoderLastPos[3]));
		
		encoderLastPos[3] = encoderPos[3];
	
		previousEncoderState[3] = ENCODER3_A;
	}
	
	
	if(previousEncoderState[4] != ENCODER4_A){
	
		if(ENCODER4_B != ENCODER4_A)
			encoderPos[4]++;
		else
			encoderPos[4]--;
		
		//searchList(0x14);
		addNewNode(0x14, (signed int)(encoderPos[4]-encoderLastPos[4]));
		
		encoderLastPos[4] = encoderPos[4];
	
		previousEncoderState[4] = ENCODER4_A;
	}
	
		
	if(previousEncoderState[5] != ((P4>>6 && 0x01))){
	
		if(ENCODER5_B != ((P4>>6 && 0x01)))
			encoderPos[5]++;
		else
			encoderPos[5]--;
		
		//searchList(0x15);
		addNewNode(0x15, (signed int)(encoderPos[5]-encoderLastPos[5]));
		
		encoderLastPos[5] = encoderPos[5];
	
		previousEncoderState[5] = (P4>>6 && 0x01);
		//previousEncoderState[5] = ENCODER5_A;
	}
	
	
	if(previousEncoderState[6] != ENCODER6_A){
	
		if(ENCODER6_B != ENCODER6_A)
			encoderPos[6]++;
		else
			encoderPos[6]--;
		
		//searchList(0x16);
		addNewNode(0x16, (signed int)(encoderPos[6]-encoderLastPos[6]));
		
		encoderLastPos[6] = encoderPos[6];
	
		previousEncoderState[6] = ENCODER6_A;
	}
}

