#ifndef _CONTROL_H
#define _CONTROL_H

#include "main.h"

int ControlAgreement(char *pGetNum, char *pCommand);    
uint8_t SectionFun(int Num);
void DoFun(int Num);
void ControlPwm1(uint16_t value);
void ControlPwm2(uint16_t value);
int getGpioFun(void);


#endif
