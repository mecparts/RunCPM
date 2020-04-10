# RunCPM - Z80 CP/M 2.2 emulator

This branch is targeted for the Teensy 4.0 platform. It uses ZCPR3.3 and
extensions to the BDOS to emulate ZSDOS, including public file support,
a system clock and file timestamps. ZSDOS utilities such as ZCAL and
ZTIME work out of the box (though ZCAL thinks it's the 20th century
rather than the 21st). A subset of the ZSDOS BDOS is written in Z80
assembler (a CIOS: Console I/O System) so that ZEX will work as well.

It also implements the GSX (Graphics System Extension) BDOS call, but 
without the overhead of having GSX.SYS and a device driver loaded in the
TPA. Instead, everything is handled in RunCPM's BDOS code. Think of it
like having a graphics screen that natively understands all of the GSX
calls. Currently I have a driver written for my heavily modified version 
of [PiGFX](https://github.com/mecparts/pigfx), a bare metal Raspberry Pi
implementation of a colour ANSI terminal.

