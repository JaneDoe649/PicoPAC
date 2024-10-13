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

//#define debugging
//#define optimize
#define maxfiles 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "pico/platform.h"
#include "pico/stdlib.h"
#include "hardware/vreg.h"
#include "pico/divider.h"
#include "hardware/flash.h"
#include "hardware/sync.h"


#include "selectgame.h"

#include "tusb.h"
#include "ff.h"
#include "fatfs_disk.h"
#include <sys/types.h>
#include "hardware/clocks.h"

#include "translate.c"
#include "picopac_cart.h"

#ifdef optimize
#include "pico/time.h"
uint32_t msSinceBoot1;
uint32_t msSinceBoot2;
#endif


 void picopac_cart_main();
 void updaterom(unsigned char *m, int memblock);
 #ifdef debugging
 void debugFS(unsigned char *array);
 #endif

// We're going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.


//#define BINLENGTH  1024*128+1
unsigned char __attribute__((aligned(4))) rom_table[2][8][4096];
volatile int rom_buffer = 0;
volatile int next_rom_buffer = 1;
unsigned char extROM[1024];
unsigned char RAM[1024];

char path[5];
char errorBuf[40];
int romsize;
volatile u_int8_t bank_type=1;
volatile u_int8_t new_bank_type=1;
volatile char gamechoosen=0;
//volatile char newgame=0;
volatile char resetnow=0;
volatile char menu_loaded=0;
char extram[0xff];


////////////////////////////////////////////////////////////////////////////////////
//                     Error(N)
////////////////////////////////////////////////////////////////////////////////////
void error(int numblink){
 	multicore_lockout_start_blocking();	
    sleep_ms(1000);
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);
  while(1){

    for(int i=0;i<numblink;i++) {
      gpio_put(PICO_DEFAULT_LED_PIN,true);
      sleep_ms(600);
      gpio_put(PICO_DEFAULT_LED_PIN,false);
      sleep_ms(500);
    }
  sleep_ms(2000);
  }
}

/*
 Theory of Operation
 -------------------
 //
 //
 //
*/
#pragma GCC push_options
#pragma GCC optimize ("O3")

void __not_in_flash_func(core1_main()) {
    uint32_t addr;
    char dataWrite=0;
    uint32_t pins;
    u_int8_t bank=0;
	u_int8_t romlatch=0;

	multicore_lockout_victim_init();	
   
    //gpio_set_dir_in_masked(ALWAYS_IN_MASK);

	while(1) {
	//newgame=0;
	gamechoosen=0;
	resetnow=0;

		// Initial conditions
		//SET_DATA_MODE_IN;
		#ifdef optimize
		uint64_t bt = to_us_since_boot(get_absolute_time());
		printf("Boot time : %" PRIu64 " - Ready to play\n", bt);
		#endif
		while(1) {
				pins=gpio_get_all();
				addr = (pins & 0b0111111111111);  
				bank=3-((gpio_get(P10_PIN)+(gpio_get(P11_PIN)*2)));
				if (gpio_get(PSEN_PIN)==0) {
					SET_DATA_MODE_OUT;
						gpio_put_masked(DATA_PIN_MASK,(rom_table[rom_buffer][bank][addr])<<D0_PIN);
						if ((resetnow == 1) && (rom_table[rom_buffer][bank][addr] == 0x00) && (rom_table[rom_buffer][bank][addr-1] == 0x04)) { //JMP 00h on bus
							//newgame=1;
							while (gpio_get(PSEN_PIN)==0) { //Wait end of cycle
							}
							SET_DATA_MODE_IN;
							break;
						}
						if (restart(pins))
							break; 
				}
				if((gpio_get(CS_PIN)==0) && (gpio_get(NOTCS_PIN) == 1) && (gpio_get(WR_PIN)==0)) {
					extram[addr & 0xff]=((pins & DATA_PIN_MASK)>>D0_PIN);	
					if (extram[0xff]==0xaa) {
						gamechoosen=extram[0xfe];
					} 
				}		
			SET_DATA_MODE_IN;
			}
		rom_buffer = next_rom_buffer;
		//SET_DATA_MODE_IN;
		
		switch (new_bank_type) {
		case 0:  // standard 2k / 4k
			while(1) {	
				pins=gpio_get_all();
				addr = (pins & 0b0111111111111);  
				if (gpio_get(P10_PIN)) {
					bank=0;
				} else {
					bank=1;
				}
				if (gpio_get(PSEN_PIN)==0) {
					SET_DATA_MODE_OUT;
					gpio_put_masked(DATA_PIN_MASK,(rom_table[rom_buffer][bank][addr])<<D0_PIN);
				}
				if (restart(pins))
					break;
			SET_DATA_MODE_IN;
			}
			break;
		case 1:  // standard 8k
			while(1) {	
				pins=gpio_get_all();
				addr = (pins & 0b0111111111111);  
				bank=3-((gpio_get(P10_PIN)+(gpio_get(P11_PIN)*2)));
				
				if (gpio_get(PSEN_PIN)==0) {
					SET_DATA_MODE_OUT;
					gpio_put_masked(DATA_PIN_MASK,(rom_table[rom_buffer][bank][addr])<<D0_PIN); 
				}
				if (restart(pins))
						break;
				SET_DATA_MODE_IN;
			}
			break;
		case 2:   // XROM
			while(1) {	
				pins=gpio_get_all();
				addr = (pins & 0b0111111111111); // for all cart but xrom
				if ((gpio_get(P11_PIN)==1 && gpio_get(NOTCS_PIN)==1)) {
						SET_DATA_MODE_OUT;
						gpio_put_masked(DATA_PIN_MASK,(extROM[addr&0x3ff])<<D0_PIN);
				} else {
					if (gpio_get(PSEN_PIN)==0) {
						SET_DATA_MODE_OUT;
						gpio_put_masked(DATA_PIN_MASK,(rom_table[rom_buffer][0][addr])<<D0_PIN);	
					}
				}
				if (restart(pins))
					break; 	
			SET_DATA_MODE_IN;
			}
			break;
		case 3:   // 12k/16k
			while(1) {	
				pins=gpio_get_all();
				addr = (pins & 0b0111111111111); // for all cart but xrom
				if((gpio_get(CS_PIN)==0) && (gpio_get(WR_PIN)==0)) {
					if ((addr & 0xff)>=0x80) {
						romlatch=((~(pins & DATA_PIN_MASK)>>D0_PIN)&7);
					}	
				} else {
				if (gpio_get(P10_PIN)==0) {
					bank=romlatch;
				} else {
					bank=0;
				}
					if (gpio_get(PSEN_PIN)==0) {
						SET_DATA_MODE_OUT;
						gpio_put_masked(DATA_PIN_MASK,(rom_table[rom_buffer][bank][addr])<<D0_PIN);
					}
				}
				if (restart(pins))
					break; 
				SET_DATA_MODE_IN;
			//}
			}
			break;
		}
	}
}

bool  __not_in_flash_func(restart)(uint32_t pins) {
	unsigned char restart_sequence[] = {0x00, 0x00, 0x00, 0x00, // synchronize cpu 
										0x14, 0xF1, // call init
										0xc7, 0x53, 0xf8, 0xd7, // reset stack pointer
										0x04, 0x00}; // jmp 0x00
	int p = 0;
	if (pins & RESTART) {
		menu_loaded = 0; // ask menu loading
		while (!menu_loaded) {
				if (gpio_get(PSEN_PIN)==0) {
				SET_DATA_MODE_OUT;
				gpio_put_masked(DATA_PIN_MASK,(0x00)<<D0_PIN); // wait until menu is loaded with NOP
				while (gpio_get(PSEN_PIN)==0) {} //Wait end of cycle
			}
			SET_DATA_MODE_IN;
		}
		while (p < sizeof(restart_sequence)) {
			if (gpio_get(PSEN_PIN)==0) {
				SET_DATA_MODE_OUT;
				gpio_put_masked(DATA_PIN_MASK,(restart_sequence[p++])<<D0_PIN); // menu is loaded, let's JMP 0x00
				while (gpio_get(PSEN_PIN)==0) {} //Wait end of cycle
			}
			SET_DATA_MODE_IN;
		}
		return true;
	} else {
		return false;
	}
}

#pragma GCC pop_options



////////////////////////////////////////////////////////////////////////////////////
//                     MENU Reset
////////////////////////////////////////////////////////////////////////////////////    


void reset() {
   
 //multicore_lockout_start_blocking();	

  while (gpio_get(PSEN_PIN)==0) {
	SET_DATA_MODE_OUT;
	gpio_put_masked(DATA_PIN_MASK,0x84<<D0_PIN);
  }
  SET_DATA_MODE_IN;
  while (gpio_get(PSEN_PIN)==0) {
  	SET_DATA_MODE_OUT;
  	gpio_put_masked(DATA_PIN_MASK,0x00<<D0_PIN);
  }
  SET_DATA_MODE_IN;
 
  sleep_ms(5);
 
  while (gpio_get(PSEN_PIN)==0) {
  	SET_DATA_MODE_OUT;
  	gpio_put_masked(DATA_PIN_MASK,0x84<<D0_PIN);
  }
  SET_DATA_MODE_IN;
  while (gpio_get(PSEN_PIN)==0) {
  	SET_DATA_MODE_OUT;
  	gpio_put_masked(DATA_PIN_MASK,0x00<<D0_PIN);
  }
  SET_DATA_MODE_IN;
 
  while (gpio_get(PSEN_PIN)==0) {
  	SET_DATA_MODE_OUT;
  	gpio_put_masked(DATA_PIN_MASK,0x84<<D0_PIN);
  }
  SET_DATA_MODE_IN;
  while (gpio_get(PSEN_PIN)==0) {
  	SET_DATA_MODE_OUT;
  	gpio_put_masked(DATA_PIN_MASK,0x00<<D0_PIN);
  }
  SET_DATA_MODE_IN;
}     

////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	char isDir;
	// char filename[13];
	char long_filename[32];
	char full_path[5];
} DIR_ENTRY;	// 256 bytes = 256 entries in 64k

DIR_ENTRY files[maxfiles];

int num_dir_entries = 0; // how many entries in the current directory

int entry_compare(const void* p1, const void* p2)
{
	DIR_ENTRY* e1 = (DIR_ENTRY*)p1;
	DIR_ENTRY* e2 = (DIR_ENTRY*)p2;
	if (e1->isDir && !e2->isDir) return -1;
	else if (!e1->isDir && e2->isDir) return 1;
	else return strcasecmp(e1->long_filename, e2->long_filename);
}

char *get_filename_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

int is_valid_file(char *filename) {
	char *ext = get_filename_ext(filename);
	if (strcasecmp(ext, "BIN") == 0 || strcasecmp(ext, "ROM") == 0  || strcasecmp(ext, "CV") == 0)
		return 1;
	return 0;
}

FILINFO fno;
char search_fname[FF_LFN_BUF + 1];

// polyfill :-)
char *stristr(const char *str, const char *strSearch) {
    char *sors, *subs, *res = NULL;
    if ((sors = strdup (str)) != NULL) {
        if ((subs = strdup (strSearch)) != NULL) {
            res = strstr (strlwr (sors), strlwr (subs));
            if (res != NULL)
                res = (char*)str + (res - sors);
            free (subs);
        }
        free (sors);
    }
    return res;
}

int scan_files(char *path, char *search)
{
    FRESULT res;
    DIR dir;
    UINT i;


    if (!fatfs_is_mounted())
       mount_fatfs_disk();

	FATFS FatFs;
	if (f_mount(&FatFs, "", 1) != FR_OK) {
		return -1;
	}	

	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		for (;;) {
			if (num_dir_entries == 99) break;
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) break;
			if (fno.fattrib & (AM_HID | AM_SYS)) continue;
			if (fno.fattrib & AM_DIR) {
				i = strlen(path);
				strcat(path, "/");
				if (fno.altname[0])	// no altname when lfn is 8.3
					strcat(path, fno.altname);
				else
					strcat(path, fno.fname);
				printf("%s\n", path);
				if (strlen(path) >= 210) continue;	// no more room for path in DIR_ENTRY
				res = scan_files(path, search);
				if (res != FR_OK) break;
				path[i] = 0;
			}
			else if (is_valid_file(fno.fname))
			{
				char *match = stristr(fno.fname, search);
				if (match) {
					DIR_ENTRY *dst = (DIR_ENTRY *)&files[0];
					dst += num_dir_entries;
					// fill out a record
					dst->isDir = (match == fno.fname) ? 1 : 0;	// use this for a "score"
					strncpy(dst->long_filename, fno.fname, 31);
					dst->long_filename[31] = 0;
					// 8.3 name
					/*if (fno.altname[0])
						strcpy(dst->filename, fno.altname);
					else {	// no altname when lfn is 8.3
						strncpy(dst->filename, fno.fname, 12);
						dst->filename[12] = 0;
					}*/
					// full path for search results
					strcpy(dst->full_path, path);
					printf("%s\n",dst->full_path);
					num_dir_entries++;
				}
			}
		}
		f_closedir(&dir);
	}
	return res;
}

int tree(char *path)
{
    FRESULT res;
    DIR dir;
    UINT i;
	char localpath[5];
	char localfile[37];
	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		for (;;) {
			if (num_dir_entries == maxfiles) break;
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) return FR_OK; //break;
			if (fno.fattrib & (AM_HID | AM_SYS)) continue;
			strcpy(localpath, "");
			strcpy(localpath, path);
			if (strlen(localpath) > 1)
				strcat(localpath, "/");
			strcpy(localfile, "");
			strcat(localfile, localpath);
			strcat(localfile, fno.fname);
			if (fno.fattrib & AM_DIR) {	
				/*if (fno.altname[0])	// no altname when lfn is 8.3
					strcat(localpath, fno.altname);
				else
					strcat(localpath, fno.fname);*/
				// if (strlen(localpath) >= 210) continue;	// no more room for path in DIR_ENTRY
				strcat(localpath, fno.fname);
				res = tree(localpath);
				if (res != FR_OK) break;
			}
			else if (is_valid_file(localfile))
			{
				//DIR_ENTRY *dst = (DIR_ENTRY *)&files[0];
				DIR_ENTRY *dst = &files[num_dir_entries];
				//dst += num_dir_entries;
				// fill out a record
				strncpy(dst->long_filename, fno.fname, 31);
				dst->long_filename[31] = 0;
				// 8.3 name
				/*if (fno.altname[0])
					strcpy(dst->filename, fno.altname);
				else {	// no altname when lfn is 8.3
					strncpy(dst->filename, fno.fname, 12);
					dst->filename[12] = 0;
				}*/
				// full path for search results			
				strcpy(dst->full_path, path);
				printf("%s/%s\n",dst->full_path, dst->long_filename);
				num_dir_entries++;				
			}
		}
		f_closedir(&dir);
	}
	return res;
}

int search_directory(char *path, char *search) {
	char pathBuf[256];
	strcpy(pathBuf, path);
	num_dir_entries = 0;
	int i;
	FATFS FatFs;
	if (f_mount(&FatFs, "", 1) == FR_OK) {
		if (scan_files(pathBuf, search) == FR_OK) {
			// sort by score, name
			qsort((DIR_ENTRY *)&files[0], num_dir_entries, sizeof(DIR_ENTRY), entry_compare);
			DIR_ENTRY *dst = (DIR_ENTRY *)&files[0];
			// re-set the pointer back to 0
			for (i=0; i<num_dir_entries; i++)
				dst[i].isDir = 0;
			return 1;
		}
	}
	strcpy(errorBuf, "Problem searching flash");
	return 0;
}

int read_directory(char *path) {
	int ret = 0;
	num_dir_entries = 0;
	DIR_ENTRY *dst = (DIR_ENTRY *)&files[0];
	char localpath[35];
	strcpy(localpath, path);

	if (strcmp(path, "..") == 0) {
		strcpy(localpath, "/");
	} else 	if (strcmp(path, "/") != 0) {
		dst->isDir = 1;
		strcpy(dst->full_path, "..");
		strcpy(dst->long_filename, "..");
		dst++;
		num_dir_entries++;
	}  	

    if (!fatfs_is_mounted())
       mount_fatfs_disk();

	FATFS FatFs;
	if (f_mount(&FatFs, "", 1) == FR_OK) {
		DIR dir;
		if (f_opendir(&dir, localpath) == FR_OK) {
			while (num_dir_entries < maxfiles) {
				if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0)
					break;
				if (fno.fattrib & (AM_HID | AM_SYS))
					continue;
				dst->isDir = fno.fattrib & AM_DIR ? 1 : 0;
				if (!dst->isDir) 
					if (!is_valid_file(fno.fname)) continue;
				// copy file record to first ram block
				// long file name
				strncpy(dst->long_filename, fno.fname, 31);
				dst->long_filename[31] = 0;
				// 8.3 name
				/*if (fno.altname[0])
		            strcpy(dst->filename, fno.altname);
				else {	// no altname when lfn is 8.3
					strncpy(dst->filename, fno.fname, 12);
					dst->filename[12] = 0;
				}*/
				strcpy(dst->full_path, localpath); //[0] = 0; // path only for search results
				//printf("-%s  %s\n",dst->full_path,dst->long_filename);
	            dst++;
				num_dir_entries++;
			}
			f_closedir(&dir);
		}
		else
			strcpy(errorBuf, "Can't read directory");
		f_mount(0, "", 1);
		// clean list
		for(int i = num_dir_entries; i < maxfiles; i++) {
			if (dst->long_filename[0] == 0) {
				break;
			} else {
				dst->isDir = 0;
				dst->full_path[0] = 0;
				dst->long_filename[0] = 0;
			}
			dst++;
		}
		qsort((DIR_ENTRY *)&files[0], num_dir_entries, sizeof(DIR_ENTRY), entry_compare);
		ret = 1;
	}
	else
		strcpy(errorBuf, "Can't read flash memory");
	return ret;
}

int filesize(char *filename) {
    if (!fatfs_is_mounted())
       mount_fatfs_disk();

	FATFS FatFs;
	int car_file = 0;
	UINT br = 0;
	if (f_mount(&FatFs, "", 1) != FR_OK) {
		error(1);
		return 0;
	}

	FIL fil;
	if (f_open(&fil, filename, FA_READ) != FR_OK) {
		sleep_ms(100);
		error(2);
		goto cleanup;
	}
	int byteslong = f_size(&fil);
	romsize=byteslong;

closefile:
	f_close(&fil);
cleanup:
	f_mount(0, "", 1);

	return byteslong;
}


int load_newfile(DIR_ENTRY *entry, char updatemenu, int local_rom_buffer) {
	FATFS FatFs;
	UINT br = 0;
	UINT bw = 0;
    int l,nb;

	char fullpath[35];

	strcpy(fullpath, "/");
	if (strcmp(entry->full_path, "") != 0) {
		strcat(fullpath, entry->full_path);
		strcat(fullpath, "/");
	}
	strcat(fullpath, entry->long_filename);

	memset(rom_table[local_rom_buffer],0,1024*8*4);
	
	l=filesize(fullpath); //fullpath
	
	if (f_mount(&FatFs, "", 1) != FR_OK) {
		error(1);
	}

	FIL fil;
	if (f_open(&fil, fullpath, FA_READ) != FR_OK) { //fullpath
		error(4);
	}
    
	nb = l/2048;   // nb = number of banks, l=file size)
		
    if ((strcmp(entry->long_filename,"vp_40.bin")==0)||((strcmp(entry->long_filename,"vp_31.bin")==0))||((strcmp(entry->long_filename,"4inarow.bin")==0))) {  // 3k games
	        new_bank_type=2;
			if (f_read(&fil, &extROM[0], 1024, &br) != FR_OK) {
              // error(5);
            }
            if (f_read(&fil, &rom_table[local_rom_buffer][0][1024], 3072, &br) != FR_OK) {
               // error(7);
            } 	
	} else

	 {
		new_bank_type=0;
		if (nb==4) new_bank_type=1;
		if (nb>4) new_bank_type=3;

		for (int i = nb - 1; i >= 0; i--) {
        	if (f_read(&fil,&rom_table[local_rom_buffer][i][1024], 2048, &br)!= FR_OK) {
				error(7);
			}
			if (updatemenu != 0) {
				// update rom with files on SD
				updaterom(&rom_table[local_rom_buffer][i][1024], i);
			}
			#ifdef debugging
			#ifndef optimize
				debugFS(&rom_table[local_rom_buffer][i][1024]);
			#endif
			#endif

        	memcpy(&rom_table[local_rom_buffer][i][3072], &rom_table[local_rom_buffer][i][2048], 1024); /* simulate missing A10 */
    	}
	}
	    // mirror ROM in higher banks
    if (nb<2) memcpy(&rom_table[local_rom_buffer][1],&rom_table[local_rom_buffer][0],4096);
    if (nb<4) memcpy(&rom_table[local_rom_buffer][2],&rom_table[local_rom_buffer][0],8192);
   	
	
closefile:
	f_close(&fil);
	    
cleanup:
	f_mount(0, "", 1);

	return br;
}


////////////////////////////////////////////////////////////////////////////////////
//                     PicoPAC Cart Main
////////////////////////////////////////////////////////////////////////////////////

void picopac_cart_main()
{
    uint32_t pins;
    uint32_t addr;
    uint32_t dataOut=0;
    uint16_t dataWrite=0;
	int ret=0;

    int l, nb;
    DIR_ENTRY localentry[1];

    gpio_init_mask(ALL_GPIO_MASK);
    
	#ifdef debugging
	stdio_init_all();   // for serial output, via printf()
	printf("Start\n");
	#endif
	// overclocking isn't necessary for most functions - but XEGS carts weren't working without it
	// I guess we might as well have it on all the time.
    #ifndef debugging
	//printf("clock : 27000\n");
	set_sys_clock_khz(270000, true);
	//set_sys_clock_khz(170000, true);
	#endif
  //sleep_ms(400);

  multicore_launch_core1(core1_main);
  //sleep_ms(200);
  	menu_loaded = 0;
  	rom_buffer = 0;
	memcpy(&rom_table[rom_buffer][0][1024], &selectgame[6144], 2048 );
	memcpy(&rom_table[rom_buffer][0][3072], &rom_table[rom_buffer][0][2048], 1024);
	memcpy(&rom_table[rom_buffer][1][1024], &selectgame[4096], 2048 );
	memcpy(&rom_table[rom_buffer][1][3072], &rom_table[rom_buffer][1][2048], 1024);
	memcpy(&rom_table[rom_buffer][2][1024], &selectgame[2048], 2048);
	memcpy(&rom_table[rom_buffer][2][3072], &rom_table[rom_buffer][2][2048], 1024);
  	memcpy(&rom_table[rom_buffer][3][1024], &selectgame[0], 2048);
	memcpy(&rom_table[rom_buffer][3][3072], &rom_table[rom_buffer][3][2048], 1024);
	#ifdef optimize
	uint64_t bt = to_us_since_boot(get_absolute_time());
	printf("Boot time : %" PRIu64 " - Pre rom loaded\n", bt);
	#endif


	while (1) {
		if (!menu_loaded) {
			if (rom_buffer == 0)
				next_rom_buffer = 1;
			else
				next_rom_buffer = 0;

			// get files on SD
			read_directory("/");

			localentry->isDir=0;
			strcpy(localentry->full_path, "/");
			strcpy(localentry->long_filename, "selectgame.bin");
			#ifdef debugging
				printf("load_newfile(%s, 1, %i) - rom_buffer : %i\n", localentry->long_filename, next_rom_buffer, next_rom_buffer);
			#endif
			#ifdef optimize
			msSinceBoot1 = to_ms_since_boot(get_absolute_time());
			#endif
			load_newfile(localentry, 1, next_rom_buffer);	// Load selectgame in first rom buffer
			rom_buffer = next_rom_buffer;
			menu_loaded=1;

			#ifdef optimize
			msSinceBoot2 = to_ms_since_boot(get_absolute_time());
			printf("load time (ms)%d\n", msSinceBoot2-msSinceBoot1);
			#endif

		#ifdef debugging
			gamechoosen = 2;
			printf("Game selected\n");
		#endif

			
		memset(extram,0,0xff);
	}
	
	// Initial conditions 

	    
    if (gamechoosen>=1) {

	// sleep_ms(1400);

	if (!files[gamechoosen-1].isDir) {

		if (rom_buffer == 0)
			next_rom_buffer = 1;
		else
			next_rom_buffer = 0;

		#ifdef debugging
		printf("load_newfile(%s, 0, %i) - rom_buffer : %i\n", files[gamechoosen-1].long_filename, next_rom_buffer, rom_buffer);
		#endif
		load_newfile(&files[gamechoosen-1], 0, next_rom_buffer);
		resetnow = 1; // Since now we wait for JMP 0
		gamechoosen = 0;
	} else {
		#ifdef debugging
		printf("read_directory(%s)\n", files[gamechoosen-1].long_filename);
		#endif
		read_directory(files[gamechoosen-1].long_filename);

		localentry->isDir=0;
		strcpy(localentry->full_path, "/");
		strcpy(localentry->long_filename, "selectgame.bin");

		if (rom_buffer == 0)
			next_rom_buffer = 1;
		else
			next_rom_buffer = 0;

		#ifdef debugging
		printf("load_newfile(%s, 1, %i) - rom_buffer : %i\n", localentry->long_filename, next_rom_buffer, rom_buffer);
		#endif
		load_newfile(localentry, 1, next_rom_buffer);
		
		#ifndef debugging //Simulate game selection in debug mode
		gamechoosen = 0;
		#endif
		rom_buffer = next_rom_buffer; // Switch to game buffer
	}
   }
  }
}

 void updaterom(unsigned char *m, int memblock) {
    unsigned int cpt = 0;
	unsigned int pagescnt;
	unsigned int titleoffset;
	
	switch (memblock) {
		case 3:
			titleoffset = 18 * 8; 
			pagescnt = 7;
			break;
		case 2:
			titleoffset = 11 * 8; 
			pagescnt = 7;
			break;
		case 1:
			titleoffset = 4 * 8; 
			pagescnt = 7;
			break;
		case 0:
			titleoffset = 0; 
			pagescnt = 4;
			break;
		default:
			titleoffset = 0;
			pagescnt = 0;
	}
	char title[18];
    for (unsigned int page=0; page<pagescnt; page++) {
        for (unsigned int displine=0; displine<8; displine++) {
            translate(&title[0], files[titleoffset + cpt].long_filename, files[titleoffset + cpt].isDir);
            memcpy((m + (textpages[memblock][page] - (2048 * (3 - memblock))) + 16 * displine), &title[0],16);
			//printf("%s %04x %08x %08x\n", &files[titleoffset + cpt].long_filename, titleoffset + cpt, m, (m + (textpages[memblock][page] - (2048 * (3 - memblock))) + 16 * displine));
			//debugprinttranslated(&title[0]);
            cpt ++;
        }
    }
 }  

#ifdef debugging
void debugFS(unsigned char *array) {
	for(int i=0; i<64; i++) {
		printf("%08x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n\r",
		&array[32*i],
		array[0 + 32*i], array[1 + 32*i], array[2 + 32*i], array[3 + 32*i], array[4 + 32*i], array[5 + 32*i], array[6 + 32*i], array[7 + 32*i], 
		array[8 + 32*i], array[9 + 32*i], array[10 + 32*i], array[11 + 32*i], array[12 + 32*i], array[13 + 32*i], array[14 + 32*i], array[15 + 32*i], 
		array[16 + 32*i], array[17 + 32*i], array[18 + 32*i], array[19 + 32*i], array[20 + 32*i], array[21 + 32*i], array[22 + 32*i], array[23 + 32*i],
		array[24 + 32*i], array[25 + 32*i], array[26 + 32*i], array[27 + 32*i], array[28 + 32*i], array[29 + 32*i], array[30 + 32*i], array[31 + 32*i]);
	}
}

void debugprinttranslated(unsigned char *s) {
	for (int i=0; i<16;i++) {
		printf("%02x", s[i]);
	}
	printf("\n");
}
#endif
