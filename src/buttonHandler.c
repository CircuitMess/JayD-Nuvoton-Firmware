#include "Typedef.h"
#include "N76E616.h"
#include "SFR_Macro.h"
#include "Define.h"
#include "buttons.h"
#include "list.h"

__near uint16_t switchEvent = 0x0000;

void buttonsInit(){

    SW_INPUT_MODE_INIT();
    ENCODER_SW_INPUT_MODE_INIT();
}

void buttonsScan(){

    if(!SW0 && !(switchEvent & 0x0001)){
        
        switchEvent |= 0x0001;
        addNewNode(0, 1);
    }
    else if(SW0 && (switchEvent & 0x0001)){
        
        switchEvent &= 0x0FFE;
        addNewNode(0, 0);
    }
    
    if(!SW1 && !(switchEvent & 0x0002)){
        
        switchEvent |= 0x0002;
        addNewNode(1, 1);
        
    }
    else if(SW1 && (switchEvent & 0x0002)){
            
        switchEvent &= 0x0FFD;
        addNewNode(1, 0);
    }
    
    if(!ENCODER_SW_0 && !(switchEvent & 0x0004)){
        
        switchEvent |= 0x0004;
        addNewNode(2, 1);
    }
    else if(ENCODER_SW_0 && (switchEvent & 0x0004)){
            
        switchEvent &= 0x0FFB;
        addNewNode(2, 0);
    }
    
    
    if(!ENCODER_SW_1 && !(switchEvent & 0x0008)){
        
        switchEvent |= 0x0008;
        addNewNode(3, 1);
    }
    else if(ENCODER_SW_1 && (switchEvent & 0x0008)){
            
        switchEvent &= 0x0FF7;
        addNewNode(3, 0);
    }
    
    if(!ENCODER_SW_2 && !(switchEvent & 0x0010)){
        
        switchEvent |= 0x0010;
        addNewNode(4, 1);
    }
    else if(ENCODER_SW_2 && (switchEvent & 0x0010)){
            
        switchEvent &= 0x0FEF;
        addNewNode(4, 0);
    }
    
    if(!ENCODER_SW_3 && !(switchEvent & 0x0020)){
        
        switchEvent |= 0x0020;
        addNewNode(5, 1);
    }
    else if(ENCODER_SW_3 && (switchEvent & 0x0020)){
            
        switchEvent &= 0x0FDF;
        addNewNode(5, 0);
    }
    
    if(!ENCODER_SW_4 && !(switchEvent & 0x0040)){
        
        switchEvent |= 0x0040;
        addNewNode(6, 1);
    }
    else if(ENCODER_SW_4 && (switchEvent & 0x0040)){
            
        switchEvent &= 0x0FBF;
        addNewNode(6, 0);
    }
        
    if(((P4>>5 & 0x01) != 0x01) && !(switchEvent & 0x0080)){//  register P4 only byte addressable
        
        switchEvent |= 0x0080;
        addNewNode(7, 1);
    }
    else if(((P4>>5 & 0x01) == 0x01) && (switchEvent & 0x0080)){
            
        switchEvent &= 0x0F7F;
        addNewNode(7, 0);
    }
    
    if(!ENCODER_SW_6 && !(switchEvent & 0x0100)){
        
        switchEvent |= 0x0100;
        addNewNode(8, 1);
    }
    else if(ENCODER_SW_6 && (switchEvent & 0x0100)){
            
        switchEvent &= 0x0EFF;
        addNewNode(8, 0);
    }
}
