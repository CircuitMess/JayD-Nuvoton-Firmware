#include <stdio.h>
#include <stdlib.h>
#include "Typedef.h"
#include "N76E616.h"
#include "SFR_Macro.h"
#include "Define.h"
#include "sliders.h"
#include "list.h"

uint8_t sliderValue[3] = {0};
uint8_t sliderLastValue[3] = {0};

void slidersInit(){

	INIT_ADC_SLIDER0();
	sliderLastValue[0] = sliderValue[0] = sliderRead();
	INIT_ADC_SLIDER1();
	sliderLastValue[1] = sliderValue[1] = sliderRead();
	INIT_ADC_SLIDER2();
	sliderLastValue[2] = sliderValue[2] = sliderRead();
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

void slidersScan(){
	
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
/*	Old Slider Function
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