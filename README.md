# Project interduction
- **project platform** : stm32f103
- **Software** : STM32CubeMX   Keil5
- **Programming language** : C    		(Tie : Annotate Chinese)
- **Main chip** : XTR111AIDGQ  ADM2483

# Project function introduction
The project is used as a slave to communicate with the host through the 485 bus. The host sends commands through the 485 bus to reach the io port output on the control board. Real-time monitoring of the io port changes in the board, and feedback to the host through the 485 bus in time if there is a change.

The hardware is divided into an automatic transceiver 485 communication module based on the 4 - 20mA industrial current output of the XTR111AIDGQ chip and the some io port formed with optocoupler relays. The host can drive the io port on the board to work through a specific command set. The code uses polling to detect changes in the io port. Code control pwm duty cycle to achieve 4-20mA industrial current output.
