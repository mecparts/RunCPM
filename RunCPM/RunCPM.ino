#include "globals.h"

#include <SPI.h>
#include <SdFat.h>  // One SD library to rule them all - Greinman SdFat from Library Manager

// SDCard/LED related definitions
//
//   SdFatSoftSpiEX and SdFatEX require changes to the following lines on SdFatConfig.h:
//     #define ENABLE_EXTENDED_TRANSFER_CLASS (from 0 to 1 - around line 71)
//     #define ENABLE_SOFTWARE_SPI_CLASS (from 0 to 1 - around line 87)
//
#if defined _STM32_DEF_ // STM32 boards
	SdFatSoftSpiEX<PC8, PD2, PC12> SD; // MISO, MOSI, SCK
	#define SDINIT PC11 // CS
	#define LED PD13
	#define LEDinv 0 // 0=normal 1=inverted
	#define BOARD "STM32F407DISC1"
#elif defined ESP32 // ESP32 boards
	SdFatSoftSpiEX<2, 15, 14> SD; // MISO, MOSI, SCK Some boards use 2,15,14,13, other 12,14,27,26
	#define SDINIT 13 // CS
	#define LED 22 // TTGO_T1=22 LOLIN32_Pro=5(inverted) DOIT_Esp32=2 ESP32-PICO-KIT=no led
	#define LEDinv 0
	#define BOARD "TTGO_T1"
#elif defined __IMXRT1062__ // Teensy 4.0
	SdFat SD;
	#define SDINIT 10
	#define LED 2
	#define LEDinv 0
	#define BOARD "Teensy 4.0"
	#define MODEMPORT Serial3
	#define MODEMSPD 38400
	#define MODEMRTS 18
	#define MODEMCTS 19
	#define MODEMDATABITS 8
	#define MODEMSTOPBITS 1
	#define BEEPER 20
#elif defined CORE_TEENSY // Teensy 3.5 and 3.6
	SdFatSdio SD;
	#define SDINIT
	#define LED 13
	#define LEDinv 0
	#define BOARD "TEENSY 3.5"
#elif defined ADAFRUIT_GRAND_CENTRAL_M4
	#define USE_SDIO 0
	SdFat SD;
	#define SDINIT SDCARD_SS_PIN
	#define LED 13
	#define LEDinv 0
	#define BOARD "ADAFRUIT GRAND CENTRAL M4"
#else // Arduino DUE
	SdFatEX SD;
	#define SDINIT 4
	#define LED 13
	#define LEDinv 0
	#define BOARD "ARDUINO DUE"
#endif

// Delays for LED blinking
#define sDELAY 100
#define DELAY 200

// Serial (terminal) port speed
#define TERMINALPORT Serial
#define SERIALSPD 115200


#include "abstraction_arduino.h"

#ifdef ESP32        // ESP32 specific CP/M BDOS call routines
	#include "esp32.h"
#endif
#ifdef _STM32_DEF_  // STM32 specific CP/M BDOS call routines
	#include "stm32.h"
#endif

// LST: device configuration
#ifdef USE_LST
	File lst_dev;
	int lst_open = FALSE;
	uint8 lst_chrCount = 0;
#endif

#include "ram.h"
#include "console.h"
#include "cpu.h"
#include "disk.h"
#include "host.h"
#include "cpm.h"
#ifdef CCP_INTERNAL
#include "ccp.h"
#endif

void setup(void) {
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
	TERMINALPORT.begin(SERIALSPD);
	while (!TERMINALPORT) {	// Wait until serial is connected
		digitalWrite(LED, HIGH^LEDinv);
		delay(sDELAY);
		digitalWrite(LED, LOW^LEDinv);
		delay(sDELAY);
	}
#if defined __IMXRT1062__ 
	MODEMPORT.begin(MODEMSPD);
	if (!MODEMPORT.attachRts(MODEMRTS)) {
		TERMINALPORT.println("Could not attach RTS.");
		return;
	}
	if (!MODEMPORT.attachCts(MODEMCTS)) {
		TERMINALPORT.println("Could not attach CTS.");
		return;
	}
#endif
		
#ifdef DEBUGLOG
	_sys_deletefile((uint8 *)LogName);
#endif

	for( unsigned a=0; a<MEMSIZE; ++a ) {
		_RamWrite(a, 0);
	}
	_clrscr();
	_putch('\a');
	_puts(STR(CPMSIZE));
	_puts("K ZCPR");
	_putch(Z3REV / 10 + '0');
	_putch('.');
	_putch(Z3REV % 10 + '0');
	_puts("/NovaDOS Z80 Operating System\r\n");
	_puts("RunCPM BIOS Version ");
	_puts(VERSION);
	_puts("\r\n");
	_puts("RunCPM by Marcelo Dantas\r\n");
	_puts("Arduino read/write support by Krzysztof Klis\r\n\n");
	_puts("(NovaDOS Clock Interrupt Enabled)\r\n");
	_puts("(CCP: " CCPname " @ 0x");
	_puthex16(CCPaddr);
	_puts(")\r\n");
	_puts("(Board: ");
	_puts(BOARD);
	_puts(")\r\n");

#if defined ADAFRUIT_GRAND_CENTRAL_M4
	if (SD.cardBegin(SDINIT, SD_SCK_MHZ(50))) {

		if (!SD.fsBegin()) {
			_puts("\nFile System initialization failed.\n");
			return;
		}
#elif defined __IMXRT1062__ 
if (SD.begin(SDINIT, SD_SCK_MHZ(25))) {
#else
	if (SD.begin(SDINIT)) {
#endif
		SdFile::dateTimeCallback(_fatDateTime);
		if (VersionCCP >= 0x10 || SD.exists(CCPname)) {
			while (true) {
				//_puts(CCPHEAD);
				_PatchCPM();
				Status = RUNNING;
#ifndef CCP_INTERNAL
				if (!_RamLoad((char *)CCPname, CCPaddr)) {
					_puts("Unable to load the CCP.\r\nCPU halted.\r\n");
					break;
				}
				Z80reset();
				SET_LOW_REGISTER(BC, _RamRead(0x0004));
				PC = CCPaddr;
				Z80run();
#else
				_ccp();
#endif
				if (Status == CBOOT) {
					break;
				}
#ifdef USE_LST
				if (lst_dev) {
					_sys_fflush(lst_dev);
				}
#endif
			}
		} else {
			_puts("Unable to load CP/M CCP.\r\nCPU halted.\r\n");
		}
	} else {
		_puts("Unable to initialize SD card.\r\nCPU halted.\r\n");
	}
}

void loop(void) {
	digitalWrite(LED, HIGH^LEDinv);
	delay(DELAY);
	digitalWrite(LED, LOW^LEDinv);
	delay(DELAY);
	digitalWrite(LED, HIGH^LEDinv);
	delay(DELAY);
	digitalWrite(LED, LOW^LEDinv);
	delay(DELAY * 4);
}

