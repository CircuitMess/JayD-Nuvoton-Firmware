#include "bool.h"

/*	BASIC SWITCHES GPIO MODE	*/
#define		P16_Input_Mode				P1M1|=SET_BIT6;P1M2&=~SET_BIT6
#define		P17_Input_Mode				P1M1|=SET_BIT7;P1M2&=~SET_BIT7
/*	ENCODER SWITCHES GPIO MODE	*/
#define		P32_Input_Mode				P3M1|=SET_BIT2;P3M2&=~SET_BIT2
#define		P57_Input_Mode				P5M1|=SET_BIT7;P5M2&=~SET_BIT7
#define		P54_Input_Mode				P5M1|=SET_BIT4;P5M2&=~SET_BIT4
#define		P51_Input_Mode				P5M1|=SET_BIT1;P5M2&=~SET_BIT1
#define		P26_Input_Mode				P2M1|=SET_BIT6;P2M2&=~SET_BIT6
//#define		P45_Input_Mode				P4M1|=SET_BIT5;P4M2&=~SET_BIT5
#define		P20_Input_Mode				P2M1|=SET_BIT0;P2M2&=~SET_BIT0

/*	SCHMITT TRIGGER INPUT FOR DEBOUNCING*/
#define	SW_SCHMITT_TRIGGER_INPUT		P1S&=0xFF; P1S|=0xC0;
#define	ENC_SW_SCHMITT_TRIGGER_INPUT		P2S&=0xFF;P2S|=0x41; P3S&=0xFF;P3S|=0x04; P4S&=0xFF;P4S|=0x20; P5S&=0xFF;P5S|=0x92;

/*	BASIC SWITCHES PIN NUMBER	*/
#define		SW0						P17				//	left
#define		SW1						P16				//	right
#define		SW_INPUT_MODE_INIT()	do{P16_Input_Mode; P17_Input_Mode; SW_SCHMITT_TRIGGER_INPUT;}while(0)

/*	ENCODER SWITCHES PIN NUMBER	*/
#define		ENCODER_SW_0	P32				//	center
#define		ENCODER_SW_1	P57				//	left-up
#define		ENCODER_SW_2	P54				//	right-down
#define		ENCODER_SW_3	P51				//	right-middle
#define		ENCODER_SW_4	P26				//	right-up
#define		ENCODER_SW_5	P4^5			//	left-down
#define		ENCODER_SW_6	P20				//	left-middle
#define		ENCODER_SW_INPUT_MODE_INIT()	do{P32_Input_Mode; P57_Input_Mode; P54_Input_Mode; P51_Input_Mode; P26_Input_Mode; /*P45_Input_Mode;*/ P20_Input_Mode; ENC_SW_SCHMITT_TRIGGER_INPUT;}while(0)

void buttonsScan(void);
