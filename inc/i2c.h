/*  I2C GPIO MODE   */
#define P23_OpenDrain_Mode P2M1|= SET_BIT3;P2M2|= SET_BIT3
#define P24_OpenDrain_Mode P2M1|= SET_BIT4;P2M2|= SET_BIT4

#define SLAVE_ADDR 0x43			//  slave address (ESP->master, N76->slave)

#define STATUS_UPDATE 0x12		//  update number of events
#define EVENT_HANDLER 0x13		//  handle list events
#define INITIAL_POT_VALUE 0x14	//	initialize pot values
#define ID_VERIFICATION 0		//  return slave address 0x43
#define RESET_COMMAND 1			//  reset uC

void initI2C();
