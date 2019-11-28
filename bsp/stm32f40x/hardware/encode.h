#ifndef __ENCODE_H__
#define __ENCODE_H__
#include "stm32f4xx.h"

void encode_Init(void);
extern s32 GetEncoder1(void);
extern s32 GetEncoder2(void);
extern s32 GetEncoder3(void);
#endif
