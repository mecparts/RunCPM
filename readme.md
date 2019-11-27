# RunCPM - Z80 CP/M 2.2 emulator

This branch is targeted for the Teensy 4.0 platform. It uses ZCPR3.3 and extensions to the BDOS to emulate NovaDOS.

It also implements the GSX (Graphics System Extension) BDOS call, but without the overhead of having GSX.SYS and a device driver loaded in the TPA. Instead, everything is handled in RunCPM's BDOS code. Think of it like having a graphics screen that natively understands all of the GSX calls. Currently I have drivers written for Adafruit's 160x128 TFT LCD display (using the ST7735) and generic 320x240 TFT LCD displays using the ILI9341. Though currently I only compile one driver at a time into RunCPM, there's no reason that both drivers couldn't be present, and selected via different driver IDs passed to the Open Workstation call.

