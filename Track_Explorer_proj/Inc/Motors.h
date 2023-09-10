// Motors.h
// Runs on TM4C123 for CECS347 Project 2
//#define TOTAL_PERIOD 80000000 //  80MHz/1000=80000
//#define START_SPEED 80000000*0.5
// configure the system to get its clock from the PLL
#include "PLL.h"
void Motors_Init(void);
void Car_Dir_Init(void);
void PWMA_Duty(unsigned long duty_L, unsigned long duty_R);

#define WHEEL_DIR (*((volatile unsigned long *)0x400050F0)) // PB5432 are the four direction pins for L298
#define Total_Period (400/SYSDIV2+1)*1000
// Constant definitions based on the following hardware interface:
// PB5432 are used for direction control on L298.
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.
#define FORWARD 0x28
#define BACKWARD 0x14
#define LEFTPIVOT 0x18
#define RIGHTPIVOT 0x24
