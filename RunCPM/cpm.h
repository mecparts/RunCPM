#ifndef CPM_H
#define CPM_H

/* see main.c for definition */

#define JP		0xc3
#define CALL	0xcd
#define RET		0xc9
#define INa		0xdb	// Triggers a BIOS call
#define OUTa	0xd3	// Triggers a BDOS call

#ifdef PROFILE
unsigned long time_start = 0;
unsigned long time_now = 0;
#endif

static uint8 firstTime = TRUE;

void _PatchCPM(void) {
	uint16 i;

	//**********  Patch CP/M page zero into the memory  **********

	/* BIOS entry point */
	_RamWrite(0x0000, JP);		/* JP BIOS+3 (warm boot) */
	_RamWrite16(0x0001, BIOSjmppage + 3);

	if (Status != WBOOT) {
		/* IOBYTE - Points to Console */
		_RamWrite(0x0003, 0x3D);
		/* Current drive/user - A:/0 */
		_RamWrite(0x0004, 0x00);
	}

	/* BDOS entry point (0x0005) */
	_RamWrite(0x0005, JP);
	_RamWrite16(0x0006, BDOSjmppage + 0x06);

	//**********  Patch CP/M Version into the memory so the CCP can see it
	_RamWrite16(BDOSjmppage, 0x1600);
	_RamWrite16(BDOSjmppage + 2, 0x0000);
	_RamWrite16(BDOSjmppage + 4, 0x0000);

	// Patches in the BDOS jump destination
	_RamWrite(BDOSjmppage + 6, JP);
	_RamWrite16(BDOSjmppage + 7, BDOSpage);

	// Patches in the BDOS page content
	_RamWrite(BDOSpage, INa);
	_RamWrite(BDOSpage + 1, 0x00);
	_RamWrite(BDOSpage + 2, RET);

	if (firstTime) {
		// Patches in the BIOS jump destinations
		for (i = 0; i < 0x3F; i = i + 3) {
			_RamWrite(BIOSjmppage + i, JP);
			_RamWrite16(BIOSjmppage + i + 1, BIOSpage + i);
		}

		// Patches in the BIOS page content
		for (i = 0; i < 0x3F; i = i + 3) {
			_RamWrite(BIOSpage + i, OUTa);
			_RamWrite(BIOSpage + i + 1, i & 0xff);
			_RamWrite(BIOSpage + i + 2, RET);
		}

		//**********  Patch CP/M (fake) Disk Paramater Table after the BDOS call entry  **********
		i = DPBaddr;
		_RamWrite(i++, 64);  		/* spt - Sectors Per Track */
		_RamWrite(i++, 0);
		_RamWrite(i++, 5);   		/* bsh - Data allocation "Block Shift Factor" */
		_RamWrite(i++, 0x1F);		/* blm - Data allocation Block Mask */
		_RamWrite(i++, 1);   		/* exm - Extent Mask */
		_RamWrite(i++, 0xFF);		/* dsm - Total storage capacity of the disk drive */
		_RamWrite(i++, 0x07);
		_RamWrite(i++, 255); 		/* drm - Number of the last directory entry */
		_RamWrite(i++, 3);
		_RamWrite(i++, 0xFF);		/* al0 */
		_RamWrite(i++, 0x00);		/* al1 */
		_RamWrite(i++, 0);   		/* cks - Check area Size */
		_RamWrite(i++, 0);
		_RamWrite(i++, 0x02);		/* off - Number of system reserved tracks at the beginning of the ( logical ) disk */
		_RamWrite(i++, 0x00);
		blockShift = _RamRead(DPBaddr + 2);
		blockMask = _RamRead(DPBaddr + 3);
		extentMask = _RamRead(DPBaddr + 4);
		numAllocBlocks = _RamRead16((DPBaddr + 5)) + 1;
		extentsPerDirEntry = extentMask + 1;

		// figure out the number of the first allocation block
		// after the directory for the phoney allocation block
		// list in _findnext()
		firstBlockAfterDir = 0;
		i = 0x80;
		while (_RamRead(DPBaddr + 9) & i) {
			firstBlockAfterDir++;
			i >>= 1;
		}
		if (_RamRead(DPBaddr + 9) == 0xFF) {
			i = 0x80;
			while (_RamRead(DPBaddr + 10) & i) {
				firstBlockAfterDir++;
				i >>= 1;
			}
		}
		physicalExtentBytes = logicalExtentBytes * (extentMask + 1);
		
#ifdef CCP_ZCPR33
		uint16 c;
		
		_RamWrite16(Z3CL,Z3CL+4);  // Point to the 1st char in cmd line buf
		_RamWrite(Z3CL+2,Z3CL_B);  // Command line buffer size
		
		char autocmd[] = "STARTUP ";// 8 bytes max
		_RamWrite(Z3CL+3,sizeof(autocmd));
		c = 0;
		i = Z3CL+4;						// move the automatic command to the
		while( autocmd[c] ) {		// ZCPR3 command line
			_RamWrite(i++,autocmd[c]);
			++c;
		}
		_RamWrite(i,0);
		
		i = EXPATH;						// Initial path description
		_RamWrite(i++, '$');			// current drive
		_RamWrite(i++, '$');			// current user
		_RamWrite(i++, 1);			// Drive A:, user 15
		_RamWrite(i++, 15);
		_RamWrite(i++, 0);			// end of path
		
		_RamWrite(Z3WHL, 0xFF);		// turn the wheel byte on

		i = Z3ENV;							// move environment and TCAP
		_RamWrite(i++, JP);				// 0xFE00 - leading JP
		_RamWrite16(i, 0); i+=2;

		_RamWrite(i++, 'Z');				// 0xFE03 - environment ID
		_RamWrite(i++, '3');
		_RamWrite(i++, 'E');
		_RamWrite(i++, 'N');
		_RamWrite(i++, 'V');

		_RamWrite(i++, 1);				// 0xFE08 - class 1 environment (external);

		_RamWrite16(i, EXPATH); i+=2;	// 0xFE09 - external path (PATH)
		_RamWrite(i++, EXPATH_S);

		_RamWrite16(i, RCP); i+=2;		// 0xFE0C - resident command package (RCP)
		_RamWrite(i++, RCP_S);
		
		_RamWrite16(i, IOP); i+=2;		// 0xFE0F - input/output package (IOP)
		_RamWrite(i++, IOP_S);
		
		_RamWrite16(i, FCP); i+=2;		// 0xFE12 - flow command package (FCP)
		_RamWrite(i++, FCP_S);
		
		_RamWrite16(i, Z3NDIR); i+=2;	// 0xFE15 - named directories (NDR)
		_RamWrite(i++, Z3NDIR_S);

		_RamWrite16(i, Z3CL); i+=2;	// 0xFE18 - command line (CL)
		_RamWrite(i++, Z3CL_B);

		_RamWrite16(i, Z3ENV); i+=2;	// 0xFE1B - environment (ENV)
		_RamWrite(i++, Z3ENV_S);

		_RamWrite16(i, SHSTK); i+=2;	// 0xFE1E - shell stack (SH)
		_RamWrite(i++, SHSTK_N);
		_RamWrite(i++, SHSTK_L);

		_RamWrite16(i, Z3MSG); i+=2;	// 0xFE22 - message buffer (MSG)
		_RamWrite16(i, EXTFCB); i+=2;	// 0xFE24 - external FCB (FCB)		
		_RamWrite16(i, EXTSTK); i+=2;	// 0xFE26 - external stack (STK)
		_RamWrite(i++, 0);				// 0xFE28 - Quiet flag (1=quiet, 0=not quiet)
		_RamWrite16(i, Z3WHL); i+=2;	// 0xFE29 - Wheel byte (WHL)
		_RamWrite(i++, 143);				// 0xFE2B - Processor speed (Mhz)
		_RamWrite(i++, MAXDSK-'@');	// 0xFE2C - Max disk letter
		_RamWrite(i++, MAXUSR);			// 0xFE2D - Max user number
		_RamWrite(i++, 1);				// 0xFE2E - 1=ok to accept DU:, 0=not ok
		_RamWrite(i++, 0);				// 0xFE2F - CRT selection
		_RamWrite(i++, 0);				// 0xFE30 - Printer selection
		_RamWrite(i++, TERM_COLS);		// 0xFE31 - CRT 0: Width
		_RamWrite(i++, TERM_ROWS);		// 0xFE32 - # of lines
		_RamWrite(i++, TERM_ROWS - 2);// 0xFE33 - # of text lines
		_RamWrite(i++, 132);				// 0xFE34 - CRT 1: Width
		_RamWrite(i++, 24);				//	0xFE35 - # of lines
		_RamWrite(i++, 22);				//	0xFE36 - # of text lines
		_RamWrite(i++, 80);				// 0xFE37 - PRT 0: Width
		_RamWrite(i++, 66);				// 0xFE38 - # of lines
		_RamWrite(i++, 58);				//	0xFE39 - # of text lines
		_RamWrite(i++, 1);				//	0xFE3A - FF flag (1=can form feed)
		_RamWrite(i++, 96);				// 0xFE3B - PRT 1: Width
		_RamWrite(i++, 66);				//	0xFE3C - # of lines
		_RamWrite(i++, 58);				//	0xFE3D - # of text lines
		_RamWrite(i++, 1);				// 0xFE3E - FF flag (1=can form feed)
		_RamWrite(i++, 132);				// 0xFE3F - PRT 2: Width
		_RamWrite(i++, 66);				// 0xFE40 - # of lines
		_RamWrite(i++, 58);				//	0xFE41 - # of text lines
		_RamWrite(i++, 1);				//	0xFE42 - FF flag (1=can form feed)
		_RamWrite(i++, 132);				// 0xFE43 - PRT 3: Width
		_RamWrite(i++, 88);				//	0xFE44 - # of lines
		_RamWrite(i++, 82);				//	0xFE45 - # of text lines
		_RamWrite(i++, 1);				//	0xFE46 - FF flag (1=can form feed)
		//							  12345678123
		char shellvar[]	 = "SH      VAR";
		char genericfile[] = "           ";
		c=0;
		while( shellvar[c] ) {			// 0xFE47
			_RamWrite(i++,shellvar[c]);
			++c;
		}
		for( int j=0; j<4; ++j ) {
			c=0;
			while( genericfile[c] ) {	// 0xFE52, 0xFE5D, 0xFE68, 0xFE73
				_RamWrite(i++,genericfile[c]);
				++c;
			}
		}
		_RamWrite(i++,0);				// 0xFE7E - Public drive area (ZRDOS +)
		_RamWrite(i++,0);				// 0xFE7F - Public user area (ZRDOS +)
		//						 1234567890123
		char terminal[] = "DEC-VT100    ";
		c = 0;
		while( terminal[c] ) {
			_RamWrite(i++,terminal[c]);
			++c;
		}
		_RamWrite(i++, 0x50);			// graphics offset
		_RamWrite(i++, 0x80);			// config byte B14
		_RamWrite(i++, 0x10);			// config byte B15
		_RamWrite(i++, 'E'-'@');		// Cursor UP (Wordstar Defaults)
		_RamWrite(i++, 'X'-'@');		// Cursor DOWN
		_RamWrite(i++, 'D'-'@');		// Cursor RIGHT
		_RamWrite(i++, 'S'-'@');		// Cursor LEFT
		_RamWrite(i++, 50);				// CL Delay
		_RamWrite(i++, 5);				// CM Delay
		_RamWrite(i++, 3);				// CE Delay
		
		_RamWrite(i++, 0x1b);			// CL String
		_RamWrite(i++, '[');
		_RamWrite(i++, '2');
		_RamWrite(i++, 'J');
		_RamWrite(i++, 0);

		_RamWrite(i++, 0x1b);			// CM string
		_RamWrite(i++, '[');
		_RamWrite(i++, '%');
		_RamWrite(i++, 'i');
		_RamWrite(i++, '%');
		_RamWrite(i++, 'd');
		_RamWrite(i++, ';');
		_RamWrite(i++, '%');
		_RamWrite(i++, 'd');
		_RamWrite(i++, 'H');
		_RamWrite(i++, 0);

		_RamWrite(i++, 0x1b);			// CE string (clear to end of line)
		_RamWrite(i++, '[');
		_RamWrite(i++, 'K');
		_RamWrite(i++, 0);

		_RamWrite(i++, 0x1b);			// SO string (standout on)
		_RamWrite(i++, '[');
		_RamWrite(i++, '1');
		_RamWrite(i++, 'm');
		_RamWrite(i++, 0);
		
		_RamWrite(i++, 0x1b);			// SE string (standout off)
		_RamWrite(i++, '[');
		_RamWrite(i++, 'm');
		_RamWrite(i++, 0);
		
		_RamWrite(i++, 0);				// TI string (terminal init)
		
		_RamWrite(i++, 0);				// TE string; (terminal deinit);
//
// Extensions to standard Z3TCAP
//
		_RamWrite(i++, 0x1b);			// DL line delete
		_RamWrite(i++, '[');
		_RamWrite(i++, 'M');
		_RamWrite(i++, 0);
		
		_RamWrite(i++, 0x1b);			// IL line insert
		_RamWrite(i++, '[');
		_RamWrite(i++, 'L');
		_RamWrite(i++, 0);
		
		_RamWrite(i++, 0x1b);			// CD clear to end of screen
		_RamWrite(i++, '[');
		_RamWrite(i++, 'J');
		_RamWrite(i++, 0);
//
// The attribute string contains the four command characters to set
// the following four attributes for this terminal in the following
// order:  	Normal, Blink, Reverse, Underscore
//
		_RamWrite(i++, 0x1b);
		_RamWrite(i++, '[');
		_RamWrite(i++, '%');
		_RamWrite(i++, '+');
		_RamWrite(i++, '0');
		_RamWrite(i++, 'm');
		_RamWrite(i++, 0);
//
// attribute string
//
		_RamWrite(i++, '0');
		_RamWrite(i++, '5');
		_RamWrite(i++, '7');
		_RamWrite(i++, '4');
		_RamWrite(i++, 0);

		_RamWrite(i++, 0);				// Read current cursor position
		_RamWrite(i++, 0);				// Read line until cursor
//
// Graphics TCAP area
//
		_RamWrite(i++, 0);				// GO graphics on/off delay

		_RamWrite(i++, '\x0e');			// GS graphics on
		_RamWrite(i++, 0);

		_RamWrite(i++, '\x0f');			// GE graphics off
		_RamWrite(i++, 0);

		_RamWrite(i++, 0x1b);			// CDO cursor off
		_RamWrite(i++, '[');
		_RamWrite(i++, '?');
		_RamWrite(i++, '2');
		_RamWrite(i++, '5');
		_RamWrite(i++, 'l');
		_RamWrite(i++, 0);

		_RamWrite(i++, 0x1b);			// CDE cursor on
		_RamWrite(i++, '[');
		_RamWrite(i++, '?');
		_RamWrite(i++, '2');
		_RamWrite(i++, '5');
		_RamWrite(i++, 'h');
		_RamWrite(i++, 0);
//
// Graphics characters
//
		_RamWrite(i++, 0xDA);			// GULC upper left corner
		_RamWrite(i++, 0xBF);			// GURC upper right corner
		_RamWrite(i++, 0xC0);			// GLLC lower left corner
		_RamWrite(i++, 0xD9);			// GLrC lower right corner
		_RamWrite(i++, 0xC4);			// GHL horizontal line
		_RamWrite(i++, 0xB3);			// GVL vertical line
		_RamWrite(i++, 0xDB);			// GFB full block
		_RamWrite(i++, 0xB1);			// GHB hashed block
		_RamWrite(i++, 0xC2);			// GUI upper intersection
		_RamWrite(i++, 0xC1);			// GLI lower intersection
		_RamWrite(i++, 0xC5);			// GIS intersection
		_RamWrite(i++, 0xB4);			// GRTI right intersection
		_RamWrite(i++, 0xC3);			// GLTI left intersection
		_RamWrite(i++, 0);
#endif
		firstTime = FALSE;
	}
}

#ifdef DEBUGLOG
uint8 LogBuffer[128];

void _logRegs(void) {
	uint8 J, I;
	uint8 Flags[9] = { 'S','Z','5','H','3','P','N','C' };
	uint8 c = HIGH_REGISTER(AF);
	if (c < 32 || c > 126)
		c = 46;
	for (J = 0, I = LOW_REGISTER(AF); J < 8; ++J, I <<= 1) Flags[J] = I & 0x80 ? Flags[J] : '.';
	sprintf((char*)LogBuffer, "  BC:%04x DE:%04x HL:%04x AF:%02x(%c)|%s| IX:%04x IY:%04x SP:%04x PC:%04x\n",
		WORD16(BC), WORD16(DE), WORD16(HL), HIGH_REGISTER(AF), c, Flags, WORD16(IX), WORD16(IY), WORD16(SP), WORD16(PC)); _sys_logbuffer(LogBuffer);
}

void _logMem(uint16 address, uint8 amount)	// Amount = number of 16 bytes lines, so 1 CP/M block = 8, not 128
{
	uint8 i, m, c, pos;
	uint8 head = 8;
	uint8 hexa[] = "0123456789ABCDEF";
	for (i = 0; i < amount; ++i) {
		pos = 0;
		for (m = 0; m < head; ++m)
			LogBuffer[pos++] = ' ';
		sprintf((char*)LogBuffer, "  %04x: ", address);
		for (m = 0; m < 16; ++m) {
			c = _RamRead(address++);
			LogBuffer[pos++] = hexa[c >> 4];
			LogBuffer[pos++] = hexa[c & 0x0f];
			LogBuffer[pos++] = ' ';
			LogBuffer[m + head + 48] = c > 31 && c < 127 ? c : '.';
		}
		pos += 16;
		LogBuffer[pos++] = 0x0a;
		LogBuffer[pos++] = 0x00;
		_sys_logbuffer(LogBuffer);
	}
}

void _logChar(char* txt, uint8 c) {
	uint8 asc[2];

	asc[0] = c > 31 && c < 127 ? c : '.';
	asc[1] = 0;
	sprintf((char*)LogBuffer, "        %s = %02xh:%3d (%s)\n", txt, c, c, asc);
	_sys_logbuffer(LogBuffer);
}

void _logBiosIn(uint8 ch) {
	static const char* BIOSCalls[18] =
	{
		"boot", "wboot", "const", "conin", "conout", "list", "punch/aux", "reader", "home", "seldsk", "settrk", "setsec", "setdma",
		"read", "write", "listst", "sectran", "altwboot"
	};
	int index = ch / 3;
	if (index < 18) {
		sprintf((char*)LogBuffer, "\nBios call: %3d/%02xh (%s) IN:\n", ch, ch, BIOSCalls[index]); _sys_logbuffer(LogBuffer);
	} else {
		sprintf((char*)LogBuffer, "\nBios call: %3d/%02xh IN:\n", ch, ch); _sys_logbuffer(LogBuffer);
	}

	_logRegs();
}

void _logBiosOut(uint8 ch) {
	sprintf((char*)LogBuffer, "               OUT:\n"); _sys_logbuffer(LogBuffer);
	_logRegs();
}

void _logBdosIn(uint8 ch) {
	uint16 address = 0;
	uint8 size = 0;

	static const char* CPMCalls[41] =
	{
		"System Reset", "Console Input", "Console Output", "Reader Input", "Punch Output", "List Output", "Direct I/O", "Get IOByte",
		"Set IOByte", "Print String", "Read Buffered", "Console Status", "Get Version", "Reset Disk", "Select Disk", "Open File",
		"Close File", "Search First", "Search Next", "Delete File", "Read Sequential", "Write Sequential", "Make File", "Rename File",
		"Get Login Vector", "Get Current Disk", "Set DMA Address", "Get Alloc", "Write Protect Disk", "Get R/O Vector", "Set File Attr", "Get Disk Params",
		"Get/Set User", "Read Random", "Write Random", "Get File Size", "Set Random Record", "Reset Drive", "N/A", "N/A", "Write Random 0 fill"
	};

	if (ch < 41) {
		sprintf((char*)LogBuffer, "\nBdos call: %3d/%02xh (%s) IN from 0x%04x:\n", ch, ch, CPMCalls[ch], _RamRead16(SP) - 3); _sys_logbuffer(LogBuffer);
	} else {
		sprintf((char*)LogBuffer, "\nBdos call: %3d/%02xh IN from 0x%04x:\n", ch, ch, _RamRead16(SP) - 3); _sys_logbuffer(LogBuffer);
	}
	_logRegs();
	switch (ch) {
	case 2:
	case 4:
	case 5:
	case 6:
		_logChar("E", LOW_REGISTER(DE)); break;
	case 9:
	case 10:
		address = DE; size = 8; break;
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 22:
	case 23:
	case 30:
	case 35:
	case 36:
		address = DE; size = 3; break;
	case 20:
	case 21:
	case 33:
	case 34:
	case 40:
		address = DE; size = 3; _logMem(address, size);
		sprintf((char*)LogBuffer, "\n");  _sys_logbuffer(LogBuffer);
		address = dmaAddr; size = 8; break;
	default:
		break;
	}
	if (size)
		_logMem(address, size);
}

void _logBdosOut(uint8 ch) {
	uint16 address = 0;
	uint8 size = 0;

	sprintf((char*)LogBuffer, "              OUT:\n"); _sys_logbuffer(LogBuffer);
	_logRegs();
	switch (ch) {
	case 1:
	case 3:
	case 6:
		_logChar("A", HIGH_REGISTER(AF)); break;
	case 10:
		address = DE; size = 8; break;
	case 20:
	case 21:
	case 33:
	case 34:
	case 40:
		address = DE; size = 3; _logMem(address, size);
		sprintf((char*)LogBuffer, "\n");  _sys_logbuffer(LogBuffer);
		address = dmaAddr; size = 8; break;
	case 26:
		address = dmaAddr; size = 8; break;
	case 35:
	case 36:
		address = DE; size = 3; break;
	default:
		break;
	}
	if (size)
		_logMem(address, size);
}
#endif

void _Bios(void) {
	uint8 ch = LOW_REGISTER(PCX);

#ifdef DEBUGLOG
#ifdef LOGONLY
	if (ch == LOGONLY)
#endif
		_logBiosIn(ch);
#endif

	switch (ch) {
	case 0x00:
		Status = CBOOT;	// 0 - BOOT - Ends RunCPM
		break;
	case 0x03:
		Status = WBOOT;	// 1 - WBOOT - Back to CCP
		break;
	case 0x06:				// 2 - CONST - Console status
		SET_HIGH_REGISTER(AF, _chready());
		break;
	case 0x09:				// 3 - CONIN - Console input
		SET_HIGH_REGISTER(AF, _getch());
#ifdef DEBUG
		if (HIGH_REGISTER(AF) == 4)
			Debug = 1;
#endif
		break;
	case 0x0C:				// 4 - CONOUT - Console output
		_putcon(LOW_REGISTER(BC));
		break;
	case 0x0F:				// 5 - LIST - List output
		_putlpt(LOW_REGISTER(BC));
		break;
	case 0x12:				// 6 - PUNCH/AUXOUT - Punch output
		_putpun(LOW_REGISTER(BC));
		break;
	case 0x15:				// 7 - READER - Reader input
		SET_HIGH_REGISTER(AF, _getrdr());
		break;
	case 0x18:				// 8 - HOME - Home disk head
		break;
	case 0x1B:				// 9 - SELDSK - Select disk drive
		HL = 0x0000;
		break;
	case 0x1E:				// 10 - SETTRK - Set track number
		Serial.print("BIOS - SETTRK - "); Serial.println(LOW_REGISTER(BC));
		break;
	case 0x21:				// 11 - SETSEC - Set sector number
		Serial.print("BIOS - SETSEC - "); Serial.println(LOW_REGISTER(BC));
		break;
	case 0x24:				// 12 - SETDMA - Set DMA address
		HL = BC;
		dmaAddr = BC;
		break;
	case 0x27:				// 13 - READ - Read selected sector
		Serial.println("BIOS - READ");
		SET_HIGH_REGISTER(AF, 0x00);
		break;
	case 0x2A:				// 14 - WRITE - Write selected sector
		SET_HIGH_REGISTER(AF, 0x00);
		break;
	case 0x2D:				// 15 - LISTST - Get list device status
		SET_HIGH_REGISTER(AF, 0x0ff);
		break;
	case 0x30:				// 16 - SECTRAN - Sector translate
		HL = BC;			// HL=BC=No translation (1:1)
		break;
	case 0x33:				// 17 - RETTOCCP - This allows programs ending in RET return to internal CCP
		Status = RETCCP;
		break;
	case 0x36:           // 18 - MODEMINIT - serial port configuration
		_modeminit(BC);
		break;
	case 0x39:           // 19 - TTYIST - TTY input (RDR:) status
		HL = _ttyist();
		break;
	case 0x3C:
		HL = _ttyost();   // 20 - TTYOST - TTY output (PUN:) status
		break;
	default:
#ifdef DEBUG	// Show unimplemented BIOS calls only when debugging
		_puts("\r\nUnimplemented BIOS call.\r\n");
		_puts("C = 0x");
		_puthex8(ch);
		_puts("\r\n");
#endif
		break;
	}
#ifdef DEBUGLOG
#ifdef LOGONLY
	if (ch == LOGONLY)
#endif
		_logBiosOut(ch);
#endif

}

void _Bdos(void) {
	uint16	i;
	uint8	j, count, chr, c;
	
	bdosFunc = LOW_REGISTER(BC);

#ifdef DEBUGLOG
#ifdef LOGONLY
	if (bdosFunc == LOGONLY)
#endif
		_logBdosIn(bdosFunc);
#endif

	HL = 0x0000;	// HL is reset by the BDOS
	SET_LOW_REGISTER(BC, LOW_REGISTER(DE)); // C ends up equal to E

	switch (bdosFunc) {
		/*
		C = 0 : System reset
		Doesn't return. Reloads CP/M
		*/
	case 0:
		Status = WBOOT;	// Same as call to "BOOT"
		break;
		/*
		C = 1 : Console input
		Gets a char from the console
		Returns: A=Char
		*/
	case 1:
		HL = _getche();
#ifdef DEBUG
		if (HL == 4)
			Debug = 1;
#endif
		break;
		/*
		C = 2 : Console output
		E = Char
		Sends the char in E to the console
		*/
	case 2:
		_putcon(LOW_REGISTER(DE));
		break;
		/*
		C = 3 : Auxiliary (Reader) input
		Returns: A=Char
		*/
	case 3:
		HL = _getrdr();
		break;
		/*
		C = 4 : Auxiliary (Punch) output
		*/
	case 4:
		_putpun(LOW_REGISTER(DE));
		break;
		/*
		C = 5 : Printer output
		*/
	case 5:
		_putlpt(LOW_REGISTER(DE));
		break;
		/*
		C = 6 : Direct console IO
		E = 0xFF : Checks for char available and returns it, or 0x00 if none (read)
		E = 0xFE : Checks for char available and returns 0xFF if available, or 0x00 if none
		E = char : Outputs char (write)
		Returns: A=Char or 0x00 (on read)
		*/
	case 6:
		if (LOW_REGISTER(DE) == 0xff) {
			HL = _getchNB();
#ifdef DEBUG
			if (HL == 4)
				Debug = 1;
#endif
		} else if (LOW_REGISTER(DE) == 0xfe) {
			HL = _chready();
		} else {
			_putcon8(LOW_REGISTER(DE));
		}
		break;
		/*
		C = 7 : Get IOBYTE
		Gets the system IOBYTE
		Returns: A = IOBYTE
		*/
	case 7:
		HL = _RamRead(0x0003);
		break;
		/*
		C = 8 : Set IOBYTE
		E = IOBYTE
		Sets the system IOBYTE to E
		*/
	case 8:
		_RamWrite(0x0003, LOW_REGISTER(DE));
		break;
		/*
		C = 9 : Output string
		DE = Address of string
		Sends the $ terminated string pointed by (DE) to the screen
		*/
	case 9:
		while ((chr = _RamRead(DE++)) != '$')
			_putcon(chr);
		break;
		/*
		C = 10 (0Ah) : Buffered input
		DE = Address of buffer
		Reads (DE) bytes from the console
		Returns: A = Number os chars read
		DE) = First char
		*/
	case 10:
#ifdef PROFILE
		if (time_start != 0) {
			time_now = millis();
			printf(": %ld\n", time_now - time_start);
			time_start = 0;
		}
#endif
		i = WORD16(DE);
		c = _RamRead(i);	// Gets the number of characters to read
		++i;	// Points to the number read
		count = 0;
		while (c)	// Very simplistic line input
		{
			chr = _getch();
			if (chr == 3 && count == 0) {						// ^C
				_puts("^C");
				Status = WBOOT;
				break;
			}
#ifdef DEBUG
			if (chr == 4)										// ^D
				Debug = 1;
#endif
			if (chr == 5)										// ^E
				_puts("\r\n");
			if ((chr == 0x08 || chr == 0x7F) && count > 0) {	// ^H and DEL
				_puts("\b \b");
				count--;
				continue;
			}
			if (chr == 0x0A || chr == 0x0D) {					// ^J and ^M
#ifdef PROFILE
				time_start = millis();
#endif
				break;
			}
			if (chr == 18) {									// ^R
				_puts("#\r\n  ");
				for (j = 1; j <= count; ++j)
					_putcon(_RamRead(i + j));
			}
			if (chr == 21) {									// ^U
				_puts("#\r\n  ");
				i = WORD16(DE);
				c = _RamRead(i);
				++i;
				count = 0;
			}
			if (chr == 24) {									// ^X
				for (j = 0; j < count; ++j)
					_puts("\b \b");
				i = WORD16(DE);
				c = _RamRead(i);
				++i;
				count = 0;
			}
			if (chr < 0x20 || chr > 0x7E)						// Invalid character
				continue;
			_putcon(chr);
			++count; _RamWrite((i + count) & 0xffff, chr);
			if (count == c)										// Reached the expected count
				break;
		}
		_RamWrite(i & 0xffff, count);	// Saves the number of characters read
		_putcon('\r');	// Gives a visual feedback that read ended
		break;
		/*
		C = 11 (0Bh) : Get console status
		Returns: A=0x00 or 0xFF
		*/
	case 11:
		HL = _chready();
		break;
		/*
		C = 12 (0Ch) : Get version number
		Returns: B=H=system type, A=L=version number
		*/
	case 12:
		HL = 0x22;
		break;
		/*
		C = 13 (0Dh) : Reset disk system
		*/
	case 13:
		roVector = 0;		// Make all drives R/W
		loginVector = 0;
		dmaAddr = 0x0080;
		cDrive = 0;			// userCode remains unchanged
		HL = _CheckSUB();	// Checks if there's a $$$.SUB on the boot disk
		break;
		/*
		C = 14 (0Eh) : Select Disk
		Returns: A=0x00 or 0xFF
		*/
	case 14:
		oDrive = cDrive;
		cDrive = LOW_REGISTER(DE);
		HL = _SelectDisk(LOW_REGISTER(DE) + 1);	// +1 here is to allow SelectDisk to be used directly by disk.h as well
		if (!HL)
			oDrive = cDrive;
		break;
		/*
		C = 15 (0Fh) : Open file
		Returns: A=0x00 or 0xFF
		*/
	case 15:
		HL = _OpenFile(DE);
		break;
		/*
		C = 16 (10h) : Close file
		*/
	case 16:
		HL = _CloseFile(DE);
		break;
		/*
		C = 17 (11h) : Search for first
		*/
	case 17:
		HL = _SearchFirst(DE, TRUE);	// TRUE = Creates a fake dir entry when finding the file
		break;
		/*
		C = 18 (12h) : Search for next
		*/
	case 18:
		HL = _SearchNext(DE, TRUE);		// TRUE = Creates a fake dir entry when finding the file
		break;
		/*
		C = 19 (13h) : Delete file
		*/
	case 19:
		HL = _DeleteFile(DE);
		break;
		/*
		C = 20 (14h) : Read sequential
		*/
	case 20:
		HL = _ReadSeq(DE);
		break;
		/*
		C = 21 (15h) : Write sequential
		*/
	case 21:
		HL = _WriteSeq(DE);
		break;
		/*
		C = 22 (16h) : Make file
		*/
	case 22:
		HL = _MakeFile(DE);
		break;
		/*
		C = 23 (17h) : Rename file
		*/
	case 23:
		HL = _RenameFile(DE);
		break;
		/*
		C = 24 (18h) : Return log-in vector (active drive map)
		*/
	case 24:
		HL = loginVector;	// (todo) improve this
		break;
		/*
		C = 25 (19h) : Return current disk
		*/
	case 25:
		HL = cDrive;
		break;
		/*
		C = 26 (1Ah) : Set DMA address
		*/
	case 26:
		dmaAddr = DE;
		break;
		/*
		C = 27 (1Bh) : Get ADDR(Alloc)
		*/
	case 27:
		HL = SCBaddr;
		break;
		/*
		C = 28 (1Ch) : Write protect current disk
		*/
	case 28:
		roVector = roVector | (1 << cDrive);
		break;
		/*
		C = 29 (1Dh) : Get R/O vector
		*/
	case 29:
		HL = roVector;
		break;
		/*
		C = 30 (1Eh) : Set file attributes
		*/
	case 30:
		HL = _SetFileAttributes(DE);
		break;
		/*
		C = 31 (1Fh) : Get ADDR(Disk Parms)
		*/
	case 31:
		HL = DPBaddr;
		break;
		/*
		C = 32 (20h) : Get/Set user code
		*/
	case 32:
		if (LOW_REGISTER(DE) == 0xFF) {
			HL = userCode;
		} else {
			_SetUser(DE);
		}
		break;
		/*
		C = 33 (21h) : Read random
		*/
	case 33:
		HL = _ReadRand(DE);
		break;
		/*
		C = 34 (22h) : Write random
		*/
	case 34:
		HL = _WriteRand(DE);
		break;
		/*
		C = 35 (23h) : Compute file size
		*/
	case 35:
		HL = _GetFileSize(DE);
		break;
		/*
		C = 36 (24h) : Set random record
		*/
	case 36:
		HL = _SetRandom(DE);
		break;
		/*
		C = 37 (25h) : Reset drive
		*/
	case 37:
		break;
		/********** Function 38: Not supported by CP/M 2.2 **********/
		/*
		C = 39 (27h) : Return fixed media vector
		*/
	case 39:
		HL = (1<<(MAXDSK-'@'))-1;
		break;
		
		/********** (todo) Function 40: Write random with zero fill **********/
		/*
		C = 40 (28h) : Write random with zero fill (we have no disk blocks, so just write random)
		*/
	case 40:
		HL = _WriteRand(DE);
		break;
		/*
		C = 41 (29h) : Get/Set/Reset NovaDOS _
		*/
	case 41:		// NovaDOS
		if (!LOW_REGISTER(DE)) {					// E=0 means return flags
			HL = novaDOSflags;
		} else if (LOW_REGISTER(DE) & 0x80) {	// set indicated flags
			novaDOSflags |= (LOW_REGISTER(DE) & 0x7F);
		} else {											// reset indicated flags
			novaDOSflags &= ~LOW_REGISTER(DE);
		}
		break;
		/*
		C = 47 (2Fh):  Return Current DMA Address
		*/
	case 47:		// NovaDOS, ZRDOS
		HL = dmaAddr;
		break;
		/*
		C = 48 (30h) : return 0x0019 to identiry BDOS as ZRDOS 1.9
							(which is what NovaDOS H did)
		*/
	case 48:		// NovaDOS, ZRDOS
		HL = 0x0019;
		break;
		/*
		C = 50 (32h) :Set Warm Boot Trap
		*/
	case 50:		// NovaDOS, ZRDOS
		warmBootTrap = DE;
		break;
		/*
		C = 52 (33h) :Reset Warm Boot Trap
		*/
	case 52:		// NovaDOS, ZRDOS
		warmBootTrap = 0;
		break;
		/*
		C = 54 : Get file timestamps
		OUT: HL = address of timestamp from the last file used by 
		functions 15 (open file), 17 (find first) or 18 (find next).

		The format of a NovaDOS timestamp is:

		DW  creation date
		DB  creation time, hours,	BCD
		DB  creation time, minutes, BCD
		DW  modification date
		DB  modification time, hours,	BCD
		DB  modification time, minutes, BCD
		*/
	case 54:		// NovaDOS, Z80DOS
		HL = _getFileTimeStamp();
		break;
		/*
		C = 55 : Use creation date and last modified date and time stored
					by Get Stamp instead of real time for the next Write,
					Make File or Close File call.
		*/
	case 55:		// NovaDOS, Z80DOS
		useFileStamp = true;
		break;
		/*
		C = 105, 200 (69h, C8h) : GetTime
		IN: DE = address to receive time block (5 bytes)
		*/
	case 105:	// Z80DOS
	case 200:	// NovaDOS
		_getTime(DE);
		HL = 0;
		break;		
		/*
		C = 104, 201 (68h, C9h) : SetTime
		IN: DE = address to time block to set time to (5 bytes)
		*/
	case 104:	// Z80DOS
	case 201:	// NovaDOS
		_setTime(DE);
		HL = 0;
		break;
		
#ifdef GSX
		/*
		C = 115 (73h) : GSX
		IN: DE = parameter block address
		
		DEFW	CONTRL	;Address of control array
		DEFW	INTIN		;Address of integer input array
		DEFW	PTSIN		;Address of pixel input array
		DEFW	INTOUT	;Address of integer output array
		DEFW	PTSOUT	;Address of pixel output array
		*/
	case 115:	// GSX
		HL = _gdos(DE);
		break;
#endif

	case 141:	// MP/M, Concurrent CP/M, CP/M-86 v4
		/*
		C = 141 : Delay
		IN: DE = # of milliseconds to pause. Originally was # of
               system clock ticks but ms are of more use to me.
		*/
		_delay(DE);
		break;
   case 165:
      HL = _millis();
      break;
#if defined ARDUINO || defined CORE_TEENSY || defined ESP32
		/*
		C = 220 (DCh) : PinMode
		*/
	case 220:
		pinMode(HIGH_REGISTER(DE), LOW_REGISTER(DE));
		break;
		/*
		C = 221 (DDh) : DigitalRead
		*/
	case 221:
		HL = digitalRead(HIGH_REGISTER(DE));
		break;
		/*
		C = 222 (DEh) : DigitalWrite
		*/
	case 222:
		digitalWrite(HIGH_REGISTER(DE), LOW_REGISTER(DE));
		break;
		/*
		C = 223 (DFh) : AnalogRead
		*/
	case 223:
		HL = analogRead(HIGH_REGISTER(DE));
		break;
		/*
		C = 224 (E0h) : AnalogWrite
		*/
#ifndef ESP32
	case 224:
		analogWrite(HIGH_REGISTER(DE), LOW_REGISTER(DE));
		break;
#endif

#endif
		/*
		C = 230 (E6h) : Set 8 bit masking
		*/
	case 230:
		if (LOW_REGISTER(DE) == 0xFF) {
			novaDOSflags |= HiOutFlag;		// allow 8 bit output
		} else {
			novaDOSflags &= ~HiOutFlag;	// allow 7 b
		}
		break;
		/*
		C = 231 (E7h) : Host specific BDOS call
		*/
	case 231:
		HL = hostbdos(DE);
		break;
		/*
		C = 232 (E8h) : ESP32 specific BDOS call
		*/
#ifdef ESP32
	case 232:
		HL = esp32bdos(DE);
		break;
#endif
#ifdef _STM32_DEF_
	case 232:
		HL = stm32bdos(DE);
		break;
#endif
		/*
		C = 249 (F9h) : MakeDisk
		Makes a disk directory if not existent.
		*/
	case 249:
		HL = _MakeDisk(DE);
		break;
		/*
		C = 250 (FAh) : HostOS
		Returns: A = 0x00 - Windows / 0x01 - Arduino / 0x02 - Posix / 0x03 - Dos / 0x04 - Teensy / 0x05 - ESP32 / 0x06 - STM32
		*/
	case 250:
		HL = HostOS;
		break;
		/*
		C = 251 (FBh) : Version
		Returns: A = 0xVv - Version in BCD representation: V.v
		*/
	case 251:
		HL = VersionBCD;
		break;
		/*
		C = 252 (FCh) : CCP version
		Returns: A = 0x00-0x05 = DRI|CCPZ|ZCPR2|ZCPR3|Z80CCP|ZCPR33 / 0xVv = Internal version in BCD: V.v
		*/
	case 252:
		HL = VersionCCP;
		break;
		/*
		C = 253 (FDh) : CCP address
		*/
	case 253:
		HL = CCPaddr;
		break;
#ifdef HASLUA
		/*
		C = 254 (FEh) : Run Lua file
		*/
	case 254:
		HL = _RunLua(DE);
		break;
#endif
		/*
		Unimplemented calls get listed
		*/
	default:
#ifdef DEBUG	// Show unimplemented BDOS calls only when debugging
		_puts("\r\nUnimplemented BDOS call.\r\n");
		_puts("C = 0x");
		_puthex8(bdosFunc);
		_puts("\r\n");
#endif
		break;
	}
	bdosFunc = 0;
	
	if (!errorTrapped) {
		// CP/M BDOS does this before returning
		SET_HIGH_REGISTER(BC, HIGH_REGISTER(HL));
		SET_HIGH_REGISTER(AF, LOW_REGISTER(HL));
	} else {
		errorTrapped = FALSE;
	}

#ifdef DEBUGLOG
#ifdef LOGONLY
	if (ch == LOGONLY)
#endif
		_logBdosOut(ch);
#endif

}

#endif
