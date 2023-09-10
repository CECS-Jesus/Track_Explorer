// Motors.c
// Runs on TM4C123 for CECS347 Project 2
#include "tm4c123gh6pm.h"
#include "Motors.h"
#include "PLL.h"
// The #define statement SYSDIV2 in PLL.h
// configure the system to get its clock from the PLL 

void Motors_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;	// Activate A clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)==0){};
	}	
	GPIO_PORTA_AFSEL_R |= 0xC0;	// enable alt funct: PA7 and PA6 for PWM
  GPIO_PORTA_PCTL_R &= ~0xFF000000; // PWM to be used
  GPIO_PORTA_PCTL_R |= 0x55000000; // PWM to be used
	GPIO_PORTA_DR8R_R |=0xC0;
  GPIO_PORTA_DEN_R |= 0xC0;	// enable digital I/O 
	//GPIO_PORTA_DR8R_R |= 0xC0;    // enable 8 mA drive on PA7, 6
	
	// Initializes PWM settings
	SYSCTL_RCGCPWM_R |= PWM_ENABLE_PWM1EN;	// activate PWM1
	SYSCTL_RCC_R &= ~0x001E0000; // Clear any previous PWM divider values
	// PWM1_1 output A&B Initialization for PA76
	PWM1_1_CTL_R = 0;	// re-loading down-counting mode
	PWM1_1_GENA_R |= 0xC8;	// low on LOAD, high on CMPA down
	PWM1_1_GENB_R |= 0xC08;// low on LOAD, high on CMPB down
	PWM1_1_LOAD_R = Total_Period - 1;	// cycles needed to count down to 0
  PWM1_1_CMPA_R = 0;	// count value when output rises
	PWM1_1_CMPB_R = 0;	// count value when output rises
	
	PWM1_1_CTL_R |= 0x00000001;	// Enable PWM0 Generator 0 in Countdown mode
	PWM1_ENABLE_R &= ~0x0000000C;	// Disable PA76:PWM0 output 0&1 on initialization //pwm 3
}

//Initializes PB5432 for use with L298N motor driver direction
void Car_Dir_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}		
  GPIO_PORTB_AMSEL_R &= ~0x3C;	// disable analog function
	GPIO_PORTB_AFSEL_R &= ~0x3C;	// no alternate function
  GPIO_PORTB_PCTL_R &= ~0x00FFFF00;	// GPIO clear bit PCTL 
	GPIO_PORTB_DIR_R |= 0x3C; // output on pin(s)
  GPIO_PORTB_DEN_R |= 0x3C;	// enable digital I/O on pin(s)
}

void PWMA_Duty(unsigned long duty_L, unsigned long duty_R){
  PWM1_1_CMPA_R = duty_L - 1;             // 6) count value when output rises
	PWM1_1_CMPB_R = duty_R - 1; 
}
