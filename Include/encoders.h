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


void encodersInit();
void encodersScan(void);
