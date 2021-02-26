/*  I2C GPIO MODE   */
#define P23_OpenDrain_Mode P2M1|= SET_BIT3;P2M2|= SET_BIT3
#define P24_OpenDrain_Mode P2M1|= SET_BIT4;P2M2|= SET_BIT4

#define SLAVE_ADDR 0x43//  67_(10)

#define STATUS_UPDATE 0x12//  18_(10)
#define EVENT_HANDLER 0x13//  19_(10)
#define ID_VERIFICATION 0//  returns slave address 0x43
#define RESET_COMMAND 1//  resets uC

void initI2C();
