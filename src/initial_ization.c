/*
 * initial_ization.c
 *
 *  Created on: May 29, 2017
 *      Author: jjank
 */

#include <stdio.h>
#include <stdlib.h>
#include "initial_ization.h"

// *****************************DEFINITIONS******************************


#define myTIM2_PRESCALER ((uint16_t)0x03E8)				//want ADC to run every 75ms =13.3Hz board is at 168MHz prescale by 1000
#define myTIM2_PERIOD ((uint32_t)0xffff)				//so need to count to count to 12600=0x3138 gona change for testing




//******************************Helper functions***************************







//*************************************************************************

/*
 *initializes all peripheral busses
*/

void init_rcc(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 		/* Turn on AHB Bus Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); /* Turn on AHB Bus Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); /* Turn on AHB Bus Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//Enable TIM2 clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);		//Enable DMA2 clock
	RCC_AHB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); // potentially need see how it goes with one
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //potentially need if need more adc's but I think 2 is good
}



/* Initializes timer for Interrupt to pace the ADC's all Potentiometers
 *  Note timer interrupt can be disabled when necessary
 *  also note timer is not started the timer must be enabled in the main
 *  This is last action of the ADC
 */
void init_timer2(){

	 TIM_TimeBaseInitTypeDef time_struct;						//initialization structs
	 NVIC_InitTypeDef NVIC_Init_struct;

	 TIM_TimeBaseStructInit(&time_struct);						//sets struct to default values

	 //set up timer to set flag at approx 13Hz might need to adjust this
	 time_struct.TIM_Prescaler =  myTIM2_PRESCALER;
	 time_struct.TIM_CounterMode = TIM_CounterMode_Up;
	 time_struct.TIM_Period = myTIM2_PERIOD;
	 time_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseInit(TIM2, &time_struct);
	 //set trigger and enable interrupts
	 TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Update);
	 TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);


	 /*set up interrupts
	 NVIC_Init_struct.NVIC_IRQChannel = TIM2_IRQn;
	 NVIC_Init_struct.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init_struct.NVIC_IRQChannelPreemptionPriority = 0x0F;
	 NVIC_Init_struct.NVIC_IRQChannelSubPriority = 0x0F;
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 NVIC_Init(&NVIC_Init_struct);
*/

}




/*Initalize DMA transfer for ADC conversion transfer
 * call this after timer setup and before ADC Setup
 */

void init_dma(){

	DMA_InitTypeDef DMA_Init_struct;
	//reset
	DMA_StructInit(&DMA_Init_struct);

	/* config of DMAC */
	DMA_Init_struct.DMA_Channel = DMA_Channel_0; 						// See Tab 43
	DMA_Init_struct.DMA_BufferSize = NO_OF_CONVERSION; 								// 13 ADC channels
	DMA_Init_struct.DMA_DIR = DMA_DIR_PeripheralToMemory; 				// ADC1 to mem
	DMA_Init_struct.DMA_FIFOMode = DMA_FIFOMode_Disable; 				// no FIFO
	DMA_Init_struct.DMA_FIFOThreshold = 0;
	DMA_Init_struct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Init_struct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init_struct.DMA_Mode = DMA_Mode_Circular; 						//circular buffer
	DMA_Init_struct.DMA_Priority = DMA_Priority_High; 					// high priority
	/* config of memory */
	DMA_Init_struct.DMA_Memory0BaseAddr = (uint32_t)ADC1ConvertedValue; //array for converted values
	DMA_Init_struct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 	//16 bit
	DMA_Init_struct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_Init_struct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_Init_struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_Init_struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_Init(DMA2_Stream0, &DMA_Init_struct); 							//Table 43 for mapping
	DMA_Cmd(DMA2_Stream0, ENABLE);
}







//Initialize GPIO pins call this first
void init_pins(){

	/*****************************Initialize ADC Pins*****************************/
	GPIO_InitTypeDef GPIO_InitStructure;

	//A bank pins
	//GPIO_Pin_0	VFO-Amplitude
	//GPIO_Pin_1	VFO-Frequency
	//GPIO_Pin_2	LFO-Amplitude
	//GPIO_Pin_3	LFO-Frequency
	//GPIO_Pin_4	VCO-Volume
	//GPIO_Pin_5	ENVELOPE-Attack
	//GPIO_Pin_6	ENVELOPE-decay
	//GPIO_Pin_7	ENVELOPE-Sustain
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 |GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//B bank pins
	//GPIO_Pin_0	ENVELOPE-Release
	//GPIO_Pin_1	FILTER-FreqLow
	GPIO_InitStructure.GPIO_Pin = 0;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	//C bank pins//GPIO_Pin_0	FILTER-FreqHigh
	//GPIO_Pin_1	FILTER-FreqResonance
	//GPIO_Pin_4	FILTER-FreqGain
	GPIO_InitStructure.GPIO_Pin = 0;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);





	/*****************************Initialize Button Pins*****************************/


}




/* Initializes ADC pins and Calibrates ADC Going to use 1 adc and single scan conversion
 * Call this guy last after timer and DMA Then enable TIM2
 */

void init_adc(){

	//reset
	ADC_Cmd(ADC1, DISABLE);
	ADC_DeInit();

	/*****************************Initialize ADC******************************/
	ADC_CommonInitTypeDef  adc_com_init_struct;
	ADC_InitTypeDef  adc_init_struct;

	//populates structs with reset values
	ADC_StructInit(&adc_init_struct);
	ADC_CommonStructInit(&adc_com_init_struct);

	//Initialize Common ADC
	adc_com_init_struct.ADC_Mode = ADC_Mode_Independent;
	adc_com_init_struct.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_com_init_struct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled ;  //gona see what alex says
	adc_com_init_struct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
	ADC_CommonInit(&adc_com_init_struct);

	//Initialize ADC
	adc_init_struct.ADC_Resolution = ADC_Resolution_10b;			// gona try 10 bits to help with any noise
	adc_init_struct.ADC_ScanConvMode = ENABLE;
	adc_init_struct.ADC_ContinuousConvMode = DISABLE;
	adc_init_struct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_RisingFalling;
	adc_init_struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
	adc_init_struct.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_struct.ADC_NbrOfConversion = NO_OF_CONVERSION;
	ADC_Init(ADC1, &adc_init_struct);





	//configure channels
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);		//VFO-Amplitude
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_480Cycles);		//VFO-Frequency
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_480Cycles);		//LFO-Amplitude
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_480Cycles);		//LFO-Frequency
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_480Cycles);		//VCO-Volume
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_480Cycles);		//ENVELOPE-Attack
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_480Cycles);		//ENVELOPE-decay
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_480Cycles);		//ENVELOPE-Sustain
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_480Cycles);		//ENVELOPE-Release
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 10, ADC_SampleTime_480Cycles);	//FILTER-FreqLow
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 11, ADC_SampleTime_480Cycles);	//FILTER-FreqHigh
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 12, ADC_SampleTime_480Cycles);	//FILTER-FreqResonance
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 13, ADC_SampleTime_480Cycles);	//FILTER-FreqGain

    //this section is for testing adc
    NVIC_InitTypeDef NVIC_Init_struct;
    NVIC_Init_struct.NVIC_IRQChannel = ADC_IRQn;
    NVIC_Init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init_struct.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_Init_struct.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_Init(&NVIC_Init_struct);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE);


    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 **************************************************************/
    ADC_Cmd(ADC1, ENABLE);
    ADC_SoftwareStartConv(ADC1);

}




















