/*	SLIDER GPIO MODE	*/
#define		P06_Input_Mode				P0M1|=SET_BIT6;P0M2&=~SET_BIT6
#define		P05_Input_Mode				P0M1|=SET_BIT5;P0M2&=~SET_BIT5
#define		P04_Input_Mode				P0M1|=SET_BIT4;P0M2&=~SET_BIT4

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

void slidersInit();
unsigned int sliderRead(void);
void slidersScan();

