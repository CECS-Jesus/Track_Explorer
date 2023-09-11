# Track_Explorer

## Project Description

In this project, you will build an autonomous Robot Car that can autonomously explore a track. Our goals in this project are to understand basic microcontroller hardware components, including GPIO, interrupt, analog to digital conversion (ADC), and hardware PWM, and to build an embedded control system that makes use of those components to interface with switches, LEDs, DC motors, and IR sensors.

The robot car will autonomously explore a track with two walls, starting at the open end, exploring the whole track, and then returning to the open end where it starts. The shape of the track is unknown, but there will be both left turns and right turns, and the track has no branches. In other words, there is only one way to the closed end and one way back to the open end.

## Hardware Requirements

1. PLL is required to set the system clock to be 16MHz using an external clock source.
2. Hardware PWM is required to drive the DC motors.
3. Three IR sensors are required for the robot to detect obstacles around it.
4. GPIO assignments:

| GPIO Pins | Devices                  |
|-----------|--------------------------|
| PE1       | Left IR sensor output    |
| PE4       | Front sensor output      |
| PE5       | Right IR sensor output   |
| PA6       | Hardware PWM output for left motor   |
| PA7       | Hardware PWM output for right motor  |
| PB45      | Direction pins for left motor        |
| PB23      | Direction pins for right motor       |
| PF123     | Onboard color LEDs                  |
| PF0       | Onboard switch 2                    |
| PF4       | Onboard switch 1                    |

5. Onboard LEDs are used to indicate the status of the robot:

| Color | Indication                                              |
|-------|---------------------------------------------------------|
| RED   | Robot Car is too close to an obstacle: less than 15cm away. |
| BLUE  | Mission accomplished: the car finished exploring the whole track and back to the open end.        |
| GREEN | Robot Car is traveling in a safe area: at least 10cm away from any obstacles.                          |

## Project Highlights

- Embedded Systems project
- Developed an autonomous mini car to complete a lap on a track by programming an ARM-based MCU in C, using 3 IR sensors, 2 stepper motors, and assembling a power distribution board.
- Formulated an optimal time complexity distance algorithm based on analog IR sensor signals, increasing distance calculation throughput by 50%.
- Configured a hardware-based Pulse Width Modulation (PWM) to evenly distribute power across 2 stepper motors, allowing fine-tuning of the car’s velocity and alignment.
- Soldered, assembled, and designed schematic.

### PLL Configuration

Phase Locked Loop (PLL) is utilized to generate a stable output high frequency from a low frequency signal. The PLL allows us to modify the execution speed of the TM4C microcontroller. Initially, we set the clock frequency to 16MHz to fulfill the project. For the chassis extra credit, we set the frequency to 50 MHz. In order to achieve this frequency, the 400 MHz bus frequency was required to be divided by SYSDIV2+1. The value of 7 in the SYSDIV2 outputs the frequency of 50Mhz since 400Mhz/(7+1) = 50Mhz.

### Hardware PWM and DC Motors

Hardware PWM was utilized to control the DC Motors of the robot. Hardware PWM is used to control the power delivered to the DC motors, providing an efficient and accurate way to control current delivered to the motors. The L298N motor driver was also utilized in order to drive the direction and speed of the DC motors. This is an H-bridge that enables a voltage to be applied across a load in either direction depending on the preference of the user. The L298N motor driver has two pins for PWM speed to allow the user to configure the speed of the motors depending on their respective duty cycles.

### GPIO Interrupts

GPIO Interrupts were used in order for the robot to activate and deactivate on the press of the switches. Interrupts were necessary to use because you must wait for an interrupt before the robot could start to move. Once the switch is pressed, it will cause an interrupt, and the robot could begin to read IR sensor values and move accordingly to the steering algorithm.

### Analog to Digital Conversion (ADC)

Analog to Digital conversion is required in order to read values from the IR sensors. Analog to Digital conversion is the process of converting an analog signal into a digital signal that can be processed by a digital system such as the TM4C microcontroller. Since we are using IR sensors which are analog components, we must convert the analog signals from the IR sensor’s into a digital signal in order for our IR sensors to read proper values to the microcontroller so that the robot could function as intended.

### Parts List

LEDs

Breadboard

Jumper wires
TM4C Microcontroller
USB cable (for programming the microcontroller)

Power supply 12 volts

L298N H-bridge

3 IR sensor’s

2 DC motors

### Demo Video:
https://youtube.com/shorts/poySFVOHSc8?si=13veGEQ6q29lasUS
