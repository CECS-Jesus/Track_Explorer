// LEDSW.h
// Runs on TM4C123 for CECS347 Project 2
#define LED (*((volatile unsigned long *)0x40025038))  // use onboard three LEDs: PF321

	
void LEDSW_Init(void);

#define Dark    	0x00
#define Red     	0x02
#define Blue    	0x04
#define Green   	0x08
#define Yellow  	0x0A
#define Cran      0x0C
#define White   	0x0E
#define Purple  	0x06

#define SW1 0x10 //left switch
#define SW2 0x01 //right switch
