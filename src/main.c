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

#define P25_PushPull_Mode P2M1&= ~SET_BIT5;P2M2|= SET_BIT5

enum dataReceived{
	statusUpdate,
	eventHandler,
	idVerification
}dataRec;


__near uint8_t rxDataCnt = 0;
__near uint8_t txDataCnt = 0;
__near bool lastDataTx = false;
__near bool lastDataRx = false;
__near uint8_t expectedEventsInQueue = 0;
__near uint8_t potID = 0;


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

			/*if(dataRec == statusUpdate){

				lastDataRx = true;
				potID = I2DAT;
			}*/

            /*if(dataRec == eventHandler){

                lastDataRx = true;
                expectedEventsInQueue = I2DAT;
            }*/

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
				else if(dataRec == statusUpdate){

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
					/*sendNodeNum();
					lastDataTx = true;*/
                	if(txDataCnt >= 2){

						lastDataTx = true;
						txDataCnt = 0;
						//dataRec = 0;
					}
                	else{
                		++txDataCnt;

                		if(txDataCnt == 1){
							sendNodeNum();
                		}
                		else if (txDataCnt == 2){
							sendPotValue(potID);
                		}

						lastDataTx = false;
                	}
                }
                else if(dataRec == eventHandler){

                    if(txDataCnt >= expectedEventsInQueue*2){

                        lastDataTx = true;
                        txDataCnt = 0;
                        //dataRec = 0;
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
                    //dataRec = 0;
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

    uint32_t i = 0;

    P25_PushPull_Mode;

    P25 = 1;
    for(i = 0;i<100000;i++){}
    P25 = 0;
    for(i = 0;i<100000;i++){}
    P25 = 1;

    buttonsInit();
    slidersInit();
    encodersInit();

    initI2C();// Initialize i2c communication

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
