// TrackExplorer.c
// Runs on TM4C123
// This project uses hardware PWM to control two DC Motors, 
// ADC to collect analog inputs from three Sharp IR sensors.
// The three Sharp analog IR distance sensors (GP2Y0A21YK0F) are used
// to allowthe robot to navigate through a track with two walls: 
// one mounted looking directly forward to avoid a head-on collision, 
// the other two looking forward to the left and to the right to detect  
// the distances between the car and the two walls. The goal is to 
// control power to each wheel so the left and right distances to the 
// walls are equal.
// If an object is detected too close to the robot, 
// the robot should be able to avoid it.
/*
    ------------------------------------------wall---------
                      /
                     /
                    / 
                   /
         -----------
         |         |
         | Robot   | ---> direction of motion and third sensor
         |         |
         -----------
                   \
                    \
                     \
                      \
    ------------------------------------------wall---------
*/
// The original project is designed by Dr. Daniel Valvano, Jonathan Valvano
// September 12, 2013
// Modifications are made by Dr. Min He.

// PE1 connected to forward facing IR distance sensor
// PE4 connected to right IR distance sensor
// PE5 connected to left IR distance sensor

#include "tm4c123gh6pm.h"
#include "Sensors.h"
#include "Motors.h"
#include "LEDSW.h"
#include "PLL.h"
#include "stdint.h"

// basic functions defined at end of startup.s
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode

// You use datasheet to calculate the following ADC values
// then test your sensors to adjust the values 
#define IR10CM						3448
#define CRASH             IR15CM  // 15 cm if there is less than this distance ahead of the robot, it will immediately stop
#define IR15CM            2233  // ADC output for 15cm:1.8v -> (1.8/3.3)*4095=2233 
#define IR20CM            1724  // ADC output for 20cm:1.39v -> (1.39/3.3)*4095=1724
#define IR30CM            1116  // ADC output for 30cm:0.9v -> (0.9/3.3)*4095=1116
#define IR40CM            918   // ADC output for 40cm:0.74v -> (0.74/3.3)*4095=918
#define IR80CM            496   // ADC output for 80cm:0.4v -> (0.4/3.3)*4095=496
                                // with equal power to both motors (LeftH == RightH), the robot still may not drive straight
                                // due to mechanical differences in the motors, so bias the left wheel faster or slower than
                                // the constant right wheel
#define LEFTPOWER        0.60*Total_Period    // duty cycle of left wheel 
#define RIGHTPOWER       0.60*Total_Period    // duty cycle of left wheel 
#define START_SPEED1	0.99*Total_Period
#define START_SPEED2	0.97*Total_Period

void System_Init(void);
void steering(uint16_t ahead_dist,uint16_t left_dist, uint16_t right_dist);
void Delay(void);
void GPIOPortF_Handler(void);

int main(void){
	uint16_t ahead, left, right;	
	DisableInterrupts();  // disable interrupts while initializing
	System_Init();
	EnableInterrupts();   // enable after all initialization are done
	//Calibrate the sensors: read at least 10 times from the sensor 
	// before the car starts to move: this will allow software to filter the sensor outputs.	
	for(int x = 0; x < 10; x++){
		ReadSensorsFIRFilter(&left, &ahead, &right);//sensor test
	}
	GPIOPortF_Handler();	//call sw1 or sw2
	WaitForInterrupt();	//Wait to be activated by switch on mcu
  	while(1){
		ReadSensorsFIRFilter(&left, &ahead, &right);
		steering(left,ahead,right); //set a breakpoint here once implemented sensor.c to check value if they are correct 	
	}
	LED = Blue;
	PWM1_ENABLE_R &= ~0x0000000C; //disable wheels
}

void System_Init(void) {
	PLL_Init();           // bus clock at 16 MHz
	Sensors_Init();        // initialize ADC to sample AIN2 (PE1), AIN9 (PE4), AIN8 (PE5)
	LEDSW_Init();         // configure onboard LEDs and push buttons
	Car_Dir_Init(); // control direction of L298N motor driver
	Motors_Init();         // Initialize signals for the two DC Motors
}

void GPIOPortF_Handler(void){ // 
	if(GPIO_PORTF_RIS_R&SW2){  // SW2 pressed
		GPIO_PORTF_ICR_R = SW2;  // acknowledge flag0
		LED = Yellow;
		PWM1_ENABLE_R &= ~0x0000000C; // stop both wheels
		Delay();
		WaitForInterrupt();
	}
	if(GPIO_PORTF_RIS_R&SW1){  // SW1 pressed
		GPIO_PORTF_ICR_R = SW1;  // acknowledge flag4
		PWMA_Duty(START_SPEED1, START_SPEED2);
		LED = Green;
		WHEEL_DIR = FORWARD;
		PWM1_ENABLE_R |= 0x0000000C; // enable both wheels
		Delay();
		
	}
}

void steering(uint16_t left_dist,uint16_t ahead_dist, uint16_t right_dist){
	// when track is complete stop and shine blue LED 	
	if(ahead_dist < IR80CM && right_dist < IR80CM  && left_dist < IR80CM ){
		LED = Blue;
		PWM1_ENABLE_R &= ~0x0000000C; //disable wheels
	}
	
	else if (ahead_dist > CRASH && right_dist < CRASH){ //left pivot
		PWMA_Duty(LEFTPOWER, RIGHTPOWER);
		LED = Red;
		PWM1_ENABLE_R &= ~0x0000000C; // stop both wheels
		WHEEL_DIR=LEFTPIVOT;
		PWM1_ENABLE_R |= 0x0000000C; // Enable both wheels

	}
	
	else if (ahead_dist > CRASH && left_dist < CRASH){ // right pivot
		PWMA_Duty(LEFTPOWER, RIGHTPOWER);
		LED = Red;
		PWM1_ENABLE_R &= ~0x0000000C; // stop both wheels
		WHEEL_DIR=RIGHTPIVOT;
		PWM1_ENABLE_R |= 0x0000000C; // Enable both wheels
	}
	

	else if(left_dist > CRASH){ //left turn
		PWMA_Duty(LEFTPOWER, RIGHTPOWER);
		LED = Red;
		WHEEL_DIR = FORWARD;
		PWM1_ENABLE_R &= ~0x00000008; // Disable right wheel
		PWM1_ENABLE_R |= 0x0000004; // Enable left wheel	
	}
	
	else if(right_dist > CRASH){ //right turn
		PWMA_Duty(LEFTPOWER, RIGHTPOWER);
		LED = Red;
		WHEEL_DIR = FORWARD;
		PWM1_ENABLE_R |= 0x00000008; // Enable right wheel
		PWM1_ENABLE_R &= ~0x0000004; // Disable left wheel
	}
	
	
	//if no object detected continue straight
	else{
		PWMA_Duty(START_SPEED1, START_SPEED2); 
		LED = Green;
		WHEEL_DIR = FORWARD;
		PWM1_ENABLE_R|= 0x0000000C; //enable both wheels
	}
}

//1 ms delay
void Delay(void){
	unsigned long volatile time;
  	time = 727240*100/91;  // 0.01sec
 	 while(time){
		time--;
  	}
}
