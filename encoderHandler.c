#include "Typedef.h"
#include "N76E616.h"
#include "SFR_Macro.h"
#include "Define.h"
#include "encoders.h"
#include "list.h"

uint8_t previousEncoderState[7] = {0};
signed int encoderPos[7] = {0};
signed int encoderLastPos[7] = {0};

void encodersInit(){
	
	ENC_SCHMITT_TRIGGER_INPUT;

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
		
		if(ENCODER0_B != ENCODER0_A)
			encoderPos[0]++;
		else
			encoderPos[0]--;
		
		//searchList(0x10);
		addNewNode(0x10, (signed int)(encoderPos[0]-encoderLastPos[0]));
	
		encoderLastPos[0] = encoderPos[0];
	
		previousEncoderState[0] = ENCODER0_A;
	}
	
	
	if(previousEncoderState[1] != ENCODER1_A){
	
		if(ENCODER1_B != ENCODER1_A)
			encoderPos[1]++;
		else
			encoderPos[1]--;
		
		//searchList(0x11);
		addNewNode(0x11, (signed int)(encoderPos[1]-encoderLastPos[1]));
		
		encoderLastPos[1] = encoderPos[1];
	
		previousEncoderState[1] = ENCODER1_A;
	}
	
	
	if(previousEncoderState[2] != ENCODER2_A){
	
		if(ENCODER2_B != ENCODER2_A)
			encoderPos[2]++;
		else
			encoderPos[2]--;
		
		//searchList(0x12);
		addNewNode(0x12, (signed int)(encoderPos[2]-encoderLastPos[2]));
		
		encoderLastPos[2] = encoderPos[2];
	
		previousEncoderState[2] = ENCODER2_A;
	}
	
	
	if(previousEncoderState[3] != ENCODER3_A){
	
		if(ENCODER3_B != ENCODER3_A)
			encoderPos[3]++;
		else
			encoderPos[3]--;
		
		//searchList(0x13);
		addNewNode(0x13, (signed int)(encoderPos[3]-encoderLastPos[3]));
		
		encoderLastPos[3] = encoderPos[3];
	
		previousEncoderState[3] = ENCODER3_A;
	}
	/*
	
	if(previousEncoderState[4] != ENCODER4_A){
	
		if(ENCODER4_B != ENCODER4_A)
			encoderPos[4]++;
		else
			encoderPos[4]--;
		
		//searchList(0x14);
		addNewNode(0x14, (signed int)(encoderPos[4]-encoderLastPos[4]));
		
		encoderLastPos[4] = encoderPos[4];
	
		previousEncoderState[4] = ENCODER4_A;
	}
	
		
	if(previousEncoderState[5] != ((P4>>6 && 0x01))){
	
		if(ENCODER5_B != ((P4>>6 && 0x01)))
			encoderPos[5]++;
		else
			encoderPos[5]--;
		
		//searchList(0x15);
		addNewNode(0x15, (signed int)(encoderPos[5]-encoderLastPos[5]));
		
		encoderLastPos[5] = encoderPos[5];
	
		previousEncoderState[5] = (P4>>6 && 0x01);
		//previousEncoderState[5] = ENCODER5_A;
	}
	
	
	if(previousEncoderState[6] != ENCODER6_A){
	
		if(ENCODER6_B != ENCODER6_A)
			encoderPos[6]++;
		else
			encoderPos[6]--;
		
		//searchList(0x16);
		addNewNode(0x16, (signed int)(encoderPos[6]-encoderLastPos[6]));
		
		encoderLastPos[6] = encoderPos[6];
	
		previousEncoderState[6] = ENCODER6_A;
	}*/
}