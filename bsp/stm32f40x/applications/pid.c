#include "pid.h"
#include <math.h>
#include "stm32f4xx.h"

extern u16	Parameter;


float ResultValue;
float ResultValueBack;
float ResultValueBackOne;
float ResultValueBackTwo;
float GiveValueBackOne;
float GiveValueBackTwo;
float GiveValueBack;
float ResultValuess;
float PIDErrADD;
float ErrBack;
float SeqIntPIDErrADD;
float SeqIntErrBack;
float OverIntPIDErrADD;
float OverIntResultBack;
float OverIntErrBack;

float PID(float Kp, float Ki, float Kd, float GiveValue, float ActualValue)
{
float result;
float Err,KpWork, KiWork, KdWork;
Err = GiveValue - ActualValue;
if(fabs(Err) > 2*Parameter)
		Err=4*Parameter-fabs(Err);
KpWork = Kp*Err;
KiWork = Ki*PIDErrADD;
KdWork = Kd*(Err-ErrBack);
result = KpWork+KiWork+KdWork;
PIDErrADD = PIDErrADD + Err;
ErrBack = Err;
return result;
}


float SeqIntPID(float Kp, float Ki, float Kd,  u16 GiveValue, u16 ActualValue)
{
float result;
float KpWork, KiWork, KdWork;
s16 Err = (GiveValue - ActualValue);
KpWork = Kp*Err;
KiWork = Ki*SeqIntPIDErrADD;
KdWork = Kd*(Err-SeqIntErrBack);
if(fabs(Err) > 100)
{
result = KpWork+KdWork;
}else{
result = KpWork+KiWork+KdWork;
}
SeqIntPIDErrADD = SeqIntPIDErrADD + Err;
SeqIntErrBack = Err;
return result;
}


float OverIntPID(float Kp, float Ki, float Kd, float GiveValue, float ActualValue)
{
float result;
float Err,KpWork, KiWork, KdWork;
Err = GiveValue - ActualValue;
	if(fabs(Err) > 2*Parameter)
		Err=4*Parameter-fabs(Err);
if(OverIntResultBack > 120)
{
if(Err < 0)
{
OverIntPIDErrADD = OverIntPIDErrADD + Err;
}
}else if(OverIntResultBack < 120){if(Err > 0)
{
OverIntPIDErrADD = OverIntPIDErrADD + Err;
}
}else{
OverIntPIDErrADD = OverIntPIDErrADD + Err;
}
KpWork = Kp*Err;
KiWork = Ki*OverIntPIDErrADD;
KdWork = Kd*(Err-OverIntErrBack);
result = KpWork+KiWork+KdWork;
OverIntErrBack = Err;
OverIntResultBack = result;
return result;
}

float OscilElement(float GiveValue) 
{
float result;
//result = (GiveValue+(2*T*T+2*WP*T)*ResultValueBackOne-T*T*ResultValueBackTwo)/(T*T+2*T*WP+1); ºóÏò²î·Ö
result = 1.319*ResultValueBackOne - 0.3399*ResultValueBackTwo + 0.004608*(GiveValueBackTwo + GiveValue)+ 0.009216*GiveValueBackOne;
//result = 1.921*ResultValueBackOne - 0.9236*ResultValueBackTwo + 0.006663*(GiveValueBackTwo + GiveValue)+ 0.01333*GiveValueBackOne;
GiveValueBackTwo = GiveValueBackOne;
GiveValueBackOne = GiveValue;
ResultValueBackTwo = ResultValueBackOne;
ResultValueBackOne = result;
return result;
}
