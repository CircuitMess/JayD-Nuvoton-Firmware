#include "Typedef.h"
#include "N76E616.h"
#include "SFR_Macro.h"
#include "Define.h"
#include "buttons.h"
#include "list.h"

bool switchEvent[9] = {false};

void buttonsInit(){

	SW_INPUT_MODE_INIT();
	ENCODER_SW_INPUT_MODE_INIT();
}

void buttonsScan(){
/*
	if(!SW0 && !switchEvent[0]){
		
		switchEvent[0] = true;		
		addNewNode(0, 1);
	}
	else if(SW0 && switchEvent[0]){
		
		switchEvent[0] = false;		
		addNewNode(0, 0);
	}
	
	if(!SW1 && !switchEvent[1]){
		
		switchEvent[1] = true;		
		addNewNode(1, 1);
		
	}
	else if(SW1 && switchEvent[1]){
			
		switchEvent[1] = false;		
		addNewNode(1, 0);
	}		
	*/
	if(!ENCODER_SW_0 && !switchEvent[2]){
		
		switchEvent[2] = true;		
		addNewNode(2, 1);
	}
	else if(ENCODER_SW_0 && switchEvent[2]){
			
		switchEvent[2] = false;		
		addNewNode(2, 0);
	}
	
	
	if(!ENCODER_SW_1 && !switchEvent[3]){
		
		switchEvent[3] = true;		
		addNewNode(3, 1);
	}
	else if(ENCODER_SW_1 && switchEvent[3]){
			
		switchEvent[3] = false;		
		addNewNode(3, 0);		
	}
	/*
	if(!ENCODER_SW_2 && !switchEvent[4]){
		
		switchEvent[4] = true;		
		addNewNode(4, 1);
	}
	else if(ENCODER_SW_2 && switchEvent[4]){
			
		switchEvent[4] = false;		
		addNewNode(4, 0);		
	}
	
	if(!ENCODER_SW_3 && !switchEvent[5]){
		
		switchEvent[5] = true;		
		addNewNode(5, 1);
	}
	else if(ENCODER_SW_3 && switchEvent[5]){
			
		switchEvent[5] = false;		
		addNewNode(5, 0);		
	}
	
	if(!ENCODER_SW_4 && !switchEvent[6]){
		
		switchEvent[6] = true;		
		addNewNode(6, 1);
	}
	else if(ENCODER_SW_4 && switchEvent[6]){
			
		switchEvent[6] = false;		
		addNewNode(6, 0);	
	}
		
	if(((P4>>5 & 0x01) != 0x01)  && !switchEvent[7]){							//	register P4 only byte addressable
		
		switchEvent[7] = true;		
		addNewNode(7, 1);
	}
	else if(((P4>>5 & 0x01) == 0x01) && switchEvent[7]){
			
		switchEvent[7] = false;		
		addNewNode(7, 0);	
	}
	
	if(!ENCODER_SW_6 && !switchEvent[8]){
		
		switchEvent[8] = true;		
		addNewNode(8, 1);
	}
	else if(ENCODER_SW_6 && switchEvent[8]){
			
		switchEvent[8] = false;		
		addNewNode(8, 0);	
	}*/
}