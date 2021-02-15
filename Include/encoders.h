/*	ENCODERS GPIO MODE	*/
#define		P15_Quasi_Mode				P1M1&=~SET_BIT5;P1M2&=~SET_BIT5
#define		P33_Quasi_Mode				P3M1&=~SET_BIT3;P3M2&=~SET_BIT3
#define		P31_Quasi_Mode				P3M1&=~SET_BIT1;P3M2&=~SET_BIT1
#define		P30_Quasi_Mode				P3M1&=~SET_BIT0;P3M2&=~SET_BIT0
//#define		P31_Input_Mode				P3M1|=SET_BIT1;P3M2&=~SET_BIT1
//#define		P30_Input_Mode				P3M1|=SET_BIT0;P3M2&=~SET_BIT0
#define		P56_Quasi_Mode				P5M1&=~SET_BIT6;P5M2&=~SET_BIT6
#define		P55_Quasi_Mode				P5M1&=~SET_BIT5;P5M2&=~SET_BIT5
#define		P53_Quasi_Mode				P5M1&=~SET_BIT3;P5M2&=~SET_BIT3
#define		P52_Quasi_Mode				P5M1&=~SET_BIT2;P5M2&=~SET_BIT2
#define		P50_Quasi_Mode				P5M1&=~SET_BIT0;P5M2&=~SET_BIT0
#define		P27_Quasi_Mode				P2M1&=~SET_BIT7;P2M2&=~SET_BIT7
#define		P46_Quasi_Mode				P4M1&=~SET_BIT6;P4M2&=~SET_BIT6
#define		P14_Quasi_Mode				P1M1&=~SET_BIT4;P1M2&=~SET_BIT4
#define		P22_Quasi_Mode				P2M1&=~SET_BIT2;P2M2&=~SET_BIT2
#define		P21_Quasi_Mode				P2M1&=~SET_BIT1;P2M2&=~SET_BIT1


/*	ENCODER A & B PINS	*/

#define		ENCODER0_A		P15				//	center
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

#define	ENC_SCHMITT_TRIGGER_INPUT		P1S&=0xFF;P1S|=0x30; P2S&=0xFF;P2S|=0x06; P3S&=0xFF;P3S|=0x0B; P4S&=0xFF;P4S|=0x40; P5S&=0xFF;P5S|=0x6D;

#define ENC_A_B_INIT()		do{P15_Quasi_Mode; P33_Quasi_Mode; P31_Quasi_Mode; P30_Quasi_Mode; P56_Quasi_Mode; P55_Quasi_Mode; P53_Quasi_Mode; P52_Quasi_Mode; P50_Quasi_Mode; P27_Quasi_Mode; P46_Quasi_Mode; P14_Quasi_Mode; P22_Quasi_Mode; P21_Quasi_Mode; ENC_SCHMITT_TRIGGER_INPUT;}while(0)


void encodersInit();
void encodersScan(void);
