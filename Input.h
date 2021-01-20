/*	I2C GPIO MODE	*/
#define		P23_OpenDrain_Mode		P2M1|=SET_BIT3;P2M2|=SET_BIT3
#define		P24_OpenDrain_Mode		P2M1|=SET_BIT4;P2M2|=SET_BIT4
/*	BASIC SWITCHES GPIO MODE	*/
#define		P16_Input_Mode				P1M1|=SET_BIT6;P1M2&=~SET_BIT6
#define		P17_Input_Mode				P1M1|=SET_BIT7;P1M2&=~SET_BIT7
/*	SLIDER GPIO MODE	*/
#define		P06_Input_Mode				P0M1|=SET_BIT6;P0M2&=~SET_BIT6
#define		P05_Input_Mode				P0M1|=SET_BIT5;P0M2&=~SET_BIT5
#define		P04_Input_Mode				P0M1|=SET_BIT4;P0M2&=~SET_BIT4
/*	ENCODER SWITCHES GPIO MODE	*/
#define		P32_Input_Mode				P3M1|=SET_BIT2;P3M2&=~SET_BIT2
#define		P57_Input_Mode				P5M1|=SET_BIT7;P5M2&=~SET_BIT7
#define		P54_Input_Mode				P5M1|=SET_BIT4;P5M2&=~SET_BIT4
#define		P51_Input_Mode				P5M1|=SET_BIT1;P5M2&=~SET_BIT1
#define		P26_Input_Mode				P2M1|=SET_BIT6;P2M2&=~SET_BIT6
//#define		P45_Input_Mode				P4M1|=SET_BIT5;P4M2&=~SET_BIT5
#define		P20_Input_Mode				P2M1|=SET_BIT0;P2M2&=~SET_BIT0
/*	ENCODERS GPIO MODE	*/
#define		P07_Quasi_Mode				P0M1&=~SET_BIT7;P0M2&=~SET_BIT7
#define		P33_Quasi_Mode				P3M1&=~SET_BIT3;P3M2&=~SET_BIT3
#define		P31_Quasi_Mode				P3M1&=~SET_BIT1;P3M2&=~SET_BIT1
#define		P30_Quasi_Mode				P3M1&=~SET_BIT0;P3M2&=~SET_BIT0
#define		P56_Quasi_Mode				P5M1&=~SET_BIT6;P5M2&=~SET_BIT6
#define		P55_Quasi_Mode				P5M1&=~SET_BIT5;P5M2&=~SET_BIT5
#define		P53_Quasi_Mode				P5M1&=~SET_BIT3;P5M2&=~SET_BIT3
#define		P52_Quasi_Mode				P5M1&=~SET_BIT2;P5M2&=~SET_BIT2
#define		P50_Quasi_Mode				P5M1&=~SET_BIT0;P5M2&=~SET_BIT0
#define		P27_Quasi_Mode				P2M1&=~SET_BIT7;P2M2&=~SET_BIT7
#define		P46_Quasi_Mode				P4M1&=~SET_BIT6;P4M2&=~SET_BIT6
#define		P14_Quasi_Mode				P1M1&=~SET_BIT4;P1M2&=~SET_BIT4
#define		p22_Quasi_Mode				P2M1&=~SET_BIT2;P2M2&=~SET_BIT2
#define		P21_Quasi_Mode				P2M1&=~SET_BIT1;P2M2&=~SET_BIT1


/*	ENCODER A & B PINS	*/
#define		ENCODER0_A		P07				//	center
#define		ENCODER0_B		P33
#define		ENCODER1_A		P31				//	left-up
#define		ENCODER1_B		P30
#define		ENCODER2_A		P56				//	right-down
#define		ENCODER2_B		P55
#define		ENCODER3_A		P53				//	right-middle
#define		ENCODER3_B		P52
#define		ENCODER4_A		P50				//	right-up
#define		ENCODER4_B		P27
#define		ENCODER5_A		P4^6			//	left-down
#define		ENCODER5_B		P14
#define		ENCODER6_A		P22				//	left-middle
#define		ENCODER6_B		P21
	

/*	BASIC SWITCHES PIN NUMBER	*/
#define		SW0						P17				//	left
#define		SW1						P16				//	right
#define		SW_INPUT_MODE_INIT()	do{P16_Input_Mode; P17_Input_Mode;}while(0)
/*	ENCODER SWITCHES PIN NUMBER	*/
#define		ENCODER_SW_0	P32				//	center
#define		ENCODER_SW_1	P57				//	left-up
#define		ENCODER_SW_2	P54				//	right-down
#define		ENCODER_SW_3	P51				//	right-middle
#define		ENCODER_SW_4	P26				//	right-up
#define		ENCODER_SW_5	P4^5			//	left-down
#define		ENCODER_SW_6	P20				//	left-middle
#define		ENCODER_SW_INPUT_MODE_INIT()	do{P32_Input_Mode; P57_Input_Mode; P54_Input_Mode; P51_Input_Mode; P26_Input_Mode; /*P45_Input_Mode;*/ P20_Input_Mode;}while(0)
/*	SLIDER PIN NUMBER	*/
#define		SLIDER0				P06
#define		SLIDER1				P05
#define		SLIDER2				P04
/*	CHANELS FOR SLIDERS	*/
#define		SLIDER0_CH		ADCCON0&=0xF0; ADCCON0|=0x06
#define		SLIDER1_CH		ADCCON0&=0xF0; ADCCON0|=0x05
#define		SLIDER2_CH		ADCCON0&=0xF0; ADCCON0|=0x04
/*	SLIDER INIT	*/
#define		INIT_ADC_SLIDER0()		do{	SLIDER0_CH;	P06_Input_Mode; P0DIDS&=0x00; P0DIDS|=SET_BIT6; ADCCON1|=SET_BIT0;}while(0)
#define		INIT_ADC_SLIDER1()		do{	SLIDER1_CH;	P05_Input_Mode; P0DIDS&=0x00; P0DIDS|=SET_BIT5; ADCCON1|=SET_BIT0;}while(0)
#define		INIT_ADC_SLIDER2()		do{	SLIDER2_CH;	P04_Input_Mode; P0DIDS&=0x00; P0DIDS|=SET_BIT4; ADCCON1|=SET_BIT0;}while(0)
