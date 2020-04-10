#include "globals.h"

#include <SPI.h>
#include <SdFat.h>  // One SD library to rule them all - Greinman SdFat from Library Manager
#include <Wire.h>

// SDCard/LED related definitions
//
//	SdFatSoftSpiEX and SdFatEX require changes to the following lines on SdFatConfig.h:
//		#define ENABLE_EXTENDED_TRANSFER_CLASS (from 0 to 1 - around line 71)
//		#define ENABLE_SOFTWARE_SPI_CLASS (from 0 to 1 - around line 87)
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
	#define LED 3
	#define LEDinv 0
	#define BOARD "Teensy 4.0"

	#define MODEMPORT Serial3
	#define MODEMSPD 1200
	#define MODEMRTS 18
	#define MODEMCTS 19
	#define MODEMDATABITS 8
	#define MODEMSTOPBITS 1

	uint32 modemSpeed = MODEMSPD;

   #define LPT_PORT		Wire1
   #define LPT_ADDR		0x20		// MCP23017 I2C address

	// MCP23017 registers
   #define LPT_IODIRA	0x00
   #define LPT_IODIRB	0x01
   #define LPT_IOCON		0x0A
   #define LPT_GPPUB		0x0D		// pull ups for control port
   #define LPT_GPIOA		0x12		// LPT data port
   #define LPT_GPIOB		0x13		// LPT status/control port

   #define LPT_CONFIG	0x20		// SEQOP
   #define LPT_BUSY		0x01		// /BUSY on GPIOB.1
   #define LPT_STROBE	0x02		// /STROBE out on GPIOB.2
   
	#define BEEPER 20

	#define USE_GSX
	#define PiGFX
	//#define TFT
	//#define ST7735
	//#define ILI9341

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
#define TERMINALPORT Serial1
#define SERIALSPD 115200
#define STARTUP_DELAY   3000

#include "abstraction_arduino.h"

#ifdef ESP32			// ESP32 specific CP/M BDOS call routines
	#include "esp32.h"
#endif
#ifdef _STM32_DEF_	// STM32 specific CP/M BDOS call routines
	#include "stm32.h"
#endif

#include "ram.h"
#include "console.h"
#include "cpu.h"
#include "disk.h"
#include "host.h"
#ifdef USE_GSX
#include "gsx.h"
#endif
#include "cpm.h"
#ifdef CCP_INTERNAL
#include "ccp.h"
#endif

void setup(void) {
   int32 startup_delay = STARTUP_DELAY;
   
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);

   while (startup_delay > 0) {
		digitalWrite(LED, HIGH^LEDinv);
		delay(sDELAY);
      startup_delay -= sDELAY;
		digitalWrite(LED, LOW^LEDinv);
		delay(sDELAY);
      startup_delay -= sDELAY;
	}
   
	TERMINALPORT.begin(SERIALSPD);
	while (!TERMINALPORT) {	// Wait until serial is connected
		digitalWrite(LED, HIGH^LEDinv);
		delay(sDELAY);
		digitalWrite(LED, LOW^LEDinv);
		delay(sDELAY);
	}
#if defined MODEMPORT
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

#ifdef LPT_PORT
	LPT_PORT.begin();
	_writeI2Cregister(LPT_IOCON, LPT_CONFIG);	// IOCON.SEQOP
	_writeI2Cregister(LPT_GPPUB, LPT_BUSY);	// enable /BUSY pull up resistor
	_writeI2Cregister(LPT_IODIRA, 0x00);		// data port all outputs
	_writeI2Cregister(LPT_GPIOB, LPT_STROBE);	// make sure /STROBE starts inactive
	_writeI2Cregister(LPT_IODIRB, LPT_BUSY);	// /BUSY is an input; all others out
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
	_puts("/ZSDOS 1.1\r\n");
	_puts("RunCPM BIOS Version ");
	_puts(VERSION);
	_puts("\r\n");
	_puts("RunCPM by Marcelo Dantas\r\n");
	_puts("Arduino read/write support by Krzysztof Klis\r\n\n");
	_puts("(ZSDOS Clock Loaded)\r\n");
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
#else
	if (SD.begin(SDINIT)) {
#endif
#if defined USE_GSX
		gsx.begin();
#endif

		if (VersionCCP >= 0x10 || SD.exists(CCPname)) {
			while (true) {
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

