/*
 * initial_ization.h
 *
 *  Created on: May 31, 2017
 *      Author: jjank
 */


// *****************************DEFINITIONS******************************


#define myTIM2_PRESCALER ((uint16_t)0x03E8)				//want ADC to run every 75ms =13.3Hz board is at 168MHz prescale by 1000
#define myTIM2_PERIOD ((uint32_t)0x3138)				//so need to count to count to 12600=0x3138 gona change for testing
#define NUM_CHANNELS 5



// Initializes ADC pins and Calibrates ADC Do DMA transfer
void init_adc(volatile uint16_t ADCBuffer[NUM_CHANNELS]);



