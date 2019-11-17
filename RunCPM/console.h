#ifndef CONSOLE_H
#define CONSOLE_H

/* see main.c for definition */

uint8 _chready(void)		// Checks if there's a character ready for input
{
	return(_kbhit() ? 0xff : 0x00);
}

uint8 _getchNB(void)		// Gets a character, non-blocking, no echo
{
	return(_kbhit() ? _getch() : 0x00);
}

void _putcon(uint8 ch)		// Puts a character
{
	_putch((novaDOSflags & HiOutFlag) ? ch : ch & 0x7F);
}

void _putcon8(uint8 ch)		// Puts an 8 bit character
{
	_putch(ch);
}

void _puts(const char* str)	// Puts a \0 terminated string
{
	while (*str)
		_putcon(*(str++));
}

void _puthex8(uint8 c)		// Puts a HH hex string
{
	_putcon(tohex(c >> 4));
	_putcon(tohex(c & 0x0f));
}

void _puthex16(uint16 w)	// puts a HHHH hex string
{
	_puthex8(w >> 8);
	_puthex8(w & 0x00ff);
}

#endif
