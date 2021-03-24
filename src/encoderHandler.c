#include "Typedef.h"
#include "N76E616.h"
#include "SFR_Macro.h"
#include "Define.h"
#include "encoders.h"
#include "list.h"

__near uint8_t previousEncoderState[7] = {0};

void encodersInit(){
    
    ENC_A_B_INIT();

    previousEncoderState[0] = ENCODER0_A;
    previousEncoderState[1] = ENCODER1_A;
    previousEncoderState[2] = ENCODER2_A;
    previousEncoderState[3] = ENCODER3_A;
    previousEncoderState[4] = ENCODER4_A;
    previousEncoderState[5] = ENCODER5_A;
    previousEncoderState[6] = ENCODER6_A;
    
}

void encodersScan(void){

    if(previousEncoderState[0] != ENCODER0_A){
        
        if(ENCODER0_B != ENCODER0_A){
        
            addNewNode(0x10, 1);
        }
        else{
            addNewNode(0x10, -1);
        }
        previousEncoderState[0] = ENCODER0_A;
    }
    
    
    if(previousEncoderState[1] != ENCODER1_A){
    
        if(ENCODER1_B != ENCODER1_A){
        
            addNewNode(0x11, 1);
        }
        else{
            addNewNode(0x11, -1);
        }
        previousEncoderState[1] = ENCODER1_A;
    }
    

    if(previousEncoderState[2] != ENCODER2_A){
    
        if(ENCODER2_B != ENCODER2_A){
        
            addNewNode(0x12, 1);
        }
        else{
            addNewNode(0x12, -1);
        }
        previousEncoderState[2] = ENCODER2_A;
    }
    
    
    if(previousEncoderState[3] != ENCODER3_A){
    
        if(ENCODER3_B != ENCODER3_A){
        
            addNewNode(0x13, 1);
        }
        else{
            addNewNode(0x13, -1);
        }
        previousEncoderState[3] = ENCODER3_A;
    }
    
    
    if(previousEncoderState[4] != ENCODER4_A){
    
        if(ENCODER4_B != ENCODER4_A){
        
            addNewNode(0x14, 1);
        }
        else{
            addNewNode(0x14, -1);
        }
        previousEncoderState[4] = ENCODER4_A;
    }
    
        
    if(previousEncoderState[5] != ((P4>>6 && 0x01))){
    
        if(ENCODER5_B != ((P4>>6 && 0x01))){
        
            addNewNode(0x15, 1);
        }
        else{
            addNewNode(0x15, -1);
        }
        previousEncoderState[5] = (P4>>6 && 0x01);
    }

    if(previousEncoderState[6] != ENCODER6_A){
    
        if(ENCODER6_B != ENCODER6_A){
        
            addNewNode(0x16, 1);
        }
        else{
            addNewNode(0x16, -1);
        }
        previousEncoderState[6] = ENCODER6_A;
    }

}
