#ifndef GSX_H
#define GSX_H

#if defined ST7735
#include <Adafruit_GFX.h>
#if defined CORE_TEENSY
#include <ST7735_t3.h>
#else
#include <ST7735.h>
#endif
#elif defined ILI9341
#if defined CORE_TEENSY
#include <ILI9341_t3.h>
#else
#include <ILI9341.h>
#endif
#endif

enum {
	GSX_PB_CONTRL = 0,
	GSX_PB_INTIN,
	GSX_PB_PTSIN,
	GSX_PB_INTOUT,
	GSX_PB_PTSOUT,
	
	GSX_PB_MAX,
};

enum {	
	CONTRL_FUNCTION = 0, // function
	CONTRL_PTSIN, 
	CONTRL_PTSOUT,	
	CONTRL_INTIN,
	CONTRL_INTOUT,
	CONTRL_SPECIAL
};

enum {
	CONTROL_FUNCTIONID = 5,
};

enum {	
	OWS_ID = 0,
	OWS_LINETYPE,
	OWS_POLYLINE_COLOR_INDEX,
	OWS_MARKER_TYPE,
	OWS_POLYMARKER_COLOR_INDEX,
	OWS_TEXT_FONT,
	OWS_TEXT_COLOR_INDEX,
	OWS_FILL_INTERIOR_STYLE,
	OWS_FILL_STYLE_INDEX,
	OWS_FILL_COLOR_INDEX
};

enum {
	OWS_INTOUT_MAX_WIDTH,
	OWS_INTOUT_MAX_HEIGHT,
	OWS_INTOUT_COORD_FLAGS,
	OWS_INTOUT_PIXEL_WIDTH,
	OWS_INTOUT_PIXEL_HEIGHT
};

enum {
	LS_SOLID = 1,
	LS_DASH,
	LS_DOT,
	LS_DASH_DOT,
	LS_LONG_DASH
};

const static uint16 _gsx_line_style_mask[] = {
	0,
	0b1111111111111111,
	0b1111111100000000,
	0b1110000011100000,
	0b1111111000111000,
	0b1111111111110000
};

enum {
	MS_DOT = 1,
	MS_PLUS,
	MS_ASTERISK,
	MS_CIRCLE,
	MS_CROSS
};

enum {
	FS_HOLLOW = 0,
	FS_SOLID,
	FS_HALFTONE,
	FS_HATCH
};

enum {
	FSI_VERTICAL = 1,
	FSI_HORIZONTAL,
	FSI_PLUS45,
	FSI_MINUS45,
	FSI_CROSS,
	FSI_X
};

const static uint32 halftone1[] = {
	0xAAAAAAAA,
	0x00000000,
	0xA888A888,
	0x00000000,
	0xAAAAAAAA,
	0x00000000,
	0x88888888,
	0x00000000,
	0xAAAAAAAA,
	0x00000000,
	0xA8A888A8,
	0x00000000,
	0xAAAAAAAA,
	0x00000000,
	0x88888888,
	0x00000000,
	0xAAAAAAAA,
	0x00000000,
	0xA888A888,
	0x00000000,
	0xAAAAAAAA,
	0x00000000,
	0x88888888,
	0x00000000,
	0xAAAAAAAA,
	0x00000000,
	0x88A8A8A8,
	0x00000000,
	0xAAAAAAAA,
	0x00000000,
	0x88888888,
	0x00000000
};

const static uint32 halftone2[] = {
	0x11111111,
	0xAAAAAAAA,
	0x44444445,
	0xAAAAAAAA,
	0x11111111,
	0xAAAAAAAA,
	0x54545454,
	0xAAAAAAAA,
	0x11111111,
	0xAAAAAAAA,
	0x45444544,
	0xAAAAAAAA,
	0x11111111,
	0xAAAAAAAA,
	0x54545454,
	0xAAAAAAAA,
	0x11111111,
	0xAAAAAAAA,
	0x44454445,
	0xAAAAAAAA,
	0x11111111,
	0xAAAAAAAA,
	0x54545454,
	0xAAAAAAAA,
	0x11111111,
	0xAAAAAAAA,
	0x45444544,
	0xAAAAAAAA,
	0x11111111,
	0xAAAAAAAA,
	0x54545454,
	0xAAAAAAAA
};

const static uint32 halftone3[] = {
	0x55555555,
	0xABABABAB,
	0x55555555,
	0xEEEEEEEE,
	0x55555555,
	0xBABABABA,
	0x55555555,
	0xEEEEEEEE,
	0x55555555,
	0xABABABAB,
	0x55555555,
	0xEEEEEEEE,
	0x55555555,
	0xBBBABBBA,
	0x55555555,
	0xEEEEEEEE,
	0x55555555,
	0xABABABAB,
	0x55555555,
	0xEEEEEEEE,
	0x55555555,
	0xBABBBABA,
	0x55555555,
	0xEEEEEEEE,
	0x55555555,
	0xABABABAB,
	0x55555555,
	0xEEEEEEEE,
	0x55555555,
	0xBBBABBBA,
	0x55555555,
	0xEEEEEEEE
};

const static uint32 halftone4[] = {
	0x55575557,
	0xFFFFFFFF,
	0xDDDDDDDD,
	0xFFFFFFFF,
	0x75757575,
	0xFFFFFFFF,
	0xDDDDDDDD,
	0xFFFFFFFF,
	0x57555757,
	0xFFFFFFFF,
	0xDDDDDDDD,
	0xFFFFFFFF,
	0x75757575,
	0xFFFFFFFF,
	0xDDDDDDDD,
	0xFFFFFFFF,
	0x55575557,
	0xFFFFFFFF,
	0xDDDDDDDD,
	0xFFFFFFFF,
	0x75757575,
	0xFFFFFFFF,
	0xDDDDDDDD,
	0xFFFFFFFF,
	0x57575755,
	0xFFFFFFFF,
	0xDDDDDDDD,
	0xFFFFFFFF,
	0x75757575,
	0xFFFFFFFF,
	0xDDDDDDDD,
	0xFFFFFFFF
};

enum {
	WM_REPLACE = 1,
	WM_TRANSPARENT,
	WM_XOR,
	WM_ERASE
};

enum {
	LID_LOCATOR = 1,
	LID_VALUATOR,
	LID_CHOICE,
	LID_STRING,
};
#define LID_MAX 4

enum {
	IM_REQUEST = 1,
	IM_SAMPLE
};

enum {
	BLACK_565		= 0b0000000000000000,
	GREY01_565		= 0b0001000010000010,
	GREY02_565		= 0b0010000100000100,
	GREY03_565		= 0b0011000110000110,
	GREY04_565		= 0b0100001000001000,
	GREY05_565		= 0b0101001010001010,
	GREY06_565		= 0b0110001100001100,
	GREY07_565		= 0b0111001110001110,
	GREY08_565		= 0b1000010000010000,
	GREY09_565		= 0b1001010010010010,
	GREY10_565		= 0b1010010100010100,
	GREY11_565		= 0b1011010110010110,
	GREY12_565		= 0b1100011000011000,
	GREY13_565		= 0b1101011010011010,
	GREY14_565		= 0b1110011100011100,
	WHITE_565		= 0b1111111111111111,
	NAVY_565			= 0b0000000000001111,	//	  0,   0, 123
	DARKGREEN_565	= 0b0000001111100000,	//	  0, 125,   0
	DARKCYAN_565	= 0b0000001111101111,	//	  0, 125, 123
	MAROON_565		= 0b0111100000000000,	//	123,   0,   0
	PURPLE_565		= 0b0111100000001111,	//	123,   0, 123
	OLIVE_565		= 0b0111101111100000,	//	123, 125,   0
	BLUE_565			= 0b0000000000011111,	//	  0,   0, 255
	GREEN_565		= 0b0000011111100000,	//	  0, 255,   0
	CYAN_565			= 0b0000011111111111,	//	  0, 255, 255
	RED_565			= 0b1111100000000000,	//	255,   0,   0
	MAGENTA_565		= 0b1111100000011111,	//	255,   0, 255
	YELLOW_565		= 0b1111111111100000,	//	255, 255,   0
	ORANGE_565		= 0b1111110100100000,	//	255, 165,   0
	GREENYELLOW_565= 0b1010111111100101,	//	173, 255,  41
	PINK_565			= 0b1111110000011000		//	255, 130, 198
};

#define	MAX_PTSIN		75		// This is what CP/M-80 had (all the room
										// it had in GDOS for internal buffer).
										// If needed, we can expand arbitrarly.
#define	GSX_MIN_NDC	0
#define	GSX_MAX_NDC	32767

int16*	_gsx_contrl;
int16*	_gsx_intin;
int16*	_gsx_ptsin;
int16*	_gsx_intout;
int16*	_gsx_ptsout;

uint16	_gsx_device_number;
uint16	_gsx_line_style;
uint16	_gsx_line_colour;
uint16	_gsx_line_width;
uint16	_gsx_marker_style;
uint16	_gsx_marker_colour;
uint16	_gsx_marker_height;
uint16	_gsx_text_style;
uint16	_gsx_text_colour;
uint16	_gsx_text_height;
uint16	_gsx_text_width;
uint16	_gsx_text_size;
uint16	_gsx_text_direction;
uint16	_gsx_text_font;
uint16	_gsx_fill_style;
uint16	_gsx_fill_index;
uint16	_gsx_fill_colour;
uint16	_gsx_writing_mode;
uint16	_gsx_input_mode[LID_MAX + 1];

int16	_gsx_int_ptsin[MAX_PTSIN * 2 + 2]; // allow one extra vertice to close polygon

int16	_gsx_maxX, _gsx_maxY;
bool		_gsx_reverse_video;

int16	_gsx_gc_x = 0, _gsx_gc_y = 0;

#define TFT_RST   8  // RST can use any pin
#define TFT_DC    9  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define TFT_CS    2  //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define TFT_BL    7	// LCD backlight

#define GSX_GC_ARMLNG	10
#define GSX_GC_W			(2 * GSX_GC_ARMLNG + 1)

#if defined ST7735

#define PIXEL_WIDTH	223				// Width of a pixel, in thousandths of a millimetre.
#define PIXEL_HEIGHT	223				// Height of a pixel, in thousandths of a millimetre.

#define TFT_BL_ON		LOW
#define TFT_BL_OFF	HIGH

#undef TFT_READBACK

#define TFT_INIT		{ tft.initR(INITR_BLACKTAB); tft.cp437(true); }

#if defined CORE_TEENSY		// Teensy 3.5, 3.6, 4.0
ST7735_t3 tft = ST7735_t3(TFT_CS, TFT_DC, TFT_RST);
#else
ST7735 tft = ST7735(TFT_CS, TFT_DC, TFT_RST);
#endif

#elif defined ILI9341

#define PIXEL_WIDTH	178				// Width of a pixel, in thousandths of a millimetre.
#define PIXEL_HEIGHT	178				// Height of a pixel, in thousandths of a millimetre.

#define TFT_BL_ON		HIGH
#define TFT_BL_OFF	LOW

#define TFT_READBACK

#define TFT_INIT	{ tft.begin(); }

#if defined CORE_TEENSY
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
#else
ILI9341 tft = ILI9341(TFT_CS, TFT_DC);
#endif

#endif

static uint16	_gsx_colour_table[] = {
	BLACK_565,
	RED_565,
	GREEN_565,
	BLUE_565,
	CYAN_565,
	YELLOW_565,
	MAGENTA_565,
	WHITE_565,
	BLACK_565,
	GREY01_565,
	GREY02_565,
	GREY03_565,
	GREY04_565,
	GREY05_565,
	GREY06_565,
	GREY07_565,
	GREY08_565,
	GREY09_565,
	GREY10_565,
	GREY11_565,
	GREY12_565,
	GREY13_565,
	GREY14_565,
	WHITE_565,
	NAVY_565,
	DARKGREEN_565,
	DARKCYAN_565,
	MAROON_565,
	PURPLE_565,
	OLIVE_565,
	ORANGE_565,
	GREENYELLOW_565,
	PINK_565
};
#define	_GSX_NUM_COLOURS (sizeof _gsx_colour_table/sizeof(uint16))

void _gsx_close_workstation();
void _gsx_clear_workstation();
void _gsx_filled_hline( int16 x0, int16 x1, int16 y);
void _gsx_filled_circle(int16 x0, int16 y0, int16 r);
void _gsx_draw_arc_point(int16 x0, int16 y0, int16 x, int16 y, int16 a0, int16 a1, int16 clr);
void _gsx_draw_arc(int16 x0, int16 y0, int16 r, int16 arc_s, int16 arc_e, int16 clr);
int16 _gsx_ndc_to_dev(long ndc, long dev_min, long dev_max);
//int16 _gsx_ndc_to_dev(int16 ndc, int16 dev_min, int16 dev_max);
int16 _gsx_dev_to_ndc(long dev, long dev_min, long dev_max);
//int16 _gsx_dev_to_ndc(int16 dev, int16 dev_min, int16 dev_max);
uint16 _gsx_colour565(uint16 r, uint16 g, uint16 b);
void _gsx_place_graphic_cursor(int16 x, int16 y);
void _gsx_remove_graphic_cursor();

#define TH_MIN	7
#define TW_MIN 5

#define LW_MAX	1

void _gsx_init() {
   pinMode(TFT_BL,OUTPUT);
   digitalWrite(TFT_BL,TFT_BL_OFF);
   TFT_INIT;
   tft.setRotation(3);
   tft.setTextWrap(false);
#if defined ILI9341
   tft.sleep(true);
#endif
}

// GSX 1: open workstation REQ for CRT
void _gsx_open_workstation() {
	_gsx_close_workstation();
	
	_gsx_device_number	= _gsx_intin[OWS_ID];
	_gsx_line_style		= _gsx_intin[OWS_LINETYPE];
	_gsx_line_colour		= _gsx_intin[OWS_POLYLINE_COLOR_INDEX];
	_gsx_marker_style		= _gsx_intin[OWS_MARKER_TYPE];
	_gsx_marker_colour	= _gsx_intin[OWS_POLYMARKER_COLOR_INDEX];
	_gsx_text_style		= _gsx_intin[OWS_TEXT_FONT];
	_gsx_text_colour		= _gsx_intin[OWS_TEXT_COLOR_INDEX];
	_gsx_fill_style		= _gsx_intin[OWS_FILL_INTERIOR_STYLE];
	_gsx_fill_index		= _gsx_intin[OWS_FILL_STYLE_INDEX];
	_gsx_fill_colour		= _gsx_intin[OWS_FILL_COLOR_INDEX];
	
	_gsx_text_height		= TH_MIN;
	_gsx_text_width		= TW_MIN;
	_gsx_text_size			= 1;
	_gsx_text_direction	= 0;
	_gsx_text_font			= 1;
	_gsx_line_width		= 1;
	_gsx_marker_height	= 1;		// minimum marker height
	_gsx_writing_mode		= WM_REPLACE;
	for (uint8 i = 0; i < LID_MAX; ++i ) {
		_gsx_input_mode[i] = IM_REQUEST;
	}
	_gsx_reverse_video	= false;

	_gsx_contrl[CONTRL_PTSOUT] = 6;	// # points in _gsx_ptsout
	
	_gsx_ptsout[0] = 0;
	_gsx_ptsout[1] = _gsx_text_height;	// minimum character height
	_gsx_ptsout[2] = 0;
	_gsx_ptsout[3] = _gsx_text_height*4;	// maximum character height
	_gsx_ptsout[4] = 1;	// minimum line width
	_gsx_ptsout[5] = 0;
	_gsx_ptsout[6] = 1;	// maximum line width
	_gsx_ptsout[7] = 0;
	_gsx_ptsout[8] = 0;
	_gsx_ptsout[9] = _gsx_marker_height;	// minimum marker height
	_gsx_ptsout[10] = 0;
	_gsx_ptsout[11] = 239;	// maximum marker height.
	
	_gsx_contrl[CONTRL_INTOUT] = 45;	// # integers in _gsx_intout

	_gsx_intout[OWS_INTOUT_MAX_WIDTH] = tft.width()-1;		// Screen width, device units
	_gsx_intout[OWS_INTOUT_MAX_HEIGHT] = tft.height()-1;	// Screen height, device units
	_gsx_intout[OWS_INTOUT_COORD_FLAGS] = 1; 					// 0 if device is capable of
																			// continuous scaling (printer)
																			// 1 if not (CRT)
	_gsx_intout[OWS_INTOUT_PIXEL_WIDTH] = PIXEL_WIDTH;		// Width of a pixel, in thousandths of a millimetre.
	_gsx_intout[OWS_INTOUT_PIXEL_HEIGHT] = PIXEL_HEIGHT;	// Height of a pixel, in thousandths of a millimetre.
 	_gsx_intout[5] = 4;	// Number of character sizes, 0 for continuous sizing.
	_gsx_intout[6] = 5;	// Number of line styles.
	_gsx_intout[7] = 1;	// Number of line widths.
	_gsx_intout[8] = 5;	// Number of marker styles.
	_gsx_intout[9] = 239;// Number of marker sizes.
	_gsx_intout[10] = 1;	// Number of fonts.
	_gsx_intout[11] = 6;	// Number of patterns.
	_gsx_intout[12] = 6;	// Number of hatch styles.
	_gsx_intout[13] = 65535;	//	Number of colours displayable at once.

	_gsx_intout[14] = 3;	// Number of General Drawing Primitives
	_gsx_intout[15] = 1;	// General Drawing Primitive numbers.
	_gsx_intout[16] = 2;	//-1 => End of list
	_gsx_intout[17] = 4;	// 1 => Bar
   _gsx_intout[18] = -1;// 2 => Arc
								// 3 => Pie slice
								// 4 => Circle
								// 5 => Ruling characters
 
	_gsx_intout[25] = 0;	// General Drawing Primitive attributes
	_gsx_intout[26] = 1;	//-1 => End of list
	_gsx_intout[27] = 2;	// 0 => Polyline
	_gsx_intout[28] = 3;	// 1 => Polymarker
	_gsx_intout[29] = 4;	// 2 => Text
	_gsx_intout[30] = -1;// 3 => Filled area
								// 4 => None
 
	_gsx_intout[35] = 1; //	0 for black/white, 1 for colour.
	_gsx_intout[36] = 1; //	0 if text rotation is not possible, 1 if it is.
	_gsx_intout[37] = 1; //	0 if filled areas are not possible, 1 if they are.
	_gsx_intout[38] = 1; //	0 if cannot read cell array, 1 if can.
	_gsx_intout[39] = 0; //	Number of colours in the palette.
								// 0 => More than 32767
								// 2 => Black and white
								// >2 => number of colours available
	_gsx_intout[40] = 1;	//	Number of locator devices (mice, tablets, lightpens)
	_gsx_intout[41] = 0;	//	Number of valuator devices
	_gsx_intout[42] = 0;	//	Number of choice devices
	_gsx_intout[43] = 0;	// Number of string devices
	_gsx_intout[44] = 2;	//	Workstation type
								// 0 => Output only
								// 1 => Input only
								// 2 => Input and output
								// 3 => Segment storage
								// 4 => GKS metafile output.
#if defined ILI9341
	tft.sleep(false);
#endif
	_gsx_clear_workstation();
   digitalWrite(TFT_BL,TFT_BL_ON);
}

// GSX 2: close workstation REQ for CRT
void _gsx_close_workstation() {
	_gsx_clear_workstation();
#if defined ILI9341
	tft.sleep(true);
#endif
   digitalWrite(TFT_BL,TFT_BL_OFF);
}

// GSX 3: clear workstation REQ for CRT
void _gsx_clear_workstation() {
   tft.fillScreen(BLACK_565);
   tft.setCursor(0, 0);
   tft.setTextColor(WHITE_565);
   tft.fillScreen(BLACK_565);
}

// GSX 4: update workstation REQ for CRT
void _gsx_update_workstation() {
}

// GSX 5:  escape REQ for CRT
void _gsx_escape() {
	switch (_gsx_contrl[CONTROL_FUNCTIONID]) {
		
		case 1:	// inquire addressable character cells REQ for CRT
			_gsx_contrl[CONTRL_INTOUT] = 2;
			_gsx_intout[0] = tft.height() / (TH_MIN + 1);
			_gsx_intout[1] = tft.width() / (TW_MIN + 1);
			break;
			
		case 2:	// enter graphics mode REQ for CRT
			// TODO
			break;
			
		case 3:	// exit graphics mode REQ for CRT
			// TODO
			break;
			
		case 4:	// cursor up REQ for CRT
			if (tft.getCursorY() - (TH_MIN + 1) >= 0) {
				tft.setCursor(tft.getCursorX(), tft.getCursorY() - (TH_MIN + 1));
			}
			break;
			
		case 5:	// cursor down REQ for CRT
			if (tft.getCursorY() + (TH_MIN + 1) < tft.height()) {
				tft.setCursor(tft.getCursorX(), tft.getCursorY() + (TH_MIN + 1));
			}
			break;
			
		case 6:	// cursor right REQ for CRT
			if (tft.getCursorX() + (TW_MIN - 1) < tft.width()) {
				tft.setCursor(tft.getCursorX() + (TW_MIN + 1), tft.getCursorY());
			}
			break;
			
		case 7:	// cursor left REQ for CRT
			if (tft.getCursorX() - (TW_MIN - 1) >= 0) {
				tft.setCursor(tft.getCursorX() - (TW_MIN - 1), tft.getCursorY());
			}
			break;
			
		case 8:	// home cursor REQ for CRT
			tft.setCursor(0, 0);
			break;
			
		case 9:	// erase to end of screen REQ for CRT
			// TODO
			break;
			
		case 10:	// erase to end of line REQ for CRT
			// TODO
			break;
			
		case 11: // direct cursor address REQ for CRT
			{
				int16 y = (_gsx_intin[0] - 1) * (TH_MIN + 1);
				int16 x = (_gsx_intin[1] - 1) * (TW_MIN + 1);
				tft.setCursor(x, y);
			}
			break;
			
		case 12:	// direct cursor addressable text REQ for CRT
			{
				char text[256];
				uint16 clr = _gsx_colour_table[_gsx_text_colour];
				
				memset(text, 0, sizeof text);
				for (uint16 i = 0; i < _gsx_contrl[CONTRL_INTIN] && i < (sizeof text) - 1; ++i) {
					text[i] = (char)_gsx_intin[i];
				}
				if (_gsx_reverse_video) {
					// reverse video, black text
					tft.setTextColor(_gsx_colour_table[0], clr);
				} else {
					// normal video, transparent background
					tft.setTextColor(clr, clr);
				}
				tft.setTextSize(1);
				tft.print(text);
			}
			break;
			
		case 13:	// reverse video on
			_gsx_reverse_video = true;
			break;
			
		case 14:	// reverse video off
			_gsx_reverse_video = false;
			break;
			
		case 15:	// inquire current current cursor address REQ for CRT
			_gsx_contrl[CONTRL_INTOUT] = 2;
			_gsx_intout[0] = tft.getCursorY() / (TH_MIN + 1);
			_gsx_intout[1] = tft.getCursorX() / (TW_MIN + 1);
			break;
			
		case 16:	// inquire tablet status
			_gsx_contrl[CONTRL_INTOUT] = 1;
			_gsx_intout[0] = 0;	// tablet not available
			break;
			
		case 17:	// hardcopy
			// TODO
			break;
			
		case 18:	// place graphic cursor at location REQ for CRT
			_gsx_place_graphic_cursor(_gsx_int_ptsin[0], _gsx_int_ptsin[1]);
			break;
			
		case 19:	// remove last graphic cursor REQ for CRT
			_gsx_remove_graphic_cursor();
			break;
	}
}

// GSX 6: draw polyline REQ for CRT
void _gsx_draw_polyline() {
	int16 nPoints = _gsx_contrl[CONTRL_PTSIN] - 1;
	int16 x0, y0, x1, y1;
	uint16 clr = _gsx_colour_table[_gsx_line_colour];
	//uint16 style_mask = _gsx_line_style_mask[_gsx_line_style];
	
	for (int16 i = 0; i < nPoints * 2; i += 2) {
		x0 = _gsx_int_ptsin[i + 0];
		y0 = _gsx_int_ptsin[i + 1];
		x1 = _gsx_int_ptsin[i + 2];
		y1 = _gsx_int_ptsin[i + 3];
		switch (_gsx_line_style) {
			case LS_DASH:			// 1111111100000000
			case LS_DOT:			// 1110000011100000
			case LS_DASH_DOT:		// 1111111000111000
			case LS_LONG_DASH:	// 1111111111110000
			case LS_SOLID:
			default:
				// TODO: line style
				tft.drawLine(x0, y0, x1, y1, clr);
				break;
		}
	}
}

// GSX 7: plot a group of markers REQ for CRT
void _gsx_draw_polymarkers() {
	int16 nMarkers = _gsx_contrl[CONTRL_PTSIN];
	uint16 armLng = (_gsx_marker_height-1)/2;
	uint16 shortArmLng = (uint16)(armLng * 0.707);
	uint16 clr = _gsx_colour_table[_gsx_marker_colour];
	
	for (int16 i = 0; i < nMarkers; ++i) {
		int16 x = _gsx_int_ptsin[i + i + 0];
		int16 y = _gsx_int_ptsin[i + i + 1];
		
		switch (_gsx_marker_style) {
			
			case MS_DOT:
				tft.drawPixel(x, y, clr);
				break;
				
			case MS_PLUS:
				tft.drawLine(x - armLng, y, x + armLng, y, clr);
				tft.drawLine(x, y - armLng, x, y + armLng, clr);
				break;
				
			case MS_ASTERISK:
				tft.drawLine(x - armLng, y, x + armLng, y, clr);
				tft.drawLine(x, y - armLng, x, y + armLng, clr);
				tft.drawLine(x - shortArmLng, y - shortArmLng, x + shortArmLng, y + shortArmLng, clr);
				tft.drawLine(x - shortArmLng, y + shortArmLng, x + shortArmLng, y - shortArmLng, clr);
				break;
				
			case MS_CIRCLE:
				tft.drawCircle(x, y, armLng, clr);
				break;
				
			case MS_CROSS:
				tft.drawLine(x - armLng, y - armLng, x + armLng, y + armLng, clr);
				tft.drawLine(x - armLng, y + armLng, x + armLng, y - armLng, clr);
				break;
		}
	}
}

// GSX 8: draw text REQ for CRT
void _gsx_draw_text() {
	char text[256];
	int16 x = _gsx_int_ptsin[0];
	int16 y = _gsx_int_ptsin[1];
	
	memset(text, 0, sizeof text);
	for (uint16 i = 0; i < _gsx_contrl[CONTRL_INTIN] && i < (sizeof text) - 1; ++i ) {
		text[i] = (char)_gsx_intin[i];
	}
	tft.setTextColor(_gsx_colour_table[_gsx_text_colour], _gsx_colour_table[_gsx_text_colour]);
	tft.setTextSize(_gsx_text_size);
	switch (_gsx_text_direction) {
		case 900:	// rotation (0)
			x = tft.height() - y;
			y = _gsx_int_ptsin[0];
			tft.setRotation(2);
			tft.setCursor(x, y - _gsx_text_height);
			tft.print(text);
			tft.setRotation(3);
			break;
		case 1800:	// rotation (1)
			x = tft.width() - x;
			y = tft.height() - y;
			tft.setRotation(1);
			tft.setCursor(x, y - _gsx_text_height);
			tft.print(text);
			tft.setRotation(3);
			break;
		case 2700:	// rotation (2)
			x = y;
			y = tft.width() - _gsx_int_ptsin[0];
			tft.setRotation(0);
			tft.setCursor(x, y - _gsx_text_height);
			tft.print(text);
			tft.setRotation(3);
			break;
		case 0:		// normal rotation (3)
		default:
			// Shift character up so that x,y is the baseline, not the top
			// GFX: x,y is UL corner
			// GSX: x,y is LL corner (baseline)
			tft.setCursor(x, y - _gsx_text_height);
			tft.print(text);
			break;
	}
}

// GSX 9: draw a filled polygon REQ for CRT
void _gsx_draw_filled_polygon() {

	int16 nPoints = _gsx_contrl[CONTRL_PTSIN];
	int dy,dx;
	int temp,k;
	int xi[1024];
	float slope[MAX_PTSIN];
	int ymin=32767,ymax=0;
	
	_gsx_int_ptsin[nPoints * 2 + 0] = _gsx_int_ptsin[0];
	_gsx_int_ptsin[nPoints * 2 + 1] = _gsx_int_ptsin[1];
	for (int16 i = 0; i < nPoints * 2; i += 2) {
		int16 x0 = _gsx_int_ptsin[i + 0];
		int16 y0 = _gsx_int_ptsin[i + 1];
		int16 x1 = _gsx_int_ptsin[i + 2];
		int16 y1 = _gsx_int_ptsin[i + 3];
		tft.drawLine(x0, y0, x1, y1, _gsx_colour_table[_gsx_fill_colour]);

		if( y0 < ymin ) {
			ymin = y0;
		}
		if( y0 > ymax ) {
			ymax = y0;
		}
	}
	
	if (_gsx_fill_style != FS_HOLLOW) {

		for (int16 i = 0 ; i < nPoints ; ++i) {
			dy = _gsx_int_ptsin[i + i + 3] - _gsx_int_ptsin[i + i + 1];	// dy=y2-y1
			dx = _gsx_int_ptsin[i + i + 2] - _gsx_int_ptsin[i + i];		// dx=x2-x1

			if (dy == 0) {
				slope[i] = 1.0;
			}
			if (dx == 0) {
				slope[i] = 0.0;
			}
			if (dy != 0 && dx != 0) {			// calculate inverse slope
				slope[i] = (float)dx / dy;		// typecast to float
			}
		}

		for (int16 y = ymin; y <= ymax; ++y) {		// maximum range of y length resolution is 480 
			k = 0;
			for (int16 i = 0; i < nPoints; ++i) {
				int16 x0 = _gsx_int_ptsin[i + i + 0];
				int16 y0 = _gsx_int_ptsin[i + i + 1];
				int16 y1 = _gsx_int_ptsin[i + i + 3];
				if ((y0 <= y && y1 > y) ||	(y0 > y && y1 <= y)) {
					xi[k] = (int)(x0 + slope[i] * (y - y0));
					++k;
				}
			}

			for (int16 i = 0; i < k - 1; ++i) {		// Arrange x-intersections in order
				for (int16 j = 0; j < k - 1; ++j) {
					if (xi[j] > xi[j + 1]) {	// sort x intersection in xi[k] in order using swapping
					  temp = xi[j];
					  xi[j] = xi[j + 1];
					  xi[j + 1] = temp;
					}
				}
			}
			for (int16 i = 0; i < k; i += 2) {		// draw lines to fill polygon
				_gsx_filled_hline(xi[i], xi[i + 1], y);
			}
		}
	}
}

// GSX 10: draw bitmap REQ for CRT
void _gsx_draw_bitmap() {
	// TODO
}
			
// GSX 11: general drawing primitive REQ for CRT
void _gsx_drawing_primitive() {
	switch (_gsx_contrl[CONTROL_FUNCTIONID]) {
		case 1:	// bar REQ for CRT
			{
				int16 llx = _gsx_int_ptsin[0];
				int16 lly = _gsx_int_ptsin[1];
				int16 urx = _gsx_int_ptsin[2];
				int16 ury = _gsx_int_ptsin[3];
				tft.drawRect(
					llx, ury, 
					urx-llx+1, lly-ury+1,
					_gsx_colour_table[_gsx_fill_colour]);
				for (int16 y = ury + 1; y < lly; ++y) {
					_gsx_filled_hline(llx, urx, y);
				}
			}
			break;
		case 2:	// arc: drawn counter clockwise from arc_s to arc_e
			{
				int16 clr = _gsx_colour_table[_gsx_line_colour];
				int16 x0 = _gsx_int_ptsin[0];
				int16 y0 = _gsx_int_ptsin[1];
				int16 r = _gsx_int_ptsin[6];
				int16 arc_s = _gsx_intin[0];
				int16 arc_e = _gsx_intin[1];

				if (arc_s <= arc_e) {
					_gsx_draw_arc(x0, y0, r, arc_s, arc_e, clr);
				} else {
					_gsx_draw_arc(x0, y0, r, arc_s, 3600, clr);
					_gsx_draw_arc(x0, y0, r, 0, arc_e, clr);
				}
			}
			break;
		case 3:	// pie slice
			// TODO
			break;
		case 4:	// circle
			_gsx_filled_circle(
				_gsx_int_ptsin[0],	// x
				_gsx_int_ptsin[1],	// y
				_gsx_int_ptsin[4]);	// r
			break;
		case 5:	// print graphic characters (ruling characters)
			// TODO
			break;
	}
}

// GSX 12: set text size REQ for CRT
void _gsx_set_text_size() {
	_gsx_text_height = _gsx_ndc_to_dev(_gsx_ptsin[1], 0, _gsx_maxY);
	if (_gsx_text_height < TH_MIN ) {
		_gsx_text_height = TH_MIN;
	}
	if (_gsx_text_height < 2 * TH_MIN) {
		_gsx_text_size = 1;
	} else if (_gsx_text_height < 3 * TH_MIN) {
		_gsx_text_size = 2;
	} else if (_gsx_text_height < 4 * TH_MIN) {
		_gsx_text_size = 3;
	} else {
		_gsx_text_size = 4;
	} 
	_gsx_text_height = _gsx_text_size * TH_MIN;
	_gsx_text_width = _gsx_text_size * TW_MIN;
	
	_gsx_contrl[CONTRL_PTSOUT] = 2;
	_gsx_ptsout[0] = _gsx_text_width;
	_gsx_ptsout[1] = _gsx_text_height;
	_gsx_ptsout[2] = _gsx_text_width + _gsx_text_size;
	_gsx_ptsout[3] = _gsx_text_height + _gsx_text_size;
}
			
// GSX 13: set text direction
void _gsx_set_text_direction() {
	_gsx_text_direction = _gsx_intin[0];
	if (_gsx_text_direction > 450 && _gsx_text_direction <= 1350) {
		_gsx_text_direction = 900;
	} else if (_gsx_text_direction > 1350 && _gsx_text_direction <= 2250) {
		_gsx_text_direction = 1800;
	} else if (_gsx_text_direction > 2250 && _gsx_text_direction <= 3150) {
		_gsx_text_direction = 2700;
	} else {
		_gsx_text_direction = 0;
	}
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_text_direction;
}

// GSX 14: set colour index (palette registers) REQ for CRT
void _gsx_set_colour_palette() {
	uint16 clr_index = _gsx_intin[0];
	if (clr_index < _GSX_NUM_COLOURS) {
		_gsx_colour_table[clr_index] = _gsx_colour565(_gsx_intin[1], _gsx_intin[2], _gsx_intin[3]);
	}
}
			
// GSX 15: set line style REQ for CRT
void _gsx_set_line_style() {
	_gsx_line_style = _gsx_intin[0];
	if (_gsx_line_style < LS_SOLID || _gsx_line_style > LS_LONG_DASH) {
		_gsx_line_style = LS_SOLID;
	}
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_line_colour;
}

// GSX 16: set line width
void _gsx_set_line_width() {
	_gsx_line_width = _gsx_intin[0];
	if (_gsx_line_width < 1 ) {
		_gsx_line_width = 1;
	} else if (_gsx_line_width > LW_MAX) {
		_gsx_line_width = LW_MAX;
	}
	_gsx_contrl[CONTRL_PTSOUT] = 1;
	_gsx_ptsout[0] = _gsx_line_width;
	_gsx_ptsout[1] = 0;
}
			
// GSX 17: set line colour REQ for CRT
void _gsx_set_line_colour() {
	_gsx_line_colour = _gsx_intin[0];
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_line_colour;
}

// GSX 18: set marker type REQ for CRT
void _gsx_set_marker_style() {
	_gsx_marker_style = _gsx_intin[0];
	if (_gsx_marker_style < MS_DOT || _gsx_marker_style > MS_CROSS) {
		_gsx_marker_style = MS_ASTERISK;
	}
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_marker_style;
}

// GSX 19: set marker height
void _gsx_set_marker_height() {
	_gsx_marker_height = _gsx_ndc_to_dev(_gsx_ptsin[1], 0, _gsx_maxY);
	_gsx_contrl[CONTRL_PTSOUT] = 1;
	_gsx_ptsout[0] = 0;
	_gsx_ptsout[1] = _gsx_marker_height;
}

// GSX 20: set marker colour REQ for CRT
void _gsx_set_marker_colour() {
	_gsx_marker_colour = _gsx_intin[0];
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_marker_colour;
}

// GSX 21: set text font
void _gsx_set_text_font() {
	_gsx_text_font = _gsx_intin[0];
	_gsx_text_font = 1;					// TODO
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_text_font;
}
			
// GSX 22: set text colour REQ for CRT
void _gsx_set_text_colour() {
	_gsx_text_colour = _gsx_intin[0];
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_text_colour;
}

// GSX 23: set fill style
void _gsx_set_fill_style() {
	_gsx_fill_style = _gsx_intin[0];
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_fill_style;
}

// GSX 24: set fill index
void _gsx_set_fill_index() {
	_gsx_fill_index = _gsx_intin[0];
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_fill_index;
}

// GSX 25: set fill colour REQ for CRT
void _gsx_set_fill_colour() {
	_gsx_fill_colour = _gsx_intin[0];
}

// GSX 26: read colour palette REQ for CRT
void _gsx_read_colour_palette() {
	uint16 clr_index = _gsx_intin[0];
	if (clr_index >= _GSX_NUM_COLOURS) {
		clr_index = _GSX_NUM_COLOURS - 1;
	}
	uint16 clr = _gsx_colour_table[clr_index];
	_gsx_intout[0] = clr_index;
	_gsx_intout[1] = map((clr >> 11), 0, 0x1F, 0, 1000);
	_gsx_intout[2] = map(((clr >> 5) & 0x3F), 0, 0x3F, 0, 1000);
	_gsx_intout[3] = map((clr & 0x1F), 0, 0x1F, 0, 1000);
}

// GSX 27: read bitmap
void _gsx_read_bitmap() {
	// TODO
}

// GSX 28: read locator (eg tablet or mouse)
void _gsx_read_locator() {
	int16 x = _gsx_int_ptsin[0];
	int16 y = _gsx_int_ptsin[1];
   char ch;
   
	_gsx_place_graphic_cursor(x, y);
   while (true) {
		if (_chready()) {
			ch = _getch();
			_gsx_remove_graphic_cursor();
			switch (ch) {
				case 'X'-'@': y = min(y + 10, _gsx_maxY); break;
				case 'E'-'@': y = max(y - 10, 0); break;
				case 'D'-'@': x = min(x + 10, _gsx_maxX); break;
				case 'S'-'@': x = max(x - 10, 0); break;
				case 'Z'-'@': y = min(y + 1, _gsx_maxY); break;
				case 'W'-'@': y = max(y - 1, 0); break;
				case 'F'-'@': x = min(x + 1, _gsx_maxX); break;
				case 'A'-'@': x = max(x - 1, 0); break;
				default:
					_gsx_contrl[CONTRL_PTSOUT] = 1;
					_gsx_ptsout[0] = x;
					_gsx_ptsout[1] = _gsx_maxY - y;
					_gsx_contrl[CONTRL_INTOUT] = 1;
					_gsx_intout[0] = ch;
					return;
			}
			_gsx_place_graphic_cursor(x, y);
		}
	}
}

// GSX 29: read valuator
void _gsx_read_valuator() {
	// TODO
}
			
// GSX 30: read choice
void _gsx_read_choice() {
	// TODO
}
			
// GSX 31: read string
void _gsx_read_string() {
	// TODO
}
			
// GSX 32: set writing mode
void _gsx_set_writing_mode() {
	_gsx_writing_mode = _gsx_intin[0];
	if (_gsx_writing_mode < WM_REPLACE || _gsx_writing_mode > WM_ERASE ) {
		_gsx_writing_mode = WM_REPLACE;
	}
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_writing_mode;
}

// GSX 33: set input mode
void _gsx_set_input_mode() {
	uint16 lid = _gsx_intin[0];
	uint input_mode = _gsx_intin[1];
	
	if (lid >= LID_LOCATOR && lid <= LID_STRING) {
		if (input_mode == IM_REQUEST || input_mode == IM_SAMPLE) {
			_gsx_input_mode[lid] = input_mode;
		}
	}
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = input_mode;
}

//
// A utility routine used by _gsx_draw_filled_polygon and 
// _gsx_drawing_primitiveto draw a horizontal line between
// (x0,y) and (x1,y) using the current fill parameters
// (colour, style and style index).
//
void _gsx_filled_hline(int16 x0, int16 x1, int16 y) {

	uint16 clr = _gsx_colour_table[_gsx_fill_colour];
	
	switch (_gsx_fill_style) {
		
		case FS_HOLLOW:
			break;
			
		case FS_SOLID:
			tft.drawLine(x0, y, x1, y, clr);
			break;
			
		case FS_HALFTONE:
			switch (_gsx_fill_index) {
				
				case 1:	// hollow
					break;
					
				case 2:	// very dark
					for (int x = x0; x <= x1; ++x) {
						if (halftone1[y & 0x1F] & 1 << (x & 0x1F)) {
							tft.drawPixel(x, y, clr); 
						}
					}
					break;
					
				case 3:	// dark
					for (int x = x0; x <= x1; ++x) {
						if (halftone2[y & 0x1F] & 1 << (x & 0x1F)) {
							tft.drawPixel(x, y, clr); 
						}
					}
					break;
					
				case 4:	// light
					for (int x = x0; x <= x1; ++x) {
						if (halftone3[y & 0x1F] & 1 << (x & 0x1F)) {
							tft.drawPixel(x, y, clr); 
						}
					}
					break;
					
				case 5:	// very light
					for (int x = x0; x <= x1; ++x) {
						if (halftone4[y & 0x1F] & 1 << (x & 0x1F)) {
							tft.drawPixel(x, y, clr); 
						}
					}
					break;
					
				case 6:	// solid
					tft.drawLine(x0, y, x1, y, clr);
					break;
			}
			break;
			
		case FS_HATCH:
			switch (_gsx_fill_index) {
				
				case FSI_VERTICAL:
					for (int x = x0; x <= x1; ++x) {
						if( !(x & 3) ) {
							tft.drawPixel(x, y, clr);
						}
					}
					break;
					
				case FSI_HORIZONTAL:
					if( !(y & 3) ) {
						tft.drawLine(x0, y, x1, y, clr);
					}
					break;
					
				case FSI_PLUS45:
					for (int x = x0; x <= x1; ++x) {
						if (!((x+ y) % 5)) {
							tft.drawPixel(x, y, clr);
						}
					}
					break;
					
				case FSI_MINUS45:
					for (int x = x0; x <= x1; ++x) {
						if (!((x - y) % 5)) {
							tft.drawPixel(x, y, clr);
						}
					}
					break;
					
				case FSI_CROSS:
					if( !(y & 3) ) {
						tft.drawLine(x0, y, x1, y, clr);
					} else {
						for (int x = x0; x <= x1; ++x) {
							if( !(x & 3) ) {
								tft.drawPixel(x, y, clr);
							}
						}
					}
					break;
					
				case FSI_X:
					for (int x =x0; x <= x1; ++x) {
						if (!((x + y) % 5) || !((x - y) % 5)) {
							tft.drawPixel(x, y, clr);
						}
					}
					break;
			}
			break;
	}
}

// Draw a circle with the current fill characteristic.
// Basic code pinched from Adafruit GFX library.
// (Bresenham circle algorithm)
void _gsx_filled_circle(int16 x0, int16 y0, int16 r) {
	int16 f = 1 - r;
	int16 ddF_x = 1;
	int16 ddF_y = -2 * r;
	int16 x = 0;
	int16 y = r;
	uint16 clr = _gsx_colour_table[_gsx_fill_colour];

	tft.drawPixel(x0, y0 + r, clr);
	tft.drawPixel(x0, y0 - r, clr);
	tft.drawPixel(x0 + r, y0, clr);
	tft.drawPixel(x0 - r, y0, clr);
	_gsx_filled_hline(x0 - r, x0 + r, y0);
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		tft.drawPixel(x0 + x, y0 + y, clr);
		tft.drawPixel(x0 - x, y0 + y, clr);
		tft.drawPixel(x0 + x, y0 - y, clr);
		tft.drawPixel(x0 - x, y0 - y, clr);
		tft.drawPixel(x0 + y, y0 + x, clr);
		tft.drawPixel(x0 - y, y0 + x, clr);
		tft.drawPixel(x0 + y, y0 - x, clr);
		tft.drawPixel(x0 - y, y0 - x, clr);
		_gsx_filled_hline(x0 - x, x0 + x, y0 + y);
		_gsx_filled_hline(x0 - x, x0 + x, y0 - y);
		_gsx_filled_hline(x0 - y, x0 + y, y0 + x);
		_gsx_filled_hline(x0 - y, x0 + y, y0 - x);
	}
}

//
// 0 degrees is defined as 90 degrees clockwise from vertical.
// degrees increase in the counter clockwise direction.
//
void _gsx_draw_arc_point(int16 x0, int16 y0, int16 x, int16 y, int16 a0, int16 a1, int16 clr) {
   if (x <= y) {
      // if both x and y are >= 0, returned angle will be in [45.. 90] 
      int16 angle = 10*(int16)trunc(0.5+180*atan2(y,x)/M_PI);
      
      if (900 - angle >= a0 and 900 - angle <= a1) {		//   0 -  45
         tft.drawPixel(x0 + y, y0 - x, clr);
		}
      if (angle >= a0 and angle <= a1) {					//  45 -  90
         tft.drawPixel(x0 + x, y0 - y, clr);
		}
      if (1800 - angle >= a0 and 1800 - angle <= a1) {	//  90 - 135
         tft.drawPixel(x0 - x, y0 - y, clr);
		}
      if (angle + 900 >= a0 and angle + 900 <= a1) {		// 135 - 180
         tft.drawPixel(x0 - y, y0 - x, clr);
		}
      if (2700 - angle >= a0 and 2700 - angle <= a1) {	// 180 - 225
         tft.drawPixel(x0 - y, y0 + x, clr);
		}
      if (angle + 1800 >= a0 and angle + 1800 <= a1) {	// 225 - 270
         tft.drawPixel(x0 - x, y0 + y, clr);
		}
      if (3600 - angle >= a0 and 3600 - angle <= a1) {	// 270 - 315
         tft.drawPixel(x0 + x, y0 + y, clr);
		}
      if (angle + 2700 >= a0 and angle + 2700 <= a1) {	// 315 - 360
         tft.drawPixel(x0 + y, y0 + x, clr);
		}
	}
}
         
void _gsx_draw_arc(int16 x0, int16 y0, int16 r, int16 arc_s, int16 arc_e, int16 clr) {
	int16 f = 1 - r;
	int16 ddF_x = 1;
	int16 ddF_y = -2 * r;
	int16 x = 0;
	int16 y = r;

	_gsx_draw_arc_point(x0, y0, x, y, arc_s, arc_e, clr);
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		_gsx_draw_arc_point(x0, y0, x, y, arc_s, arc_e, clr);
	}
}

int16 _gsx_ndc_to_dev(long ndc, long dev_min, long dev_max) {
	return ndc * (dev_max - dev_min) / GSX_MAX_NDC + dev_min;
//	return map(ndc, 0, GSX_MAX_NDC, dev_min, dev_max);
}

int16 _gsx_dev_to_ndc(long dev, long dev_min, long dev_max) {
	return (dev - dev_min) * GSX_MAX_NDC / (dev_max - dev_min);
	//return map(dev, dev_min, dev_max, 0, GSX_MAX_NDC);
}

uint16 _gsx_colour565(uint16 r, uint16 g, uint16 b) {
	r = map(r, 0, 1000, 0, 255);
	g = map(g, 0, 1000, 0, 255);
	b = map(b, 0, 1000, 0, 255);
#if defined ILI9341
	return tft.color565(r, g, b);
#else
	return tft.Color565(r, g, b);
#endif
}

#if defined TFT_READBACK
static uint16 _gsx_gc_rect[GSX_GC_W * GSX_GC_W];
static int16 _gsx_gc_ulx, _gsx_gc_uly, _gsx_gc_w, _gsx_gc_h;
#endif

void _gsx_place_graphic_cursor(int16 x, int16 y) {
	_gsx_gc_x = x;
	_gsx_gc_y = y;
	uint16 clr = WHITE_565;

   _gsx_gc_ulx = x - GSX_GC_ARMLNG;
   _gsx_gc_uly = y - GSX_GC_ARMLNG;
   _gsx_gc_w = GSX_GC_W;
   _gsx_gc_h = GSX_GC_W;
   if (_gsx_gc_ulx < 0) {
		_gsx_gc_w += _gsx_gc_ulx;
		_gsx_gc_ulx = 0;
	}
	if (_gsx_gc_ulx + _gsx_gc_w > GSX_MAX_NDC) {
		_gsx_gc_w = GSX_MAX_NDC - _gsx_gc_ulx + 1;
	}
	if (_gsx_gc_uly < 0) {
		_gsx_gc_h += _gsx_gc_uly;
		_gsx_gc_uly = 0;
	}
	if (_gsx_gc_uly + _gsx_gc_h > GSX_MAX_NDC) {
		_gsx_gc_h = GSX_MAX_NDC - _gsx_gc_uly + 1;
	}
#if defined TFT_READBACK
	tft.readRect(_gsx_gc_ulx, _gsx_gc_uly, _gsx_gc_w, _gsx_gc_h, _gsx_gc_rect);
	uint8 r, g, b;
	tft.color565toRGB(tft.readPixel(x, y), r, g, b);
	if (r > 80 && g > 80 && b > 80) {
		clr = BLACK_565;
	} 

#endif

	tft.drawLine(x - GSX_GC_ARMLNG, y, x + GSX_GC_ARMLNG, y, clr);
	tft.drawLine(x, y - GSX_GC_ARMLNG, x, y + GSX_GC_ARMLNG, clr);
}

void _gsx_remove_graphic_cursor() {
#if defined TFT_READBACK
	tft.writeRect(_gsx_gc_ulx, _gsx_gc_uly, _gsx_gc_w, _gsx_gc_h, _gsx_gc_rect);
#else
	tft.drawLine(_gsx_gc_x - GSX_GC_ARMLNG, _gsx_gc_y, _gsx_gc_x + GSX_GC_ARMLNG, _gsx_gc_y, BLACK_565);
	tft.drawLine(_gsx_gc_x, _gsx_gc_y - GSX_GC_ARMLNG, _gsx_gc_x, _gsx_gc_y + GSX_GC_ARMLNG, BLACK_565);
#endif
}

uint16 _gdos(uint16 DE) {
	
	uint16*	gsx_params = (uint16*)_RamSysAddr(DE);

	_gsx_contrl = (int16*)_RamSysAddr(gsx_params[GSX_PB_CONTRL]);
	_gsx_intin  = (int16*)_RamSysAddr(gsx_params[GSX_PB_INTIN]);
	_gsx_ptsin  = (int16*)_RamSysAddr(gsx_params[GSX_PB_PTSIN]);
	_gsx_intout = (int16*)_RamSysAddr(gsx_params[GSX_PB_INTOUT]);
	_gsx_ptsout = (int16*)_RamSysAddr(gsx_params[GSX_PB_PTSOUT]);
	
	// any input points to convert from NDC to dev units?
	// convert to internal buffer (that's what CP/M-80's GSX.SYS did)
	if (_gsx_contrl[CONTRL_FUNCTION] != 1 && _gsx_contrl[CONTRL_PTSIN]) {
		int16 num_ptsin = _gsx_contrl[CONTRL_PTSIN] < MAX_PTSIN 
							  ? _gsx_contrl[CONTRL_PTSIN] : MAX_PTSIN;
		for (int16 i = 0; i < num_ptsin*2; i = i + 2) {
			_gsx_int_ptsin[i + 0] = _gsx_ndc_to_dev(_gsx_ptsin[i + 0], 0, _gsx_maxX);
			_gsx_int_ptsin[i + 1] = _gsx_ndc_to_dev(_gsx_ptsin[i + 1], _gsx_maxY, 0);
		}
	}
	
	_gsx_contrl[CONTRL_PTSOUT] = 0;		// assume no values returned ptsout
	_gsx_contrl[CONTRL_INTOUT] = 0;		// and nothing in intout
	switch (_gsx_contrl[CONTRL_FUNCTION]) {

		case 1:			// open workstation REQ for CRT
			_gsx_open_workstation();
			_gsx_maxX = _gsx_intout[0];
			_gsx_maxY = _gsx_intout[1];
			break;

		case 2:			// close workstation REQ for CRT
			_gsx_close_workstation();
			break;

		case 3:			// clear workstation REQ for CRT
			_gsx_clear_workstation();
			break;
			
		case 4:			// update workstation REQ for CRT
			_gsx_update_workstation();
			break;
			
		case 5:			// escape REQ for CRT
			_gsx_escape();
			break;
			
		case 6:			// draw polyline REQ for CRT
			_gsx_draw_polyline();
			break;
			
		case 7:			// plot a group of markers REQ for CRT
			_gsx_draw_polymarkers();
			break;
			
		case 8:			// draw text REQ for CRT
			_gsx_draw_text();
			break;
			
		case 9:			// draw a filled polygon REQ for CRT
			_gsx_draw_filled_polygon();
			break;
			
		case 10:			// draw bitmap REQ for CRT
			_gsx_draw_bitmap();
			break;
			
		case 11:			// general drawing primitive REQ for CRT
			_gsx_drawing_primitive();
			break;
			
		case 12:			// set text size REQ for CRT
			_gsx_set_text_size();
			break;
			
		case 13:			// set text direction
			_gsx_set_text_direction();
			break;
			
		case 14:			// set colour index (palette registers) REQ for CRT
			_gsx_set_colour_palette();
			break;
			
		case 15:			// set line style REQ for CRT
			_gsx_set_line_style();
			break;
			
		case 16:			// set line width
			_gsx_set_line_width();
			break;
			
		case 17:			// set line colour REQ for CRT
			_gsx_set_line_colour();
			break;
			
		case 18:			// set marker style REQ for CRT
			_gsx_set_marker_style();
			break;
			
		case 19:			// set marker height
			_gsx_set_marker_height();
			break;
			
		case 20:			// set marker colour REQ for CRT
			_gsx_set_marker_colour();
			break;
			
		case 21:			// set text font
			_gsx_set_text_font();
			break;
			
		case 22:			// set text colour REQ for CRT
			_gsx_set_text_colour();
			break;
			
		case 23:			// set fill style
			_gsx_set_fill_style();
			break;
			
		case 24:			// set fill index
			_gsx_set_fill_index();
			break;
			
		case 25:			// set fill colour REQ for CRT
			_gsx_set_fill_colour();
			break;
			
		case 26:			// read palette REQ for CRT
			_gsx_read_colour_palette();
			break;

		case 27:			// read bitmap
			_gsx_read_bitmap();
			break;
			
		case 28:			// read locator (eg tablet or mouse)
			_gsx_read_locator();
			break;
			
		case 29:			// read valuator
			_gsx_read_valuator();
			break;
			
		case 30:			// read choice
			_gsx_read_choice();
			break;
			
		case 31:			// read string
			_gsx_read_string();
			break;
			
		case 32:			// set writing mode
			_gsx_set_writing_mode();
			break;
			
		case 33:			// set input mode
			_gsx_set_input_mode();
			break;
			
		default:
			break;
	}

	for (int16 i = 0; i < _gsx_contrl[CONTRL_PTSOUT]*2; i = i + 2) {
		_gsx_ptsout[i + 0] = _gsx_dev_to_ndc(_gsx_ptsout[i + 0], 0, _gsx_maxX);
		_gsx_ptsout[i + 1] = _gsx_dev_to_ndc(_gsx_ptsout[i + 1], 0, _gsx_maxY);
	}
	return DE;			// CP/M-80's GSX.SYS doesn't return anything useful
							// but this can't hurt
}
#endif	// #ifndef GSX_H
