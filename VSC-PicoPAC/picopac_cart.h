/*
//                       PicoPAC MultiCART by Andrea Ottaviani 2024
//
//  VIDEOPAC  multicart based on Raspberry Pico board -
//
//  More info on https://github.com/aotta/ 
//
//   parts of code are directly from the A8PicoCart project by Robin Edwards 2023
//  
//   Needs to be a release NOT debug build for the cartridge emulation to work
// 
//   Edit myboard.h depending on the type of flash memory on the pico clone//
//
//   v. 1.0 2024-08-05 : Initial version for Pi Pico 
//
*/



#ifndef __picopac_CART_H__
#define __picopac_CART_H__

#define FLAG_MASK       0x0FC00000L
#define PSEN_PIN 22

/* Extram read*/
#define EXTRAM_READ() {\
			if((gpio_get(CS_PIN) == 0) && (gpio_get(NOTCS_PIN) == 1) && (gpio_get(WR_PIN)==1)) {\
				SET_DATA_MODE_OUT;\
				gpio_put_masked(DATA_PIN_MASK,(extram[addr & 0xff])<<D0_PIN);\
			}\
}
#define EXTRAM_WRITE() {\
		  	if((gpio_get(CS_PIN)==0) && (gpio_get(NOTCS_PIN) == 1) && (gpio_get(WR_PIN)==0)) {\
				   extram[addr & 0xff]=((pins & DATA_PIN_MASK)>>D0_PIN);\
			} \
}
#endif