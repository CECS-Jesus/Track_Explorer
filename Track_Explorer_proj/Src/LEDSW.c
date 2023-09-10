// LEDSW.c
// Runs on TM4C123 for CECS347 Project 2
#include "tm4c123gh6pm.h"
#include "LEDSW.h"
void LEDSW_Init(void){
		if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
	}
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; 	// unlock GPIO Port F
	//LEDS
  GPIO_PORTF_AMSEL_R &= ~0x0E;      // 3) disable analog function
  GPIO_PORTF_PCTL_R &= ~0x0000FFF0; // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R |= 0x0E;         // 6) PF1-PF3 output
  GPIO_PORTF_AFSEL_R &= ~0x0E;      // 7) no alternate function     
  GPIO_PORTF_DEN_R |= 0x0E;         // 8) enable digital pins PF3-PF1
  LED = Dark;                       // Turn off all LEDs.
	
	//SWITCHES
  GPIO_PORTF_CR_R |= 0x11;         		// allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~0x11;    			// (c) make PF4,0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x11;  			//     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x11;     			//     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000F000F; 	//  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x11;  			//     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= 0x11;     			//     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;     			// (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    			//     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;    			//     PF4,PF0 falling edge event
  GPIO_PORTF_ICR_R = 0x11;      			// (e) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;      			// (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00400000; // (g) bits:23-21 for PORTF, set priority to 2
  NVIC_EN0_R |= 0x40000000; // (h) enable interrupt 30 in NVIC
}

