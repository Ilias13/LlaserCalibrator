/*
 * inits.c
 *
 *  Created on: Mar 26, 2018
 *      Author: Ilias Mastoras
 *
 *     Construction of the initialization functions.
 *     It is better to keep them separated from the
 *     main..
 *      When main function calls the initializeAll
 *     function, then all the inits happen.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"

#include "inits.h"

// ***These are the initialization functions for the hardware.***


// This function initializes the clock.
void initializeClock() {
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
}

// This function initializes the interrupt.
void initializeInterrupt(){
    IntMasterEnable();
}

void Timer0IntHandler(void) {
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}


// This function initializes the GPIO.
void initializeGPIOandPWM(){
    // We enable PWM1 and GPIOD modules for both PD0 and PD1 pins.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // PWM PD0, Laser A
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD0_M1PWM0);

    // PWM PD1, Laser B
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PD1_M1PWM1);

    PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, false); // output for PD0
    PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, false); // output for PD1
    PWMGenEnable(PWM1_BASE, PWM_GEN_0); // Enable the generator 0 which works for both pins(PD0 and PD1)

    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN); // Configure module 1 PWM generator 0 as a down-counter and load the count value.
                                                              // what exactly this line does?

}

// This function initializes the Timer.
void initializeTimer(){
    uint32_t period = (SysCtlClockGet() / 1000) / 1;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);

    TimerLoadSet(TIMER0_BASE, TIMER_A, period - 1);

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER0_BASE, TIMER_A);
}

void initializeUART(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);
}

void initializeAll() {
    initializeUART();
    initializeClock();
    initializeTimer();
    Timer0IntHandler();
    initializeInterrupt();
    initializeGPIOandPWM();
 }






