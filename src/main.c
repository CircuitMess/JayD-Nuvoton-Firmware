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

//#define P25_PushPull_Mode P2M1&= ~SET_BIT5;P2M2|= SET_BIT5

// I2C Event Type
enum dataReceived{
	statusUpdate,
	eventHandler,
	idVerification,
	initPotValue,
	null
}dataRec = null;

// __near -> ROM/EPROM/FLASH (18432B)
// __far  -> EXTERNAL RAM (256B)
__near uint8_t rxDataCnt = 0;
__near uint8_t txDataCnt = 0;
__near bool lastDataTx = false;
__near bool lastDataRx = false;
__near uint8_t expectedEventsInQueue = 0;
__near uint8_t potID = 0;

// I2C Interrupt Routine
void I2C_ISR(void) __interrupt(6)
{
    switch (I2STAT)
    {
        case 0x00://  bus error
            STO = 1;
            break;

        case 0x60://  slave receive address ACK
            AA = 1;
            break;

        case 0x68://  slave receive arbitration lost
            AA = 0;
            STA = 1;
            break;

        case 0x80://  slave receive data ACK

            if(I2DAT == STATUS_UPDATE){

                dataRec = statusUpdate;
                lastDataRx = true;
            }
            else if(I2DAT == EVENT_HANDLER){

                dataRec = eventHandler;
                lastDataRx = true;
            }
            else if(I2DAT == ID_VERIFICATION){

                dataRec = idVerification;
                lastDataRx = true;
            }
            else if(I2DAT == RESET_COMMAND){

                TA = 0x0AA;
                TA = 0x55;
                CHPCON |= 0x80;

                lastDataRx = true;
            }
			else if(I2DAT == INITIAL_POT_VALUE){

				dataRec = initPotValue;
				lastDataRx = true;
			}

            if(lastDataRx){
                AA = 0;
                lastDataRx = false;
            }
            else{
                AA = 1;
            }

            break;

            case 0x88://  slave receive data NACK

				if(dataRec == eventHandler){

					expectedEventsInQueue = I2DAT;
				}
				else if(dataRec == initPotValue){

					potID = I2DAT;
				}

            	AA = 1;
                break;

            case 0xA0://  slave transmit repeat start or stop
                AA = 1;
                break;

            case 0xA8://  slave transmit address ACK
                I2DAT = SLAVE_ADDR;
                AA = 1;
                break;

            case 0xB8://  slave transmit data ACK

                if(dataRec == statusUpdate){
					sendNodeNum();
					lastDataTx = true;
					dataRec = null;

                }
                else if(dataRec == eventHandler){

                    if(txDataCnt >= expectedEventsInQueue*2){

                        lastDataTx = true;
                        txDataCnt = 0;
                        dataRec = null;
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
                else if(dataRec == idVerification){

                    I2DAT = SLAVE_ADDR;
                    dataRec = null;
                    lastDataTx = true;
                }
				else if(dataRec == initPotValue){

					sendPotValue(potID);
					dataRec = null;
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

            case 0xC0://  slave transmit data NACK
                AA = 1;
                break;

            case 0xC8://  slave transmit last data ACK
                AA = 1;
                break;
    }

    SI = 0;
    while(STO);
}


void main(void){
/*
    uint32_t i = 0;

    P25_PushPull_Mode;

    // test LED for RST
    P25 = 1;
    for(i = 0;i<100000;i++){}
    P25 = 0;
    for(i = 0;i<100000;i++){}
    P25 = 1;
*/

    // Input initialization
    buttonsInit();
    slidersInit();
    encodersInit();

	// Initialize i2c communication
    initI2C();

    // Loop
    while(1){

    	// bus error state handle
    	while(SI != 0){
    		if(I2STAT == 0x00){
    			STO = 1;
    		}
			SI = 0;
			if(SI!=0){
				I2CEN = 0;
				I2CEN = 1;
				SI = 0;
				I2CEN = 0;
			}
			I2CEN = 1;
    	}

    	// Input scan
        buttonsScan();
        encodersScan();
        slidersScan();
    }
}


void initI2C(){

	// Open drain mode for i2c
    P23_OpenDrain_Mode;
    P24_OpenDrain_Mode;

    // Initial value is HIGH
    P23 = 1;
    P24 = 1;

    // Enable interrupts
    set_EI2C;
    set_EA;

    // Slave address
    I2ADDR = SLAVE_ADDR<<1;

    // Enable i2c
    set_I2CEN;
    // set Acknowledge
    set_AA;
}
