/*
 * user_interface.c
 *
 *  Created on: Jun 10, 2017
 *      Author: jjank
 */

#include "user_interface.h"
#include "stm32f4xx_gpio.h"
#include "lcd.h"
#include "main.h"


/*
 * Gets and updated the state of both the selector rotary switches
 * This funtion is called by the tim2 interrupt handler
 */
void update_selector_state (){

	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 1){
		vfo_state = sine;
	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8) == 1){
		vfo_state = sawtooth;
	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 1){
		vfo_state = square;
	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10) == 1){
		vfo_state = triangle;
	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11) == 1){
		vfo_state = other2;
	}
//could put some fault detection at the end of these if statements come back later if theres time

	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) == 1){
		lfo_state = sine;
	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) == 1){
		lfo_state = sawtooth;
	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) == 1){
		lfo_state = square;
	}else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15) == 1){
		lfo_state = triangle;
	}else if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1){
		lfo_state = other2;
	}
}

void init_state(){

	current_menu_state.menu_state = startup;
	current_menu_state.cursor_option = 1;
	current_menu_state.adsr_mod = NO_MOD;
	current_menu_state.lfo_mod = NO_MOD;
	current_menu_state.filterst8 = no_filter;
	current_menu_state.secondary_vco = 0;

}

void handle_enter(){

}

void handle_back(){

}

void handle_up(){

}

void handle_down(){

}




void display_new_menu(){
	switch(current_menu_state.menu_state){

		case startup:
			lcd_writeline("     Main  Menu     ", 1);
			lcd_writeline(" ADSR     Filter    ", 2);
			lcd_writeline(" LFO      Sec VCO   ", 3);
			LCD_SETDDRAM(0x14);
			break;

		case filter:
			lcd_writeline("    Filter  Menu    ", 1);
			lcd_writeline(" Off      HighPass  ", 2);
			lcd_writeline(" LowPass  BandPass  ", 3);
			LCD_SETDDRAM(0x14);
			break;

		case ADSR:
			lcd_writeline("     ADSR  Menu     ", 1);
			lcd_writeline(" Off      Modulation", 2);
			lcd_writeline("                    ", 3);
			LCD_SETDDRAM(0x14);
			break;

		case ADSR_Mod:
			lcd_writeline("ADSR Modulation Menu", 1);
			lcd_writeline(" VCO_AMP  VCO_FREQ  ", 2);
			lcd_writeline(" LFO_AMP  LFO_FREQ  ", 3);
			LCD_SETDDRAM(0x14);
			break;

		case LFO:
			lcd_writeline("     LFO   Menu     ", 1);
			lcd_writeline(" OFF      Modulation", 2);
			lcd_writeline("                    ", 3);
			LCD_SETDDRAM(0x14);
			break;

		case LFO_Mod:
			lcd_writeline("LFO Modulation Menu ", 1);
			lcd_writeline(" VCO_AMP  VCO_FREQ  ", 2);
			lcd_writeline(" LFO_AMP  LFO_FREQ  ", 3);
			LCD_SETDDRAM(0x14);
			break;

		case secondaryVCO:
			lcd_writeline(" Secondary VCO Menu ", 1);
			lcd_writeline(" OFF      ON        ", 2);
			lcd_writeline("                    ", 3);
			LCD_SETDDRAM(0x14);
			break;
	}

}


