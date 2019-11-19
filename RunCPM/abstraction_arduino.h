#ifndef ABSTRACT_H
#define ABSTRACT_H

#include <TimeLib.h>

#ifdef PROFILE
#define printf(a, b) TERMINALPORT.println(b)
#endif

#if defined ARDUINO_SAM_DUE || defined ADAFRUIT_GRAND_CENTRAL_M4
#define HostOS 0x01
#endif
#ifdef CORE_TEENSY
#define HostOS 0x04
#endif
#ifdef ESP32
#define HostOS 0x05
#endif
#ifdef _STM32_DEF_
#define HostOS 0x06
#endif

/* Memory abstraction functions */
/*===============================================================================*/
bool _RamLoad(char* filename, uint16 address) {
	File f;
	bool result = false;

	if (f = SD.open(filename, FILE_READ)) {
		while (f.available())
			_RamWrite(address++, f.read());
		f.close();
		result = true;
	}
	return(result);
}

/* Filesystem (disk) abstraction fuctions */
/*===============================================================================*/
File rootdir, userdir;
#define FOLDERCHAR '/'

typedef struct {
	uint8 dr;
	uint8 fn[8];
	uint8 tp[3];
	uint8 ex, s1, s2, rc;
	uint8 al[16];
	uint8 cr, r0, r1, r2;
} CPM_FCB;

typedef struct {
	uint8 dr;
	uint8 fn[8];
	uint8 tp[3];
	uint8 ex, s1, s2, rc;
	uint8 al[16];
} CPM_DIRENTRY;

static dir_t	fileDirEntry;

File _sys_fopen_w(uint8* filename) {
	return(SD.open((char*)filename, O_CREAT | O_WRITE));
}

int _sys_fputc(uint8 ch, File& f) {
	return(f.write(ch));
}

void _sys_fflush(File& f) {
	f.flush();
}

void _sys_fclose(File& f) {
	f.close();
}

int _sys_select(uint8* disk) {
	uint8 result = FALSE;
	File f;

	digitalWrite(LED, HIGH ^ LEDinv);
	if (f = SD.open((char*)disk, O_READ)) {
		if (f.isDirectory())
			result = TRUE;
		f.close();
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

int _sys_setfileattributes(uint8* filename, uint16 fcbaddr) {
	CPM_FCB *F = (CPM_FCB*)_RamSysAddr(fcbaddr);
	uint8 result = false;
	File f;
	dir_t dirEntry;

	digitalWrite(LED, HIGH ^ LEDinv);
	if (f = SD.open((char*)filename, O_RDONLY)) {
		f.dirEntry(&dirEntry);
		if (F->fn[1] & 0x80) {
			dirEntry.attributes |= DIR_ATT_HIDDEN;
		} else {
			dirEntry.attributes &= ~DIR_ATT_HIDDEN;
		}
		if (F->tp[0] & 0x80) {
			dirEntry.attributes |= DIR_ATT_READ_ONLY;
		} else {
			dirEntry.attributes &= ~DIR_ATT_READ_ONLY;
		}
		if (F->tp[1] & 0x80) {
			dirEntry.attributes |= DIR_ATT_SYSTEM;
		} else {
			dirEntry.attributes &= ~DIR_ATT_SYSTEM;
		}
		if (F->tp[2] & 0x80) {
			dirEntry.attributes |= DIR_ATT_ARCHIVE;
		} else {
			dirEntry.attributes &= ~DIR_ATT_ARCHIVE;
		}
		f.close();
		result = TRUE;
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);

}

long _sys_filesize(uint8* filename) {
	long l = -1;
	File f;

	digitalWrite(LED, HIGH ^ LEDinv);
	if (f = SD.open((char*)filename, O_RDONLY)) {
		l = f.size();
		f.close();
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(l);
}

int _sys_openfile(uint8* filename, CPM_FCB* F) {
	File f;
	int result = 0;

	digitalWrite(LED, HIGH ^ LEDinv);
	f = SD.open((char*)filename, O_READ);
	if (f) {
		f.dirEntry(&fileDirEntry);
		f.close();
		F->s1 = 0x00;
		result = 1;
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

int _sys_openpublicfile(uint8* filename, CPM_FCB* F) {
	File f;
	int result = 0;
	uint8 origUserCode = filename[2];
	
	digitalWrite(LED, HIGH ^ LEDinv);
	for( uint8 i = 0; i < 16; ++i) {
		filename[2] = (i < 10) ? (i + '0') : (i + 'A' - 10);
		f = SD.open((char*)filename, O_READ);
		if (f) {
			f.dirEntry(&fileDirEntry);
			f.close();
			if (fileDirEntry.attributes & DIR_ATT_HIDDEN) {
				F->s1 = filename[2];
				result = 1;
				break;
			}
		}
	}
	filename[2] = origUserCode;
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

int _sys_makefile(uint8* filename) {
	File f;
	int result = 0;

	digitalWrite(LED, HIGH ^ LEDinv);
	f = SD.open((char*)filename, O_CREAT | O_WRITE);
	if (f) {
		f.close();
		result = 1;
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

int _sys_deletefile(uint8* filename) {
	int result;
	digitalWrite(LED, HIGH ^ LEDinv);
	result = SD.remove((char*)filename);
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

int _sys_renamefile(uint8* filename, uint8* newname) {
	File f;
	int result = 0;

	digitalWrite(LED, HIGH ^ LEDinv);
	f = SD.open((char*)filename, O_WRITE | O_APPEND);
	if (f) {
		if (f.rename(SD.vwd(), (char*)newname)) {
			f.close();
			result = 1;
		}
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

#ifdef DEBUGLOG
void _sys_logbuffer(uint8* buffer) {
#ifdef CONSOLELOG
	puts((char*)buffer);
#else
	File f;
	uint8 s = 0;
	while (*(buffer + s))	// Computes buffer size
		++s;
	if (f = SD.open(LogName, O_CREAT | O_APPEND | O_WRITE)) {
		f.write(buffer, s);
		f.flush();
		f.close();
	}
#endif
}
#endif

bool _sys_extendfile(char* fn, unsigned long fpos)
{
	uint8 result = true;
	File f;
	unsigned long i;

	digitalWrite(LED, HIGH ^ LEDinv);
	if (f = SD.open(fn, O_WRITE | O_APPEND)) {
		if (fpos > f.size()) {
			for (i = 0; i < f.size() - fpos; ++i) {
				if (f.write((uint8)0) != 1) {
					result = false;
					break;
				}
			}
		}
		f.close();
	} else {
		result = false;
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

uint8 _sys_readseq(uint8* filename, long fpos) {
	uint8 result = 0xff;
	File f;
	uint8 bytesread;
	uint8 dmabuf[BlkSZ];
	uint8 i;

	digitalWrite(LED, HIGH ^ LEDinv);
	f = SD.open((char*)filename, O_READ);
	if (f) {
		if (f.seek(fpos)) {
			for (i = 0; i < BlkSZ; ++i)
				dmabuf[i] = 0x1a;
			bytesread = f.read(&dmabuf[0], BlkSZ);
			if (bytesread) {
				for (i = 0; i < BlkSZ; ++i)
					_RamWrite(dmaAddr + i, dmabuf[i]);
			}
			result = bytesread ? 0x00 : 0x01;
		} else {
			result = 0x01;
		}
		f.close();
	} else {
		result = 0x10;
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

uint8 _sys_writeseq(uint8* filename, long fpos) {
	uint8 result = 0xff;
	File f;

	digitalWrite(LED, HIGH ^ LEDinv);
	if (_sys_extendfile((char*)filename, fpos))
		f = SD.open((char*)filename, O_RDWR);
	if (f) {
		if (f.seek(fpos)) {
			if (f.write(_RamSysAddr(dmaAddr), BlkSZ))
				result = 0x00;
		} else {
			result = 0x01;
		}
		f.close();
	} else {
		result = 0x10;
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

uint8 _sys_readrand(uint8* filename, long fpos) {
	uint8 result = 0xff;
	File f;
	uint8 bytesread;
	uint8 dmabuf[BlkSZ];
	uint8 i;
	long extSize;

	digitalWrite(LED, HIGH ^ LEDinv);
	f = SD.open((char*)filename, O_READ);
	if (f) {
		if (f.seek(fpos)) {
			for (i = 0; i < BlkSZ; ++i)
				dmabuf[i] = 0x1a;
			bytesread = f.read(&dmabuf[0], BlkSZ);
			if (bytesread) {
				for (i = 0; i < BlkSZ; ++i)
					_RamWrite(dmaAddr + i, dmabuf[i]);
			}
			result = bytesread ? 0x00 : 0x01;
		} else {
			if (fpos >= 65536L * BlkSZ) {
				result = 0x06;	// seek past 8MB (largest file size in CP/M)
			} else {
				extSize = f.size();
				// round file size up to next full logical extent
				extSize = ExtSZ * ((extSize / ExtSZ) + ((extSize % ExtSZ) ? 1 : 0));
				if (fpos < extSize)
					result = 0x01;	// reading unwritten data
				else
					result = 0x04; // seek to unwritten extent
			}
		}
		f.close();
	} else {
		result = 0x10;
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

uint8 _sys_writerand(uint8* filename, long fpos) {
	uint8 result = 0xff;
	File f;

	digitalWrite(LED, HIGH ^ LEDinv);
	if (_sys_extendfile((char*)filename, fpos)) {
		f = SD.open((char*)filename, O_RDWR);
	}
	if (f) {
		if (f.seek(fpos)) {
			if (f.write(_RamSysAddr(dmaAddr), BlkSZ))
				result = 0x00;
		} else {
			result = 0x06;
		}
		f.close();
	} else {
		result = 0x10;
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

static uint8 findNextDirName[13];
static uint16 fileRecords = 0;
static uint16 fileExtents = 0;
static uint16 fileExtentsUsed = 0;
static uint16 firstFreeAllocBlock;

uint8 _findnext(uint8 isdir) {
	File f;
	uint8 result = 0xff;
	bool isfile;
	uint32 bytes;

	digitalWrite(LED, HIGH ^ LEDinv);
	if (allExtents && fileRecords) {
		_mockupDirEntry();
		result = 0;
	} else {
		while (f = userdir.openNextFile()) {
			f.getName((char*)&findNextDirName[0], 13);
			isfile = !f.isDirectory();
			bytes = f.size();
			f.dirEntry(&fileDirEntry);
			f.close();
			if (!isfile)
				continue;
			_HostnameToFCBname(findNextDirName, fcbname);
			if (match(fcbname, pattern)) {
				if (isdir) {
					// account for host files that aren't multiples of the block size
					// by rounding their bytes up to the next multiple of blocks
					if (bytes & (BlkSZ - 1)) {
						bytes = (bytes & ~(BlkSZ - 1)) + BlkSZ;
					}
					fileRecords = bytes / BlkSZ;
					fileExtents = fileRecords / BlkEX + ((fileRecords & (BlkEX - 1)) ? 1 : 0);
					fileExtentsUsed = 0;
					firstFreeAllocBlock = firstBlockAfterDir;
					_mockupDirEntry();
				} else {
					fileRecords = 0;
					fileExtents = 0;
					fileExtentsUsed = 0;
					firstFreeAllocBlock = firstBlockAfterDir;
				}
				_RamWrite(tmpFCB, filename[0] - '@');
				_HostnameToFCB(tmpFCB, findNextDirName);
				result = 0x00;
				break;
			}
		}
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

uint8 _findfirst(uint8 isdir) {
	uint8 path[4] = { '?', FOLDERCHAR, '?', 0 };
	path[0] = filename[0];
	path[2] = filename[2];
	if (userdir)
		userdir.close();
	userdir = SD.open((char*)path); // Set directory search to start from the first position
	_HostnameToFCBname(filename, pattern);
	fileRecords = 0;
	fileExtents = 0;
	fileExtentsUsed = 0;
	return(_findnext(isdir));
}

uint8 _findnextallusers(uint8 isdir) {
	uint8 result = 0xFF;
	char dirname[13];
	bool done = false;

	while (!done) {
		while (!userdir) {
			userdir = rootdir.openNextFile();
			if (!userdir) {
				done = true;
				break;
			}
			userdir.getName(dirname, sizeof dirname);
			if (userdir.isDirectory() && strlen(dirname) == 1 && isxdigit(dirname[0])) {
				currFindUser = dirname[0] <= '9' ? dirname[0] - '0' : toupper(dirname[0]) - 'A' + 10;
				break;
			}
			userdir.close();
		}
		if (userdir) {
			result = _findnext(isdir);
			if (result) {
				userdir.close();
			} else {
				done = true;
			}
		} else {
			result = 0xFF;
			done = true;
		}
	}
	return result;
}

uint8 _findfirstallusers(uint8 isdir) {
	uint8 path[2] = { '?', 0 };

	path[0] = filename[0];
	if (rootdir)
		rootdir.close();
	if (userdir)
		userdir.close();
	rootdir = SD.open((char*)path); // Set directory search to start from the first position
	strcpy((char*)pattern, "???????????");
	if (!rootdir)
		return 0xFF;
	fileRecords = 0;
	fileExtents = 0;
	fileExtentsUsed = 0;
	return(_findnextallusers(isdir));
}

uint8 _Truncate(char* filename, uint8 rc) {
	File f;
	int result = 0;

	digitalWrite(LED, HIGH ^ LEDinv);
	f = SD.open((char*)filename, O_WRITE | O_APPEND);
	if (f) {
		if (f.truncate(rc * BlkSZ)) {
			f.close();
			result = 1;
		}
	}
	digitalWrite(LED, LOW ^ LEDinv);
	return(result);
}

void _MakeUserDir() {
	uint8 dFolder = cDrive + 'A';
	uint8 uFolder = toupper(tohex(userCode));

	uint8 path[4] = { dFolder, FOLDERCHAR, uFolder, 0 };

	digitalWrite(LED, HIGH ^ LEDinv);
	SD.mkdir((char*)path);
	digitalWrite(LED, LOW ^ LEDinv);
}

uint8 _sys_makedisk(uint8 drive) {
	uint8 result = 0;
	if (drive < 1 || drive>16) {
		result = 0xff;
	} else {
		uint8 dFolder = drive + '@';
		uint8 disk[2] = { dFolder, 0 };
		digitalWrite(LED, HIGH ^ LEDinv);
		if (!SD.mkdir((char*)disk)) {
			result = 0xfe;
		} else {
			uint8 path[4] = { dFolder, FOLDERCHAR, '0', 0 };
			SD.mkdir((char*)path);
		}
		digitalWrite(LED, LOW ^ LEDinv);
	}

	return(result);
}

/* Console abstraction functions */
/*===============================================================================*/
static uint8 charCount = 0;

void _putch(uint8 ch) {
	if (ch != '\a') {
		TERMINALPORT.write(ch);
		// Periodically flush the USB serial output. This seems to get rid
		// of the problem where not all text would be displayed before
		// waiting for character into (most noticable with [ More ] prompts
		// at the end of a page. Fingers crossed that it might also get
		// rid of the "previousl displayed text popping up again" problem.
		if (++charCount > 32) {
			TERMINALPORT.flush();
			charCount = 0;
		}
	} else {
		tone(BEEPER, 800, 200);
	}
}

#define KEY_TIMEOUT 5
#define FIFO_LNG 16
static uint8 keyFifo[FIFO_LNG];
static uint8 escNumber;
static int fifoCount = 0,fifoHead = 0, fifoTail = 0;
enum EscStateType { NOT_IN_SEQ, SAW_ESC, SAW_BRACE, SAW_O, SAW_NUMBER, SAW_SEMICOLON, SAW_5 } escState = NOT_IN_SEQ;

// Implement a Finite State Machine to translate VT100/ANSI cursor
// key sequences to WordStar equivalents. This has the major advantage
// of EVERYTHING that understand WS sequences just working with the
// cursor keypad. ZCPR3.3 utilities of all kinds, DazzleStar, Turbo
// Pascal, ZDE, you name it.
//
// This routine works fine on Arduino style platforms, but porting it
// to Linux wasn't straightforward. There seemed to be problems timing
// the interval between characters. As I was primarily interested in
// using this on my Teensy 4, I left the Linux side for another day.
// As I also did any attempt at optimization; the code is ugly, but it
// does what I want.
int _kbhit(void) {
	char c;
	uint32_t timeout = 0;

	if (novaDOSflags & HiInFlag) {
		// turn off cursor key escape processing
		// when inputting 8 bit characters
		// (needed for file transfers through the
		// console AND handy when a terminal program
		// is talking on a hardware serial port to a
		// system that expects an ANSI terminal)
		//
		// (unless of course that terminal program is
		// ZMP and swallows the ESC character sent by
		// the cursor key - curses, foiled again)
		if (TERMINALPORT.available()) {
			c = TERMINALPORT.read();
			keyFifo[fifoTail] = c;
			fifoTail = (fifoTail + 1) % FIFO_LNG;
			++fifoCount;
		}
	} else {
		do {
			if (TERMINALPORT.available()) {
				c = TERMINALPORT.read();
				timeout = millis() + KEY_TIMEOUT;
				keyFifo[fifoTail] = c;
				fifoTail = (fifoTail + 1) % FIFO_LNG;
				++fifoCount;
				switch (escState) {
					case NOT_IN_SEQ:
						if (c == 0x1b) {
							escState = SAW_ESC;
						}
						break;
					case SAW_ESC:
						if (c == '[') {
							escState = SAW_BRACE;
						} else if (c == 'O') {
							escState = SAW_O;
						} else {
							escState = NOT_IN_SEQ;
						}
						break;
					case SAW_BRACE:
						switch (c) {
							case 'A':		// up arrow key
							case 'B':		// down arrow key
							case 'C':		// right arrow key
							case 'D':		// left arrow key
								fifoCount -= 3;
								fifoTail = (fifoTail - 3 + FIFO_LNG) % FIFO_LNG;
								switch (c) {
									case 'A':	// line up
										keyFifo[fifoTail] = 'E' - '@';
										break;
									case 'B':	// line down
										keyFifo[fifoTail] = 'X' - '@';
										break;
									case 'C':	// char right
										keyFifo[fifoTail] = 'D' - '@';
										break;
									case 'D':	// char left
										keyFifo[fifoTail] = 'S' - '@';
										break;
								}
								fifoTail = (fifoTail + 1) % FIFO_LNG;
								++fifoCount;
								escState = NOT_IN_SEQ;
								break;

							case '1':		// HOME key
							case '2':		// INSERT key
							case '3':		// DELETE key
							case '5':		// PAGE UP key
							case '6':		// PAGE DOWN key
								escNumber = c;
								escState = SAW_NUMBER;
								break;
							default:
								escState = NOT_IN_SEQ;
								break;
						}
						break;
					case SAW_O:
						if (c == 'F') {	// <esc> O F - END key
							fifoCount -= 3;
							fifoTail = (fifoTail - 3 + FIFO_LNG) % FIFO_LNG;
							keyFifo[fifoTail] = 'Q' - '@';	
							fifoTail = (fifoTail + 1) % FIFO_LNG;
							++fifoCount;
							keyFifo[fifoTail] = 'D';	// ^QD end of line
							fifoTail = (fifoTail + 1) % FIFO_LNG;
							keyFifo[(fifoTail + 1) % FIFO_LNG] = 'D';
							++fifoCount;
						}
						escState = NOT_IN_SEQ;
						break;
					case SAW_NUMBER:
						if (c == '~') {			// <esc> [ N ~
							switch (escNumber) {
								case '1':	// HOME key
									fifoCount -= 4;
									fifoTail = (fifoTail - 4 + FIFO_LNG) % FIFO_LNG;
									keyFifo[fifoTail] = 'Q' - '@';
									fifoTail = (fifoTail + 1) % FIFO_LNG;
									++fifoCount;
									keyFifo[fifoTail] = 'S';	// ^QS start of line
									fifoTail = (fifoTail + 1) % FIFO_LNG;
									++fifoCount;
									break;
								case '2':	// INSERT key
									fifoCount -= 4;
									fifoTail = (fifoTail - 4 + FIFO_LNG) % FIFO_LNG;
									keyFifo[fifoTail] = 'V' - '@';	// insert on/of
									fifoTail = (fifoTail + 1) % FIFO_LNG;
									++fifoCount;
									break;
								case '3':	// DELETE key
									fifoCount -= 4;
									fifoTail = (fifoTail - 4 + FIFO_LNG) % FIFO_LNG;
									keyFifo[fifoTail] = 'G' - '@';	// del char under cursor
									fifoTail = (fifoTail + 1) % FIFO_LNG;
									++fifoCount;
									break;
								case '5':	// PAGE UP key
									fifoCount -= 4;
									fifoTail = (fifoTail - 4 + FIFO_LNG) % FIFO_LNG;
									keyFifo[fifoTail] = 'R' - '@';	// page up
									fifoTail = (fifoTail + 1) % FIFO_LNG;
									++fifoCount;
									break;
								case '6':	// PAGE DOWN key
									fifoCount -= 4;
									fifoTail = (fifoTail - 4 + FIFO_LNG) % FIFO_LNG;
									keyFifo[fifoTail] = 'C' - '@';	// page down
									fifoTail = (fifoTail + 1) % FIFO_LNG;
									++fifoCount;
									break;
							}
							escState = NOT_IN_SEQ;
						} else if (c == ';' && escNumber == '1') {
							escState = SAW_SEMICOLON;
						} else {
							escState = NOT_IN_SEQ;
						}
						break;
					case SAW_SEMICOLON:
						if (c == '5') {
							escState = SAW_5;
						} else {
							escState = NOT_IN_SEQ;
						}
						break;
					case SAW_5:
						if (c == 'C' || c == 'D') {	// <esc> [ 1 ; 5 C or D
							fifoCount -= 6;
							fifoTail = (fifoTail - 6 + FIFO_LNG) % FIFO_LNG;
							switch (c) {
								case 'C':	// <ctrl>right arrow - word right
									keyFifo[fifoTail] = 'F' - '@';
									break;
								case 'D':	// <ctrlleft arrow - word left
									keyFifo[fifoTail] = 'A' - '@';
									break;
							}
							fifoTail = (fifoTail + 1) % FIFO_LNG;
							++fifoCount;
						}
						escState = NOT_IN_SEQ;
						break;
				}
			} else if (millis() >= timeout) {
				escState = NOT_IN_SEQ;		// no keypress seen in last 5ms, we're done
			}
		} while (escState != NOT_IN_SEQ);
	}
	return fifoCount != 0;
}

// wait for and return a key from the key FIFO
uint8 _getch(void) {
	uint8 c;

	while (!_kbhit()) {
		;
	}
	c = keyFifo[fifoHead];
	fifoHead = (fifoHead + 1) % FIFO_LNG;
	--fifoCount;
	return c;
#if 0
	while (!_kbhit()) {
		yield();
	}
	return(TERMINALPORT.read() & ((novaDOSflags & HiInFlag) ? 0xFF : 0x7F));
#endif
}

// wait for an echo a keypress
uint8 _getche(void) {
	uint8 ch = _getch();
	_putch(ch);
	return(ch);
}

void _clrscr(void) {
	TERMINALPORT.println("\e[H\e[J");
}

// modem support

// return 1 if a character is ready from the modem input, 0 otherwise
uint8 _ttyist() {
	return (uint8)(MODEMPORT.available() ? 1 : 0);
}

// wait for and return a character from the modem
uint8 _getrdr() {
	while (!MODEMPORT.available() );
	return MODEMPORT.read();
}

// return 1 if a character can be sent to the modem output, 0 otherwise
uint8 _ttyost() {
	return (uint8)(digitalRead(MODEMCTS) == LOW ? 1 : 0);
}

// send a character to the modem
void _putpun(uint8 c) {
	MODEMPORT.write(c);
}

// Set up the modem serial port for a new baud rate and possibly data format.
// Largely untested, but does what I want for the moment.
void _ioinit(uint16 iotab) {
	uint32 newBaud = 0;
	uint16 dataFmt = 0xFFFF;
	
	switch (_RamRead(iotab)) {		// baud rate
		case 0:		// 110 baud
			newBaud = 110;
			break;
		case 1:		// 300 baud  
			newBaud = 300;
			break;
		case 2:		// 450 baud
			newBaud = 450;
			break;
		case 3:		// 600 baud
			newBaud = 600;
			break;
		case 4:		// 710 baud
			newBaud = 710;
			break;
		case 5:		// 1200 baud
			newBaud = 1200;
			break;
		case 6:		// 2400 baud
			newBaud = 2400;
			break;
		case 7:		// 4800 baud
			newBaud = 4800;
			break;
		case 8:		// 9600 baud
			newBaud = 9600;
			break;
		case 9:		// 19.2Kbaud
			newBaud = 19200;
			break;
		case 10:		// 38.4Kbaud
			newBaud = 38400;
			break;
		case 11:		// 57.6Kbaud
			newBaud = 57600;
			break;
		case 12:		// 76.8Kbaud
			newBaud = 76800;
			break;
		default:
			break;
	}
	switch (_RamRead(iotab+1)) {					// data bits
		case 7:
			switch (_RamRead(iotab+3)) {			// stop bits
				case 1:
					switch (_RamRead(iotab+2)) {	// parity
						case 'E':
							dataFmt = SERIAL_7E1;
							break;
						case 'O':
							dataFmt = SERIAL_7O1;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			break;
		case 8:
			switch (_RamRead(iotab+3)) {			// stop bits
				case 1:
					switch (_RamRead(iotab+2)) {	// parity
						case 'N':
							dataFmt = SERIAL_8N1;
							break;
						case 'E':
							dataFmt = SERIAL_8E1;
							break;
						case 'O':
							dataFmt = SERIAL_8O1;
							break;
						default:
							break;
					}
					break;
				case 2:
					switch (_RamRead(iotab+2)) {	// parity
						case 'N':
							dataFmt = SERIAL_8N2;
							break;
						case 'E':
							dataFmt = SERIAL_8E2;
							break;
						case 'O':
							dataFmt = SERIAL_8O2;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	if (newBaud) {
		// code specific to changing the ESP8266 virtual modem baud rate
		MODEMPORT.print("AT");
		MODEMPORT.println(newBaud);
		delay(150);	// wait for ESP8266 to change baud rate

		// update the Teensy's serial port
		if (dataFmt != 0xFFFF) {
			MODEMPORT.begin(newBaud,dataFmt);
		} else {
			MODEMPORT.begin(newBaud);
		}
	}
}

// delay # of ms
void _delay(uint16 ms) {
	delay(ms);
}

// return pointer to internal BIOS millisecond timer block
// with current milliseconds since power up value.
// NB: the value does not increment automagically. You have
// to call _millis() to get the current value.
uint16 _millis() {
	uint32 ms = millis();
	_RamWrite(millisTS,		 ms & 0xFF);
	_RamWrite(millisTS + 1, (ms >> 8) & 0xFF);
	_RamWrite(millisTS + 2, (ms >> 16) & 0xFF);
	_RamWrite(millisTS + 3, (ms >> 24) & 0xFF);
	return millisTS;
}
//
// TimeLib day 0 is Jan 1, 1970 00:00:00
// FAT file system day 0 is Jan 1, 1980 00:00:00
// Z80DOS day 0 is Dec 31, 1977 (or, day 1 is Jan 1,1978)
//
// The upshot is when we retreive a TimeLib time, we have to subtract
// the number of seconds between Jan 1, 1970 and Dec 31, 1977 - a peroid
// of 2921 days - before we can use it in a Z80DOS time block.
//
// Conversely, when we're converting from a Z80DOS time block to TimeLib
// we add 2921 days worth of seconds.
// 
#define DAYS_OFFSET_CPM_TO_UNIX 2921

#define bin2bcd(i) ((i / 10) * 16 + (i % 10))
#define bcd2bin(i) ((i / 16) * 10 + (i % 16))

void _fatDateTime(uint16_t* date, uint16_t* time) {
	time_t tmStamp;
	
	if (useFileStamp) {
		uint16 fileTimePtr;
		switch (bdosFunc) {
			case 22:							// make new file
				fileTimePtr = fileTS;	// use file's create timestamp
				break;
			case 16:							//	close file
			case 21:							// write sequential
			case 34:							// random write
			case 40:							// random write zero fill
				fileTimePtr = fileTS+4;	// use files' modify timestamp
				break;
			default:
				return;						// do nothing for other BDOS calls
		}
		useFileStamp = false;
		tmStamp = (_RamRead16(fileTimePtr) + DAYS_OFFSET_CPM_TO_UNIX) * SECS_PER_DAY;
		uint8 i;
		i = _RamRead(fileTimePtr + 2);
		tmStamp += 3600UL * bcd2bin(i);
		i = _RamRead(fileTimePtr + 3);
		tmStamp += 60UL * bcd2bin(i);
	} else {
		// User gets date and time from real-time
		// clock in callback function
		tmStamp = Teensy3Clock.get();
	}
		
	// return date using FAT_DATE macro to format fields
	*date = FAT_DATE(year(tmStamp), month(tmStamp), day(tmStamp));

	// return time using FAT_TIME macro to format fields
	*time = FAT_TIME(hour(tmStamp), minute(tmStamp), second(tmStamp));
}

uint16 _getFileTimeStamp() {
	tmElements_t te;
	time_t tt;
	
	te.Year = CalendarYrToTm(FAT_YEAR(fileDirEntry.creationDate));
	te.Month = FAT_MONTH(fileDirEntry.creationDate);
	te.Day = FAT_DAY(fileDirEntry.creationDate);
	te.Hour = FAT_HOUR(fileDirEntry.creationTime);
	te.Minute = FAT_MINUTE(fileDirEntry.creationTime);
	te.Second = 0;
	tt = makeTime(te);
	
	uint16 days = tt / SECS_PER_DAY - DAYS_OFFSET_CPM_TO_UNIX;
	_RamWrite16(fileTS+0, days);
	uint8 i;
	i = hour(tt);
	_RamWrite(fileTS+2, bin2bcd(i));
	i = minute(tt);
	_RamWrite(fileTS+3, bin2bcd(i));

	te.Year = CalendarYrToTm(FAT_YEAR(fileDirEntry.lastWriteDate));
	te.Month = FAT_MONTH(fileDirEntry.lastWriteDate);
	te.Day = FAT_DAY(fileDirEntry.lastWriteDate);
	te.Hour = FAT_HOUR(fileDirEntry.lastWriteTime);
	te.Minute = FAT_MINUTE(fileDirEntry.lastWriteTime);
	te.Second = 0;
	tt = makeTime(te);
	
	days = tt / SECS_PER_DAY - DAYS_OFFSET_CPM_TO_UNIX;
	_RamWrite16(fileTS+4, days);
	i = hour(tt);
	_RamWrite(fileTS+6, bin2bcd(i));
	i = minute(tt);
	_RamWrite(fileTS+7, bin2bcd(i));

	return fileTS;
}

void _getTime(uint16 timeBlk) {
	time_t rightNow = Teensy3Clock.get();
	uint16 days = rightNow / SECS_PER_DAY - DAYS_OFFSET_CPM_TO_UNIX;
	_RamWrite16(timeBlk, days);
	timeBlk += 2;
	uint8 i;
	i = hour(rightNow);
	_RamWrite(timeBlk++, bin2bcd(i));
	i = minute(rightNow);
	_RamWrite(timeBlk++, bin2bcd(i));
	i = second(rightNow);
	_RamWrite(timeBlk++, bin2bcd(i));
}

void _setTime(uint16 timeBlk) {
	time_t newTime = (_RamRead16(timeBlk) + DAYS_OFFSET_CPM_TO_UNIX) * SECS_PER_DAY;
	uint8 i;
	i = _RamRead(timeBlk + 2);
	newTime += 3600UL * bcd2bin(i);
	i = _RamRead(timeBlk + 3);
	newTime += 60UL * bcd2bin(i);
	i = _RamRead(timeBlk + 4);
	newTime += bcd2bin(i);
	Teensy3Clock.set(newTime); // set the RTC
}

#endif
