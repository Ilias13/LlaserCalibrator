/*
 * inits.h
 *
 *  Created on: Mar 26, 2018
 *      Author: Ilias Mastoras
 *
 *      This header file will hold all the prototypes
 *      of the initialization functions. It is better
 *      to keep all the inits separated from the main.c
 */

#ifndef INITS_H_
#define INITS_H_

#ifdef __cplusplus
extern "C"
{
#endif

extern void initializeClock();
extern void initializeInterrupt();
extern void Timer0IntHandler();
extern void initializeGPIOandPWM();
extern void initializeTimer();
extern void initializeUART();
extern void initializeAll();

#ifdef __cplusplus
}
#endif

#endif /* INITS_H_ */
