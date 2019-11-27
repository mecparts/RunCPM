#ifndef GSX_H
#define GSX_H

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

uint16 _gsx_line_style_mask[] = {
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

#define	MAX_PTSIN		75		// This is what CP/M-80 had (all the room
										// it had in GDOS for internal buffer).
										// If needed, we can expand arbitrarly.
#define	GSX_MIN_NDC	0
#define	GSX_MAX_NDC	32767

uint16*	_gsx_params[GSX_PB_MAX];
uint16*	_gsx_contrl;
uint16*	_gsx_intin;
uint16*	_gsx_ptsin;
uint16*	_gsx_intout;
uint16*	_gsx_ptsout;

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

uint16	_gsx_int_ptsin[MAX_PTSIN * 2];

uint16	_gsx_maxX, _gsx_maxY;
bool		_gsx_reverse_video;

#if defined __IMXRT1062__ // Teensy 4.0

#include <Adafruit_GFX.h>    // Core graphics library
#include <ST7735_t3.h> // Hardware-specific library

#define TFT_RST   8  // RST can use any pin
#define TFT_DC    9  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define TFT_CS    2  //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define TFT_BL    7	// LCD backlight

ST7735_t3 tft = ST7735_t3(TFT_CS, TFT_DC, TFT_RST);

void _gsx_clear_workstation();
void _gsx_filled_hline( int16 x0, int16 x1, int16 y);

#define TH_MIN	7
#define TW_MIN 5

#define LW_MAX	1

uint16	_gsx_colour_table[8] = {
	ST7735_BLACK,
	ST7735_RED,
	ST7735_GREEN,
	ST7735_BLUE,
	ST7735_CYAN,
	ST7735_YELLOW,
	ST7735_MAGENTA,
	ST7735_WHITE
};
	
void _gsx_init() {
   tft.initR(INITR_BLACKTAB);
   pinMode(TFT_BL,OUTPUT);
   tft.setRotation(3);
   digitalWrite(TFT_BL,LOW);
}

// GSX 1: open workstation REQ for CRT
void _gsx_open_workstation() {
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
	_gsx_ptsout[3] = _gsx_text_height;	// maximum character height
	_gsx_ptsout[4] = 1;	// minimum line width
	_gsx_ptsout[5] = 0;
	_gsx_ptsout[6] = 1;	// maximum line width
	_gsx_ptsout[7] = 0;
	_gsx_ptsout[8] = 0;
	_gsx_ptsout[9] = _gsx_marker_height;	// minimum marker height
	_gsx_ptsout[10] = 0;
	_gsx_ptsout[11] = 256;	// maximum marker height.
	
	_gsx_contrl[CONTRL_INTOUT] = 45;	// # integers in _gsx_intout

	_gsx_intout[OWS_INTOUT_MAX_WIDTH] = tft.width()-1;		// Screen width, device units
	_gsx_intout[OWS_INTOUT_MAX_HEIGHT] = tft.height()-1;	// Screen height, device units
	_gsx_intout[OWS_INTOUT_COORD_FLAGS] = 1; 					// 0 if device is capable of
																			// continuous scaling (printer)
																			// 1 if not (CRT)
	_gsx_intout[OWS_INTOUT_PIXEL_WIDTH] = 223;				// Width of a pixel, in thousandths of a millimetre.
	_gsx_intout[OWS_INTOUT_PIXEL_HEIGHT] = 223;				// Height of a pixel, in thousandths of a millimetre.
 	_gsx_intout[5] = 4;	// Number of character sizes, 0 for continuous sizing.
	_gsx_intout[6] = 5;	// Number of line styles.
	_gsx_intout[7] = 1;	// Number of line widths.
	_gsx_intout[8] = 5;	// Number of marker styles.
	_gsx_intout[9] = 256;	// Number of marker sizes.
	_gsx_intout[10] = 1;	// Number of fonts.
	_gsx_intout[11] = 6;	// Number of patterns.
	_gsx_intout[12] = 6;	// Number of hatch styles.
	_gsx_intout[13] = 16;	//	Number of colours displayable at once.

	_gsx_intout[14] = 2;	// Number of General Drawing Primitives
	_gsx_intout[15] = 1;	// General Drawing Primitive numbers.
	_gsx_intout[16] = 4;	//-1 => End of list
	_gsx_intout[17] = -1;// 1 => Bar
								// 2 => Arc
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
	_gsx_intout[36] = 0; //	0 if text rotation is not possible, 1 if it is.
	_gsx_intout[37] = 1; //	0 if filled areas are not possible, 1 if they are.
	_gsx_intout[38] = 0; //	0 if cannot read cell array, 1 if can.
	_gsx_intout[39] = 0; //	Number of colours in the palette.
								// 0 => More than 32767
								// 2 => Black and white
								// >2 => number of colours available
	_gsx_intout[40] = 0;	//	Number of locator devices (mice, tablets, lightpens)
	_gsx_intout[41] = 0;	//	Number of valuator devices
	_gsx_intout[42] = 0;	//	Number of choice devices
	_gsx_intout[43] = 0;	// Number of string devices
	_gsx_intout[44] = 0;	//	Workstation type
								// 0 => Output only
								// 1 => Input only
								// 2 => Input and output
								// 3 => Segment storage
								// 4 => GKS metafile output.

	_gsx_clear_workstation();	
   digitalWrite(TFT_BL,HIGH);
}

// GSX 2: close workstation REQ for CRT
void _gsx_close_workstation() {
	_gsx_clear_workstation();	
   digitalWrite(TFT_BL,LOW);
}

// GSX 3: clear workstation REQ for CRT
void _gsx_clear_workstation() {
   tft.fillScreen(ST7735_BLACK);
   tft.setCursor(0, 0);
   tft.setTextColor(ST7735_WHITE);
}

// GSX 4: update workstation REQ for CRT
void _gsx_update_workstation() {
}

// GSX 5:  escape REQ for CRT
void _gsx_escape() {
	switch (_gsx_contrl[CONTROL_FUNCTIONID]) {
		
		case 1:	// inquire addressable character cells REQ for CRT
			_gsx_contrl[CONTRL_INTOUT] = 2;
			_gsx_intout[0] = tft.height()/(TH_MIN+1);
			_gsx_intout[1] = tft.width()/(TW_MIN+1);
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
				uint16 x = (_gsx_intin[0] - 1) * (TW_MIN + 1);
				uint16 y = (_gsx_intin[1] - 1) * (TH_MIN + 1);
				tft.setCursor(x, y);
			}
			break;
			
		case 12:	// direct cursor addressable text REQ for CRT
			{
				char text[256];
				
				memset(text, 0, sizeof text);
				for (uint16 i = 0; i < _gsx_contrl[CONTRL_INTIN] && i < (sizeof text) - 1; ++i ) {
					text[i] = (char)_gsx_intin[i];
				}
				if (reverse_video) {
					// reverse video, black text
					tft.setTextColor(ST7735_BLACK, _gsx_colour_table[_gsx_text_colour]);
				} else {
					// normal video, transparent background
					tft.setTextColor(_gsx_colour_table[_gsx_text_colour], _gsx_colour_table[_gsx_text_colour]);
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
			// TODO
			break;
			
		case 19:	// remove last graphic cursor REQ for CRT
			// TODO
			break;
	}
}

// GSX 6: draw polyline REQ for CRT
void _gsx_draw_polyline() {
	uint16 nPoints = _gsx_contrl[CONTRL_PTSIN] - 1;
	uint16 x0, y0, x1, y1;
	for (uint16 i = 0; i < nPoints * 2; i += 2) {
		x0 = _gsx_int_ptsin[i + 0];
		y0 = _gsx_int_ptsin[i + 1];
		x1 = _gsx_int_ptsin[i + 2];
		y1 = _gsx_int_ptsin[i + 3];
		// TODO: line style
		tft.drawLine(x0, y0, x1, y1, _gsx_colour_table[_gsx_line_colour]);
	}
}

// GSX 7: plot a group of markers REQ for CRT
void _gsx_draw_polymarkers() {
	uint16 nMarkers = _gsx_contrl[CONTRL_PTSIN];
	uint16 armLength = (_gsx_marker_height-1)/2;
	uint16 clr = _gsx_colour_table[_gsx_marker_colour];
	
	for (uint16 i = 0; i < nMarkers; ++i) {
		uint16 x = _gsx_int_ptsin[i + i + 0];
		uint16 y = _gsx_int_ptsin[i + i + 1];
		
		switch (_gsx_marker_style) {
			
			case MS_DOT:
				tft.drawPixel(x, y, clr);
				break;
				
			case MS_PLUS:
				tft.drawLine(x - armLength, y, x + armLength, y, clr);
				tft.drawLine(x, y - armLength, x, y + armLength, clr);
				break;
				
			case MS_ASTERISK:
				break;
				
			case MS_CIRCLE:
				tft.drawCircle(x, y, armLength, clr);
				break;
				
			case MS_CROSS:
				tft.drawLine(x - armLength, y - armLength, x + armLength, y + armLength, clr);
				tft.drawLine(x - armLength, y + armLength, x + armLength, y - armLength, clr);
				break;
		}
	}
}

// GSX 8: draw text REQ for CRT
void _gsx_draw_text() {
	char text[256];
	
	memset(text, 0, sizeof text);
	for (uint16 i = 0; i < _gsx_contrl[CONTRL_INTIN] && i < (sizeof text) - 1; ++i ) {
		text[i] = (char)_gsx_intin[i];
	}
	tft.setCursor(_gsx_int_ptsin[0], _gsx_int_ptsin[1]);
	tft.setTextColor(_gsx_colour_table[_gsx_text_colour], _gsx_colour_table[_gsx_text_colour]);
	tft.setTextSize(_gsx_text_size);
	tft.print(text);
}

// GSX 9: draw a filled polygon REQ for CRT
void _gsx_draw_filled_polygon() {

	uint16 nPoints = _gsx_contrl[CONTRL_PTSIN] - 1;
	int dy,dx;
	int temp,k;
	int xi[1024];
	float slope[MAX_PTSIN];
	int ymin=32767,ymax=0;
	
	for (uint16 i = 0; i < nPoints * 2; i += 2) {
		uint16 x0 = _gsx_int_ptsin[i + 0];
		uint16 y0 = _gsx_int_ptsin[i + 1];
		uint16 x1 = _gsx_int_ptsin[i + 2];
		uint16 y1 = _gsx_int_ptsin[i + 3];
		tft.drawLine(x0, y0, x1, y1, _gsx_colour_table[_gsx_fill_colour]);

		if( y0 < ymin ) {
			ymin = y0;
		}
		if( y0 > ymax ) {
			ymax = y0;
		}
	}
	
	if (_gsx_fill_style != FS_HOLLOW) {

		for (uint16 i = 0 ; i < nPoints ; ++i) {
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

		for (uint16 y = ymin; y <= ymax; ++y) {		// maximum range of y length resolution is 480 
			k = 0;
			for (uint16 i = 0; i < nPoints; ++i) {
				uint16 x0 = _gsx_int_ptsin[i + i + 0];
				uint16 y0 = _gsx_int_ptsin[i + i + 1];
				uint16 y1 = _gsx_int_ptsin[i + i + 3];
				if ((y0 <= y && y1 > y) ||	(y0 > y && y1 <= y)) {
					xi[k] = (int)(x0 + slope[i] * (y - y0));
					++k;
				}
			}

			for (uint16 i = 0; i < k - 1; ++i) {		// Arrange x-intersections in order
				for (uint16 j = 0; j < k - 1; ++j) {
					if (xi[j] > xi[j + 1]) {	// sort x intersection in xi[k] in order using swapping
					  temp = xi[j];
					  xi[j] = xi[j + 1];
					  xi[j + 1] = temp;
					}
				}
			}
			for (uint16 i = 0; i < k; i += 2) {		// draw lines to fill polygon
				_gsx_filled_hline(xi[i], xi[i + 1], y);
			}
		}
	}
}

//
// A utility routine used by _gsx_draw_filled_polygon and 
// _gsx_drawing_primitiveto draw a horizontal line between
// (x0,y) and (x1,y) using the current fill parameters
// (colour, style and style index).
//
void _gsx_filled_hline(int16 x0, int16 x1, int16 y) {

	switch (_gsx_fill_style) {
		
		case FS_HOLLOW:
			break;
			
		case FS_SOLID:
			tft.drawLine(x0, y, x1, y, _gsx_colour_table[_gsx_fill_colour]);
			break;
			
		case FS_HALFTONE:
			switch (_gsx_fill_index) {
				case 0:	// hollow
					break;
				case 1:	// very dark
					break;
				case 2:	// dark
					break;
				case 3:	// light
					break;
				case 4:	// very light
					break;
				case 5:	// solid
					tft.drawLine(x0, y, x1, y, _gsx_colour_table[_gsx_fill_colour]);
					break;
			}
			break;
			
		case FS_HATCH:
			switch (_gsx_fill_index) {
				
				case FSI_VERTICAL:
					for (int x=x0; x<=x1; ++x) {
						if( !(x & 3) ) {
							tft.drawPixel(x,y,_gsx_colour_table[_gsx_fill_colour]);
						}
					}
					break;
					
				case FSI_HORIZONTAL:
					if( !(y & 3) ) {
						tft.drawLine(x0, y, x1, y, _gsx_colour_table[_gsx_fill_colour]);
					}
					break;
					
				case FSI_PLUS45:
					for (int x=x0; x<=x1; ++x) {
						if (!((x+y)%5)) {
							tft.drawPixel(x,y,_gsx_colour_table[_gsx_fill_colour]);
						}
					}
					break;
					
				case FSI_MINUS45:
					for (int x=x0; x<=x1; ++x) {
						if (!((x-y)%5)) {
							tft.drawPixel(x,y,_gsx_colour_table[_gsx_fill_colour]);
						}
					}
					break;
					
				case FSI_CROSS:
					if( !(y & 3) ) {
						tft.drawLine(x0, y, x1, y, _gsx_colour_table[_gsx_fill_colour]);
					} else {
						for (int x=x0; x<=x1; ++x) {
							if( !(x & 3) ) {
								tft.drawPixel(x,y,_gsx_colour_table[_gsx_fill_colour]);
							}
						}
					}
					break;
					
				case FSI_X:
					for (int x=x0; x<=x1; ++x) {
						if (!((x+y)%5) || !((x-y)%5)) {
							tft.drawPixel(x,y,_gsx_colour_table[_gsx_fill_colour]);
						}
					}
					break;
			}
			break;
	}
}

// Draw a circle with the current fill characteristic.
// Basic code pinched from Adafruit GFX library.
void _gsx_filled_circle(uint16 x0, uint16 y0, uint16 r) {
	int16 f = 1 - r;
	int16 ddF_x = 1;
	int16 ddF_y = -2 * r;
	int16 x = 0;
	int16 y = r;

	tft.startWrite();
	tft.drawPixel(x0, y0+r, _gsx_colour_table[_gsx_fill_colour]);
	tft.drawPixel(x0, y0-r, _gsx_colour_table[_gsx_fill_colour]);
	tft.drawPixel(x0+r, y0, _gsx_colour_table[_gsx_fill_colour]);
	tft.drawPixel(x0-r, y0, _gsx_colour_table[_gsx_fill_colour]);
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

		tft.drawPixel(x0 + x, y0 + y, _gsx_colour_table[_gsx_fill_colour]);
		tft.drawPixel(x0 - x, y0 + y, _gsx_colour_table[_gsx_fill_colour]);
		tft.drawPixel(x0 + x, y0 - y, _gsx_colour_table[_gsx_fill_colour]);
		tft.drawPixel(x0 - x, y0 - y, _gsx_colour_table[_gsx_fill_colour]);
		tft.drawPixel(x0 + y, y0 + x, _gsx_colour_table[_gsx_fill_colour]);
		tft.drawPixel(x0 - y, y0 + x, _gsx_colour_table[_gsx_fill_colour]);
		tft.drawPixel(x0 + y, y0 - x, _gsx_colour_table[_gsx_fill_colour]);
		tft.drawPixel(x0 - y, y0 - x, _gsx_colour_table[_gsx_fill_colour]);
		_gsx_filled_hline(x0 - x, x0 + x, y0 + y);
		_gsx_filled_hline(x0 - x, x0 + x, y0 - y);
		_gsx_filled_hline(x0 - y, x0 + y, y0 + x);
		_gsx_filled_hline(x0 - y, x0 + y, y0 - x);
	}
	tft.endWrite();
}

// GSX 10: draw sprite REQ for CRT
void _gsx_draw_sprite() {
	// TODO
}
			
// GSX 11: general drawing primitive REQ for CRT
void _gsx_drawing_primitive() {
	switch (_gsx_contrl[CONTROL_FUNCTIONID]) {
		case 1:	// bar REQ for CRT
			{
				uint16 llx = _gsx_int_ptsin[0];
				uint16 lly = _gsx_int_ptsin[1];
				uint16 urx = _gsx_int_ptsin[2];
				uint16 ury = _gsx_int_ptsin[3];
				tft.drawRect(
					llx, ury, 
					urx-llx+1, lly-ury+1,
					_gsx_colour_table[_gsx_fill_colour]);
				for (uint16 y = ury + 1; y < lly; ++y) {
					_gsx_filled_hline(llx, urx, y);
				}
			}
			break;
		case 2:	// arc
			// TODO
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
	_gsx_text_height = _gsx_int_ptsin[1];
	if (_gsx_text_height < TH_MIN) {
		_gsx_text_height = TH_MIN;
	}
	if (_gsx_text_height < 2*TH_MIN) {
		_gsx_text_height = TH_MIN;
		_gsx_text_width = TW_MIN;
		_gsx_text_size = 1;
	} else if (_gsx_text_height < 3*TH_MIN) {
		_gsx_text_height = 2*TH_MIN;
		_gsx_text_width = 2*TW_MIN;
		_gsx_text_size = 2;
	} else if (_gsx_text_height < 4*TH_MIN) {
		_gsx_text_height = 3*TH_MIN;
		_gsx_text_width = 3*TW_MIN;
		_gsx_text_size = 3;
	} else {
		_gsx_text_height = 4*TH_MIN;
		_gsx_text_width = 4*TW_MIN;
		_gsx_text_size = 4;
	} 
	
	_gsx_contrl[CONTRL_PTSOUT] = 2;
	_gsx_ptsout[0] = _gsx_text_width;
	_gsx_ptsout[1] = _gsx_text_height;
	_gsx_ptsout[2] = _gsx_text_width + _gsx_text_size;
	_gsx_ptsout[3] = _gsx_text_height + _gsx_text_size;
}
			
// GSX 13: set text direction
void _gsx_set_text_direction() {
	_gsx_text_direction = _gsx_intin[0];
	_gsx_text_direction = 0;			// TODO
	_gsx_contrl[CONTRL_INTOUT] = 1;
	_gsx_intout[0] = _gsx_line_colour;
}
			
// GSX 14: set colour index (palette registers) REQ for CRT
void _gsx_set_colour_palette() {
	// TODO
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
	_gsx_marker_height = _gsx_int_ptsin[2];
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
	// TODO
}

// GSX 27: read sprite
void _gsx_read_sprite() {
	// TODO
}

// GSX 28: read locator (eg tablet or mouse)
void _gsx_read_locator() {
	// TODO
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

uint16 _gsx_ndc_to_dev(uint16 ndc, uint16 dev_min,uint16 dev_max) {
	return map(ndc, GSX_MIN_NDC, GSX_MAX_NDC, dev_min, dev_max);
}

uint16 _gsx_dev_to_ndc(uint16 dev, uint16 dev_min, uint16 dev_max) {
	return map(dev, dev_min, dev_max, GSX_MIN_NDC, GSX_MAX_NDC);
}
#endif // Teensy 4.0

uint16 _gdos(uint16 DE) {
	
	uint16*	gsx_params = (uint16*)_RamSysAddr(DE);

	_gsx_contrl = (uint16*)_RamSysAddr(gsx_params[GSX_PB_CONTRL]);
	_gsx_intin  = (uint16*)_RamSysAddr(gsx_params[GSX_PB_INTIN]);
	_gsx_ptsin  = (uint16*)_RamSysAddr(gsx_params[GSX_PB_PTSIN]);
	_gsx_intout = (uint16*)_RamSysAddr(gsx_params[GSX_PB_INTOUT]);
	_gsx_ptsout = (uint16*)_RamSysAddr(gsx_params[GSX_PB_PTSOUT]);
	
	// any input points to convert from NDC to dev units?
	// convert to internal buffer (that's what CP/M-80's GSX.SYS did)
	if (_gsx_contrl[CONTRL_FUNCTION] != 1 && _gsx_contrl[CONTRL_PTSIN]) {
		uint16 num_ptsin = _gsx_contrl[CONTRL_PTSIN] < MAX_PTSIN 
									? _gsx_contrl[CONTRL_PTSIN] : MAX_PTSIN;
		for (uint16 i = 0; i < num_ptsin*2; i = i + 2) {
			_gsx_int_ptsin[i + 0] = _gsx_ndc_to_dev(_gsx_ptsin[i + 0],0,_gsx_maxX);
			_gsx_int_ptsin[i + 1] = _gsx_ndc_to_dev(_gsx_ptsin[i + 1],_gsx_maxY,0);
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
			
		case 10:			// draw sprite REQ for CRT
			_gsx_draw_sprite();
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

		case 27:			// read sprite
			_gsx_read_sprite();
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

	return DE;			// CP/M-80's GSX.SYS doesn't return anything useful
							// but this can't hurt
}
#endif	// #ifndef GSX_H
