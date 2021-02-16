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

#define		ADC_CLK_DIV		ADCCON1&=0x0F; ADCCON1|=0x20
#define		ADC_ACQ				ADCAQT&=0xFF
#define		ADC_ENABLE		ADCCON1&=~SET_BIT0; ADCCON1|=SET_BIT0
#define		ADC_DISABLE		ADCCON1&=~SET_BIT0

#define		ADC_INIT()		do{ADC_DISABLE; ADC_CLK_DIV; ADC_ACQ;}while(0)

/*	SLIDER INIT	*/
#define		INIT_ADC_SLIDERS()		do{ADC_DISABLE; P06_Input_Mode; P05_Input_Mode; P04_Input_Mode; P0DIDS&=0x00; P0DIDS|=0x70; }while(0)

#define		INIT_ADC_SLIDER0()		do{	ADC_DISABLE; SLIDER0_CH; ADC_ENABLE;}while(0)
#define		INIT_ADC_SLIDER1()		do{	ADC_DISABLE; SLIDER1_CH; ADC_ENABLE;}while(0)
#define		INIT_ADC_SLIDER2()		do{	ADC_DISABLE; SLIDER2_CH; ADC_ENABLE;}while(0)

#define	EMA_ALPHA	0.05        

/*Alpha -> coefficient in the range <0,1> that decides how many samples the EMA algorithm should take into account. 
	A low a will be very slow to rapid input changes and take many samples into account. A high a will be fast, but 
	average over fewer samples. You can look at a as kind of a cutoff frequency in a low-pass filter.*/

void slidersInit();
unsigned int sliderRead(void);
void slidersScan();

