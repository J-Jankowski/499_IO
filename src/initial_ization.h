/*
 * initial_ization.h
 *
 *  Created on: May 31, 2017
 *      Author: jjank
 */

#ifndef INITIAL_IZATION_H_
#define INITIAL_IZATION_H_


#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"

#define NO_OF_CONVERSION 13


//Global array for ADC converted values
volatile uint16_t ADC1ConvertedValue[NO_OF_CONVERSION];


//#define
/*define for specific board need to update when I get my board Also will need
to make sure this is correct when porting to final board
*/

//initialize all the peripheral busses
void init_rcc();


//initialize dma for adc data transfer
void init_dma();


// Initializes timer for Interrupt to poll all Potentiometers
// Note timer interrupt can be disabled when necessary
void init_timer2();


// Initializes GPIO pins for all buttons and switches
void init_pins();


// Initializes ADC pins and Calibrates ADC
void init_adc();









#endif /* INITIAL_IZATION_H_ */
