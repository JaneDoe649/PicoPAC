# PicoPAC
Flashcart for Videopac / Odyssey 2 based on Wilco2009 Videopac-microSD card project
Based on SD card by Wilco2009: https://github.com/wilco2009/Videopac-micro-SD-Cart , my greetings for his fantastic job!

Since now, tested on  G-7000, G7400, C52, JOPAC and Odyssey 2. Waiting for a friend that has to repair his G7200 for completing the list ;) 

Added support to extROM games too (VP_40, VP_31 and 4INAROW.BIN).

PicoPAC is a multicart DIY yourself based on cheap "PURPLE" Raspberry Pi Pico clone, easy to build and not expensive.
**WARNING!** "purple" Pico has not the same pinout of original Raspberry "green" ones, you MUST use the clone or you may damage your hardware. 
**Several pinouts exist depending on the clone manufacturer. The pinout corresponding to the PCB is as follows :**
![ScreenShot](https://raw.githubusercontent.com/JaneDoe649/PicoPAC/develop/pictures/PurpleClonePinout.jpg)

**NOTE** filename are "fixed" and hardcoded in firmware to match the "selectgame.bin" menu. You should find all the ROM with matching filenames following this site https://rene-g7400.datatex.nl/ 

[!NOTE]Attempt to allow navigation in the SD card tree (limited to one sub-level) from the menu. For ROMs referenced in MAME, the name of the game is displayed in the menu.
[!WARNING]Development not tested on a console at the moment

Please don't forget to upload the here provided "selectgame.bin" file in flash!!!!

![ScreenShot](https://raw.githubusercontent.com/aotta/PicoPAC/main/pictures/picopac3.jpg)
![ScreenShot](https://raw.githubusercontent.com/aotta/PicoPAC/main/pictures/picopac6.jpg)
![ScreenShot](https://raw.githubusercontent.com/aotta/PicoPAC/main/pictures/picopac1.jpg)
![ScreenShot](https://raw.githubusercontent.com/aotta/PicoPAC/main/pictures/picopac5.jpg)
![ScreenShot](https://raw.githubusercontent.com/aotta/PicoPAC/main/pictures/picopac7.jpg)
![ScreenShot](https://raw.githubusercontent.com/aotta/PicoPAC/main/pictures/picopac8.jpg)


Kicad project and gerbers files for the pcb are in the PCB folder, you need only a diode and a push buttons for resetting the cart if needed or want restart. 
Add you pico clone, and flash the firmware ".uf2" in the Pico by connecting it while pressing button on Pico and drop it in the opened windows on PC.
After flashed with firmware, and every time you have to change your ROMS repository, you can simply connect the Pico to PC and drag&drop "BIN" files  into.
But remember that only games already in menu will be seen by the cart.


Even if the diode should protect your console, **DO NOT CONNECT PICO WHILE INSERTED IN A POWERED ON CONSOLE!**

