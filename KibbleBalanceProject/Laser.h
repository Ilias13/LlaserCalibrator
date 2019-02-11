/*
 * Laser.h
 *
 *  Created on: Apr 4, 2018
 *      Author: Ilias Mastoras
 */

#ifndef LASER_H_
#define LASER_H_

/* Structure for the PWM Generators.
   This contains the laser's frequency and duty cycle values and
   the methods to set those values.
*/

typedef struct {
    volatile uint32_t frequency;
    volatile uint32_t dutyCycle;
    volatile uint32_t period;
    volatile uint32_t width;
    volatile uint32_t PWMClock;
}Laser;

// This function initializes the PWM with these values.
// Like a constructor.
void initializeFreqDuty(Laser* laser){
        laser->frequency = 700;
        laser->dutyCycle = 0;
        laser->PWMClock = SysCtlClockGet() / 64;
        laser->period  = (laser->PWMClock / laser->frequency) - 1;
        laser->width = (laser->dutyCycle * laser->period) / 100;
}

void PinGenConfig(Laser* laser, char pin){
    //Both pins work with generator 0.
    laser->period = (laser->PWMClock / laser->frequency) - 1;
    laser->width = (laser->dutyCycle * laser->period) / 100;

  //  laser->width = laser->width ? laser->width : 1;

    if(pin == 'A'){
        // Change the settings in PD0 (pin 0) for Laser A
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, laser->period);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, laser->width);
        if(laser->width == 0){
            PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, false);
        }else{
            PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
        }
    } else {
        // Change the settings in PD1 (pin 1) for Laser B
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, laser->period);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, laser->width);
        if(laser->width == 0){
            PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, false);
        }else{
            PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
        }
    }
}

bool setFrequency(Laser* laser, char pin, uint32_t value ){
    laser->frequency = value;
    PinGenConfig(laser, pin);
    return true;
}

bool setDutyCycle(Laser* laser, char pin, uint32_t value){
    laser->dutyCycle = value;
    PinGenConfig(laser, pin);
    return true;
}


#endif /* LASER_H_ */
