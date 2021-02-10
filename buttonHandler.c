#include "Typedef.h"
#include "N76E616.h"
#include "SFR_Macro.h"
#include "Define.h"
#include "buttons.h"
#include "list.h"

bool switchEvent[9] = {false};
uint16_t btnCount[9] = {0};

void btnDebounce(uint8_t num, bool val){

	if(val){
		btnCount[num]++;
		
		if(btnCount[num] >= DEBOUNCE_COUNTER){
			
			switchEvent[num] = true;		
			addNewNode(num, val);

		}
	}
	else{
		btnCount[num]--;
		
		if(btnCount[num] <= 0){
		
			switchEvent[num] = false;
			addNewNode(num, val);
			
		}
	}
}


void buttonsScan(){

	if(!SW0 && !switchEvent[0]){
		
		btnDebounce(0, 1);
	}
	else if(SW0 && switchEvent[0]){
		
		btnDebounce(0, 0);
	}
	
	if(!SW1 && !switchEvent[1]){
		
		btnDebounce(1, 1);
		
	}
	else{
		if(SW1 && switchEvent[1]){
			
			btnDebounce(1, 0);
		}	
	}
						
	if(!ENCODER_SW_0 && !switchEvent[2]){
		
		btnDebounce(2, 1);
	}
	else{
		if(ENCODER_SW_0 && switchEvent[2]){
			
			btnDebounce(2, 0);
		}
	}
	
	if(!ENCODER_SW_1 && !switchEvent[3]){
		
		btnDebounce(3, 1);
	}
	else{
		if(ENCODER_SW_1 && switchEvent[3]){
			
			btnDebounce(3, 0);			
		}
	}
	
	if(!ENCODER_SW_2 && !switchEvent[4]){
		
		btnDebounce(4, 1);
	}
	else{
		if(ENCODER_SW_2 && switchEvent[4]){
			
			btnDebounce(4, 0);
		}
	}
	
	if(!ENCODER_SW_3 && !switchEvent[5]){
		
		btnDebounce(5, 1);
	}
	else{
		if(ENCODER_SW_3 && switchEvent[5]){
			
			btnDebounce(5, 0);
		}
	}
	
	if(!ENCODER_SW_4 && !switchEvent[6]){
		
		btnDebounce(6, 1);
	}
	else{
		if(ENCODER_SW_4 && switchEvent[6]){
			
			btnDebounce(6, 0);
		}
	}
		
	if(((P4>>5 & 0x01) != 0x01)  && !switchEvent[7]){							//	register P4 only byte addressable
		
		btnDebounce(7, 1);
	}
	else{
		if(((P4>>5 & 0x01) == 0x01) && switchEvent[7]){
			
			btnDebounce(7, 0);
		}
	}
	
	if(!ENCODER_SW_6 && !switchEvent[8]){
		
		btnDebounce(8, 1);
	}
	else{
		if(ENCODER_SW_6 && switchEvent[8]){
			
			btnDebounce(8, 0);
		}
	}
}