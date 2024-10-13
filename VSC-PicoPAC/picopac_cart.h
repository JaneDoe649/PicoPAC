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

// Pico pin usage definitions

#define A0_PIN    0
#define A1_PIN    1
#define A2_PIN    2
#define A3_PIN    3
#define A4_PIN    4
#define A5_PIN    5
#define A6_PIN    6
#define A7_PIN    7
#define A8_PIN    8
#define A9_PIN    9
#define A10_PIN  10
#define A11_PIN  11
#define P11_PIN  12
#define P10_PIN  13
#define D0_PIN   14
#define D1_PIN   15
#define D2_PIN   16
#define D3_PIN   17
#define D4_PIN   18
#define D5_PIN   19
#define D6_PIN   20
#define D7_PIN   21
#define PSEN_PIN 22
#define WR_PIN 23
#define CS_PIN 24   // P14
#define LED_PIN  25 
#define NOTCS_PIN  26 
#define TD_PIN  27 

// Pico pin usage masks
#define A0_PIN_MASK     0x00000001L //gpio 0
#define A1_PIN_MASK     0x00000002L
#define A2_PIN_MASK     0x00000004L
#define A3_PIN_MASK     0x00000008L
#define A4_PIN_MASK     0x00000010L
#define A5_PIN_MASK     0x00000020L
#define A6_PIN_MASK     0x00000040L
#define A7_PIN_MASK     0x00000080L
#define A8_PIN_MASK     0x00000100L
#define A9_PIN_MASK     0x00000200L
#define A10_PIN_MASK    0x00000400L
#define A11_PIN_MASK    0x00000800L
#define P11_PIN_MASK    0x00001000L  // P11 high bank select
#define P10_PIN_MASK    0x00002000L  // P10 low bank select
#define D0_PIN_MASK     0x00004000L //gpio 14
#define D1_PIN_MASK     0x00008000L
#define D2_PIN_MASK     0x00010000L
#define D3_PIN_MASK     0x00020000L
#define D4_PIN_MASK     0x00040000L
#define D5_PIN_MASK     0x00080000L  // gpio 19
#define D6_PIN_MASK     0x00100000L
#define D7_PIN_MASK     0x00200000L
#define PSEN_PIN_MASK   0x00400000L //gpio 22
#define WR_PIN_MASK     0x00800000L //gpio 23  
#define CS_PIN_MASK     0x01000000L //gpio 24   - P14  
#define LED_PIN_MASK    0x02000000L //gpio 25   
#define NOTCS_PIN_MASK  0x04000000L //gpio 26  
#define TD_PIN_MASK     0x08000000L //gpio 27

#define RESTART			0x10000000L //gpio 28

// Aggregate Pico pin usage masks
#define ALL_GPIO_MASK  	0x0FFFFFFFL
#define BUS_PIN_MASK    0x00003FFFL 
#define BANK_PIN_MASK   0x00003000L
#define DATA_PIN_MASK   0x003FC000L
#define FLAG_MASK       (0x0FC00000L | RESTART)
#define ALWAYS_IN_MASK  (BUS_PIN_MASK | FLAG_MASK)
#define ALWAYS_OUT_MASK (DATA_PIN_MASK )

#define SET_DATA_MODE_OUT   gpio_set_dir_out_masked(DATA_PIN_MASK)
#define SET_DATA_MODE_IN    gpio_set_dir_in_masked(DATA_PIN_MASK)
#define SET_LED_ON    	gpio_init(PICO_DEFAULT_LED_PIN);(PICO_DEFAULT_LED_PIN,GPIO_OUT);gpio_put(PICO_DEFAULT_LED_PIN,true);
#define SET_LED_OFF    	gpio_init(PICO_DEFAULT_LED_PIN);gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);gpio_put(PICO_DEFAULT_LED_PIN,false);

static bool restart(uint32_t pins);

#endif