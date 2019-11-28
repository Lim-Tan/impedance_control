
#ifndef __MODBUS_H
#define __MODBUS_H
//#include "sys.h"
#include "stm32f4xx.h"

extern u8 receCount;
extern u8 receBuf[32];
extern u8 sendCount;	 
extern u8 sendBuf[120];
extern u8 sendPosi;
extern u8 checkoutError;

void checkComm0Modbus(void);

#endif
