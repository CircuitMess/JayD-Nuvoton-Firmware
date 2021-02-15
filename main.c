#include <stdio.h>
#include <stdlib.h>
#include "N76E616.h"
#include "Version.h"
#include "Typedef.h"
#include "Define.h"
#include "SFR_Macro.h"
#include "Common.h"
#include "Delay.h"
#include "i2c.h"
#include "buttons.h"
#include "sliders.h"
#include "encoders.h"
#include "list.h"

uint8_t dataReceived = 0;
uint8_t dummyDataReceived = 0;
uint8_t statusReceived = 0;
uint8_t eventReceived = 0;
bool idVerificationStatus = false;
	
uint8_t rxDataCnt = 0;
uint8_t txDataCnt = 0;
bool lastDataTx = false;
bool lastDataRx = false;
uint8_t expectedEventsInQueue = 0;


void I2C_ISR(void) interrupt 6
{
	switch (I2STAT)
	{
		case 0x00:		//	bus error
			STO = 1;			
			break;

		case 0x60:		//	slave receive address ACK
			AA = 1;
			break;
				
		case 0x68:		//	slave receive arbitration lost
			AA = 0;
			STA = 1;
			break;

		case 0x80:		//	slave receive data ACK
			dataReceived = I2DAT;
						
			if(dataReceived == STATUS_UPDATE){

				statusReceived = dataReceived;
				lastDataRx = true;
			}
			else if(dataReceived == EVENT_HANDLER){
								
				eventReceived = dataReceived;
				lastDataRx = false;
			}
			else if(dataReceived == ID_VERIFICATION){
							
				idVerificationStatus = true;
				lastDataRx = true;
			}
			else if(dataReceived == RESET_COMMAND){
						
				TA = 0x0AA;
				TA = 0x55;
				CHPCON |= 0x80;
							
				lastDataRx = true;
			}
						
			if (eventReceived == EVENT_HANDLER){
								
				lastDataRx = true;
				expectedEventsInQueue = dataReceived;
			}					
								
			if(lastDataRx){
				AA = 0;
				lastDataRx = false;
			}
			else{
				AA = 1;
			}

			break;

			case 0x88:		//	slave receive data NACK
				dummyDataReceived = I2DAT;
				AA = 1;
				break;

			case 0xA0:		//	slave transmit repeat start or stop
				AA = 1;
				break;

			case 0xA8:		//	slave transmit address ACK
				I2DAT = SLAVE_ADDR;
				AA = 1;
				break;
        
			case 0xB8:		//	slave transmit data ACK
					
				if(statusReceived == STATUS_UPDATE){
							
					sendNodeNum();
					
					lastDataTx = true;
					statusReceived = 0;
				}
				else if(eventReceived == EVENT_HANDLER){
								
					if(txDataCnt >= expectedEventsInQueue*2){
						
						lastDataTx = true;
						txDataCnt = 0;
						eventReceived = 0;
					}
					else{
						
						lastDataTx = false;
								
						if(++txDataCnt % 2 == 1){

							sendDevID();
						}
						else{ 
							sendDevValue();
							
							freeElement();
						}
					}
				}
				else if(idVerificationStatus){
						
					I2DAT = SLAVE_ADDR;
					idVerificationStatus = false;
					lastDataTx = true;
				}
						
				if(lastDataTx){
					AA = 0;
					lastDataTx = false;
				}
				else{
					AA = 1;
				}
				
				break;

			case 0xC0:		//	slave transmit data NACK
				AA = 1;
				break; 

			case 0xC8:		//	slave transmit last data ACK
				AA = 1;
				break;        
	}

	SI = 0;
	while(STO);
}


void main(void){
	
	uint32_t i = 0;
	
	Set_All_GPIO_Quasi_Mode;
	
	P2M1&=~SET_BIT5;P2M2|=SET_BIT5;
	
	P25 = 1;
	for(i = 0; i<100000; i++){}
	P25 = 0;
	for(i = 0; i<100000; i++){}
	P25 = 1;
	
	initI2C();		// Initialize i2c communication

	buttonsInit();
	slidersInit();
	encodersInit();

	while(1){
		
		buttonsScan();
		encodersScan();
		slidersScan();
	}
}


void initI2C(){

	P23_OpenDrain_Mode;
	P24_OpenDrain_Mode;
	
	P23 = 1;
	P24 = 1;
 
	set_EI2C;
	set_EA;

	I2ADDR = SLAVE_ADDR<<1;
    
	set_I2CEN;
	set_AA;
}
