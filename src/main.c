/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 7.1.2   2017-05-31

The MIT License (MIT)
Copyright (c) 2009-2017 Atollic AB

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "initial_ization.h"
#include "user_interface.h"
#include "main.h"
#include "lcd.h"


/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

int main(void)
{
  /*****************************This came with project just gona leave for now*******************************/
  /* Initialize LEDs */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);

  /* Turn on LEDs */
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
  STM_EVAL_LEDOn(LED5);
  STM_EVAL_LEDOn(LED6);
  /**
  *  IMPORTANT NOTE!
  *  The symbol VECT_TAB_SRAM needs to be defined when building the project
  *  if code has been located to RAM and interrupts are used.
  *  Otherwise the interrupt table located in flash will be used.
  *  See also the <system_*.c> file and how the SystemInit() function updates
  *  SCB->VTOR register.
  *  E.g.  SCB->VTOR = 0x20000000;
  */

  /********************************testing variables gona leave in for know *****************************/

  //used for testing the selector
  selector_state testvfo,testlfo;	//

  /* Used for Testing ADC Values can be thrown out*/
  uint16_t vfo_amp = ADCBuffer[0];
  uint16_t vfo_freq = ADCBuffer[1];
  uint16_t lfo_amp = ADCBuffer[2];
  uint16_t lfo_freq = ADCBuffer[3];
  uint16_t volume = ADCBuffer[4];
  uint16_t env_attack = ADCBuffer[5];
  uint16_t env_decay = ADCBuffer[6];
  uint16_t env_sustain = ADCBuffer[7];
  uint16_t env_release = ADCBuffer[8];
  uint16_t fc_low = ADCBuffer[9];
  uint16_t fc_high = ADCBuffer[10];
  uint16_t fc_resonance = ADCBuffer[11];
  uint16_t gain = ADCBuffer[12];

/************************************************************************************************************/




  /**************************** Run Initialization functions timer for tim2 started in init_adc*****************************/

  menubutton.button=back;					//initializes menubutton state for startup
  init_gpios();								//initialize gpios
  init_push_buttons();						//initialize menu navigation buttons
  init_adc(ADCBuffer);						//initialize ADC, do this last because it starts the timer
  //init_spi();								//initialize the SPI for LCD
  init_parallel();							//initializes all the GPIO's for parallel LCD communication
  lcd_init();								//initializes LCD screen
  update_selector_state();					// get startup state

  LCD_CLEAR();
 // LCD_WRITE(55);
  //lcd_writeline("01234567890123456789",0);
  LCD_TX(0,0,0);
  LCD_TX(0,0,1);
  LCD_TX(0,0,2);
  LCD_TX(0,0,4);
  LCD_TX(0,0,8);
  LCD_TX(0,0,16);
  LCD_TX(0,0,32);
  LCD_TX(0,0,64);
  LCD_TX(0,0,128);
  LCD_TX(0,1,0);
  LCD_TX(1,0,0);


  int i = 0;
  int j = 0;

  /* Infinite loop */
  while (1)
  {


	  // Waste some time
	  for (j = 0; j < 5000000; j++){
	  }

	//used for testing wave selectors
	testlfo = lfo_state;
	testvfo = vfo_state;

	//used for testing ADC
	vfo_amp = ADCBuffer[0];
	vfo_freq = ADCBuffer[1];
	lfo_amp = ADCBuffer[2];
	lfo_freq = ADCBuffer[3];
	volume = ADCBuffer[4];
	env_attack = ADCBuffer[5];
	env_decay = ADCBuffer[6];
	env_sustain = ADCBuffer[7];
	env_release = ADCBuffer[8];
	fc_low = ADCBuffer[9];
	fc_high = ADCBuffer[10];
	fc_resonance = ADCBuffer[11];
	gain = ADCBuffer[12];

  }
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}








/**********************************************Interrupt handlers**********************************/
//Not sure if i can keep these in the other files so i put em in the main


