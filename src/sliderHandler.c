#include <stdio.h>
#include <stdlib.h>
#include "Typedef.h"
#include "N76E616.h"
#include "SFR_Macro.h"
#include "Define.h"
#include "sliders.h"
#include "list.h"

__near uint8_t sliderValueEMA[3] = {0};
__near uint8_t sliderLastValue[3] = {0};

void slidersInit(){

    ADC_INIT();
    
    INIT_ADC_SLIDERS();
    
    INIT_ADC_SLIDER0();
    sliderLastValue[0] = sliderValueEMA[0] = sliderRead();
    INIT_ADC_SLIDER1();
    sliderLastValue[1] = sliderValueEMA[1] = sliderRead();
    INIT_ADC_SLIDER2();
    sliderLastValue[2] = sliderValueEMA[2] = sliderRead();
}

unsigned int sliderRead(){

    uint16_t sliderValue = 0x0000;
    
    clr_ADCF;
    set_ADCS;
    
    while(!ADCF);
    
    sliderValue = ADCRH;
    sliderValue <<= 2;
    sliderValue |= (ADCRL & 0x03);
    
    return (unsigned int)sliderValue/4;
}

void slidersScan(){
    
    uint8_t sliderValue,i;
    
    for(i = 0;i < 3;i++){
        
        if(i == 0){
            INIT_ADC_SLIDER0();
        }
        else if(i == 1){
            INIT_ADC_SLIDER1();
        }
        else if(i == 2){
            INIT_ADC_SLIDER2();
        }
        

        //set_IDLE;
        sliderValue = sliderRead();
        
        sliderValueEMA[i] = (EMA_ALPHA*sliderValue) + ((1-EMA_ALPHA)*sliderValueEMA[i]);
    
        if(sliderValueEMA[i] != sliderLastValue[i]){
    
            addNewNode((0x20 | i), sliderValueEMA[i]);
        
            sliderLastValue[i] = sliderValueEMA[i];
        }
        
        ADCCON1&= CLR_BIT0;
    }

}
 
