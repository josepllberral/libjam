//------------------------------------------------------------------------------
//
// Library JAM for GameBoy Advance
//
//------------------------------------------------------------------------------

#include <gba.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <font.h>

/* DevKitPro */
/*#include <gba_input.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
*/
// wine /opt/devkitpro/tools/gfx2gba.exe -fsrc -c32k source/prova.pcx
// wine /opt/devkitpro/tools/gfx2gba.exe -fsrc -pmode4.pal source/images/prova.pcx

#ifndef LIBJAM_H_
#define LIBJAM_H_

//------------------------------------------------------------------------------
// General
//------------------------------------------------------------------------------

/* External Macros*/
#ifndef REG_DISPCNT
	#define REG_DISPCNT *(unsigned long*)0x0400000
#endif

/* Constants */
#define MEM_IO 	0x04000000	// Register Base, all HW Registers are in here.
#define MEM_PAL 0x05000000	// Color Palette Register
#define MEM_BG 	0x06000000	// GBA Video RAM (in BG mode) [REG_DISPCNT]
/* Masks */
/* Functions */
/* Macros */
#define MULTIBOOT volatile const int __gba_multiboot = 1;

//------------------------------------------------------------------------------
// Video MODEs
//------------------------------------------------------------------------------

/* Masks */
#define MODE_3 0x3 		// mode 3 (240 x 160) 15 bit
#define MODE_4 0x4 		// mode 4 (240 x 160) 08 bit
#define MODE_5 0x5 		// mode 5 (160 x 128) 15 bit
#define BG2_ON 0x400 	// background 2 (0x400)
#define BACKBUFFER 0x10 // buffer bit modifier
/* Constants */
#define MEM_BG_MODE4_BUFSIZE    0x0000A000
#define MEM_BG_MODE5_BUFSIZE    0x0000A000
/* Functions */
#define RGB3(r,g,b) (unsigned short)(r + (g << 5 ) + (b << 10)) // RGB 15 bit
#define SetMode(mode) REG_DISPCNT = (mode)
#define GetMode() (unsigned short)(REG_DISPCNT)

//------------------------------------------------------------------------------
// Colors MODE_3
//------------------------------------------------------------------------------

#define WHITE 	0xFFFF
#define RED 	0x00FF
#define BLUE	0xEE00
#define CYAN	0xFF00
#define GREEN	0x0EE0
#define MAGENTA 0xF00F
#define BROWN	0x0D0D
#define BLACK	0x0000

//------------------------------------------------------------------------------
// IO 0x130 --	R_CTRLINPUT
// This register holds the values of the controller buttons [0 pressed/1 not p]
//------------------------------------------------------------------------------

#define R_CTRLINPUT		*(volatile u16 *)(MEM_IO + 0x130)
/* Masks */
#define V_CTRLINPUT_MSK  0xFC00
/* Functions */
#define F_CTRLINPUT_A_PRESSED        (~R_CTRLINPUT & 0x0001)
#define F_CTRLINPUT_B_PRESSED       ((~R_CTRLINPUT & 0x0002)>>1)
#define F_CTRLINPUT_SELECT_PRESSED  ((~R_CTRLINPUT & 0x0004)>>2)
#define F_CTRLINPUT_START_PRESSED   ((~R_CTRLINPUT & 0x0008)>>3)
#define F_CTRLINPUT_RIGHT_PRESSED   ((~R_CTRLINPUT & 0x0010)>>4)
#define F_CTRLINPUT_LEFT_PRESSED    ((~R_CTRLINPUT & 0x0020)>>5)
#define F_CTRLINPUT_UP_PRESSED      ((~R_CTRLINPUT & 0x0040)>>6)
#define F_CTRLINPUT_DOWN_PRESSED    ((~R_CTRLINPUT & 0x0080)>>7)
#define F_CTRLINPUT_R_PRESSED       ((~R_CTRLINPUT & 0x0100)>>8)
#define F_CTRLINPUT_L_PRESSED       ((~R_CTRLINPUT & 0x0200)>>9)

//------------------------------------------------------------------------------
// Interruption Functions
//------------------------------------------------------------------------------

void jamEnableIRQ ();
void jamGetKeys (char keys [10]);

//------------------------------------------------------------------------------
// Graphic Functions MODE_0
//------------------------------------------------------------------------------

void InitJAM ();
void jamResetScreen (void);
void jamDrawText (int y, int x, char* text);

//------------------------------------------------------------------------------
// Graphic Functions MODE_3
//------------------------------------------------------------------------------
unsigned short* videoBuffer; // Video Buffer Mode3

void jamDrawPixel3 (int x, int y, unsigned short color);
void jamDrawCircle3 (int xc, int yc, int radius, int color);
void jamDrawBox3 (int left, int top, int right, int bottom, int color);
void jamDrawBitmap3 (int xo, int yo, int width, int height, const unsigned short* bmp);
void jamColorBackground3 (int r, int g, int b);

//------------------------------------------------------------------------------
// Graphic Functions MODE_4
//------------------------------------------------------------------------------
unsigned short* paletteMem;
unsigned short* frontBuffer;
unsigned short* backBuffer;
volatile unsigned short* scanlineCounter;

void jamSetPalette4(int r, int g, int b, int n);
void jamLoadPalette4(const unsigned short* pal);
void jamDrawPixel4(int x, int y, unsigned char color);
void jamDrawCircle4 (int xc, int yc, int radius, int color);
void jamDrawBox4(int left, int top, int right, int bottom, unsigned char color);
void jamDrawBitmap4(const unsigned char* bmp);
void jamColorBackground4 (int r, int g, int b);
void jamFlipPage(void);
void jamWaitVBlank(void);

//------------------------------------------------------------------------------
// Graphic Functions MODE_5
//------------------------------------------------------------------------------

void jamDrawPixel5(int x, int y, unsigned short c);

//------------------------------------------------------------------------------
// Font Functions
//------------------------------------------------------------------------------

void jamPressChar(int left, int top, char letter, unsigned short color, unsigned short bgcolor);
void jamDrawChar(int left, int top, char letter, unsigned short color);
void jamPress(int left, int top, char *str, unsigned short color, unsigned short bgcolor);
void jamPrint(int left, int top, char *str, unsigned short color);

#endif
