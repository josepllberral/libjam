#include <libjam.h>

//------------------------------------------------------------------------------
// Interruption Functions
//------------------------------------------------------------------------------
void jamEnableIRQ ()
{
	irqInit();
	irqEnable(IRQ_VBLANK);
	REG_IME = 1;
}

void jamGetKeys (char keys [10])
{
	keys[0] = (F_CTRLINPUT_UP_PRESSED)? 1 : 0;
	keys[1] = (F_CTRLINPUT_DOWN_PRESSED)? 1 : 0;
	keys[2] = (F_CTRLINPUT_LEFT_PRESSED)? 1 : 0;
	keys[3] = (F_CTRLINPUT_RIGHT_PRESSED)? 1 : 0;
	keys[4] = (F_CTRLINPUT_A_PRESSED)? 1 : 0;
	keys[5] = (F_CTRLINPUT_B_PRESSED)? 1 : 0;
	keys[6] = (F_CTRLINPUT_L_PRESSED)? 1 : 0;
	keys[7] = (F_CTRLINPUT_R_PRESSED)? 1 : 0;
	keys[8] = (F_CTRLINPUT_START_PRESSED)? 1 : 0;
	keys[9] = (F_CTRLINPUT_SELECT_PRESSED)? 1 : 0;

	return;
}

//------------------------------------------------------------------------------
// Graphic Functions MODE_0
//------------------------------------------------------------------------------
void InitJAM ()
{
	irqInit();
	irqEnable(IRQ_VBLANK);

	// setting NULL & 0 for the font address & size uses the default font
	consoleInit(	0,		// charbase
					4,		// mapbase
					0,		// background number
					NULL,	// font (should be a complete 1bit 8x8 ASCII font)
					0, 		// font size
					15		// 16 color palette
				);
	//consoleDemoInit();

	// set the screen colors, color 0 is the background color
	// the foreground color is index 1 of the selected 16 color palette
	BG_COLORS[0] = RGB8(58,110,165);
	BG_COLORS[241] = RGB5(31,31,31);

	SetMode(MODE_0 | BG0_ON);

	return;
}

void jamResetScreen (void)
{
	iprintf("\x1b[2J"); // Clean Text and Home Cursor at [0,0]
	return;
}

void jamDrawText (int y, int x, char* text)
{
	iprintf("\x1b[%d;%dH%s", x, y, text);
	return;
}

//------------------------------------------------------------------------------
// Graphic Functions MODE_3
//------------------------------------------------------------------------------
unsigned short* videoBuffer = (unsigned short*) MEM_BG;

void jamDrawPixel3 (int x, int y, unsigned short color)
{
	videoBuffer[y * 240 + x] = color;
	return;
}

void jamDrawCircle3 (int xc, int yc, int radius, int color)
{
	int x = 0;
	int y = radius;
	int p = 3 - 2 * radius;
	while (x <= y)
	{
		jamDrawPixel3 (xc + x, yc + y, color);
		jamDrawPixel3 (xc - x, yc + y, color);
		jamDrawPixel3 (xc - x, yc - y, color);
		jamDrawPixel3 (xc + x, yc - y, color);
		jamDrawPixel3 (xc + y, yc + x, color);
		jamDrawPixel3 (xc - y, yc + x, color);
		jamDrawPixel3 (xc - y, yc - x, color);
		jamDrawPixel3 (xc + y, yc - x, color);
		if (p < 0)	p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
	return;
}

void jamDrawBox3 (int left, int top, int right, int bottom, int color)
{
	int x, y;
	for (y = top; y < bottom; y++)
		for (x = left; x < right; x++)
			jamDrawPixel3(x, y, color);
	return;
}

void jamDrawBitmap3 (int xo, int yo, int width, int height, const unsigned short* bmp)
{
	int x, y;
	for (y = yo; y < yo + height; y++)
		for (x = xo; x < xo + width; x++)
			jamDrawPixel3(x, y, bmp[(y-yo) * width + (x-xo)]);
	return;
}

void jamColorBackground3 (int r, int g, int b)
{
	int x, y;
	for (x = 0; x < 240; x++)
		for (y = 0; y < 160; y++)
			jamDrawPixel3(x,y,RGB3(r,g,b));
	return;
}

//------------------------------------------------------------------------------
// Graphic Functions MODE_4
//------------------------------------------------------------------------------
unsigned short* paletteMem = (unsigned short*) MEM_PAL;
unsigned short* frontBuffer = (unsigned short*) MEM_BG;
unsigned short* backBuffer = (unsigned short*) (MEM_BG | MEM_BG_MODE4_BUFSIZE);
volatile unsigned short* scanlineCounter = (volatile unsigned short*) 0x04000006;

void jamSetPalette4(int r, int g, int b, int n)
{
	paletteMem[n] = RGB3(r,g,b);
	return;
}

void jamLoadPalette4(const unsigned short* pal)
{
	int n;
	for (n = 0; n < 256; n++)
		paletteMem[n] = pal[n];
	return;
}

void jamDrawPixel4(int x, int y, unsigned char color)
{
	unsigned short pixel;
	unsigned short offset = (y * 240 + x) >> 1;
	pixel = videoBuffer[offset];
	if (x & 1) videoBuffer[offset] = (color << 8) + (pixel & 0x00FF);
	else videoBuffer[offset] = (color) + (pixel & 0xFF00);
}

void jamDrawCircle4 (int xc, int yc, int radius, int color)
{
	int x = 0;
	int y = radius;
	int p = 3 - 2 * radius;
	while (x <= y)
	{
		jamDrawPixel4 (xc + x, yc + y, color);
		jamDrawPixel4 (xc - x, yc + y, color);
		jamDrawPixel4 (xc - x, yc - y, color);
		jamDrawPixel4 (xc + x, yc - y, color);
		jamDrawPixel4 (xc + y, yc + x, color);
		jamDrawPixel4 (xc - y, yc + x, color);
		jamDrawPixel4 (xc - y, yc - x, color);
		jamDrawPixel4 (xc + y, yc - x, color);
		if (p < 0)	p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
	return;
}

void jamDrawBox4(int left, int top, int right, int bottom, unsigned char color)
{
	int x, y;
	for (y = top; y < bottom; y++)
		for (x = left; x < right; x++)
			jamDrawPixel4(x,y,color);
}

void jamDrawBitmap4(const unsigned char* bmp)
{
	memcpy(videoBuffer, bmp, 38400);
	return;
}

void jamColorBackground4 (int r, int g, int b)
{
	int x, y;
	for (x = 0; x < 240; x++)
		for (y = 0; y < 160; y++)
			jamDrawPixel4(x,y,RGB3(r,g,b));
	return;
}

void jamFlipPage(void)
{
	if (REG_DISPCNT & BACKBUFFER)
	{
		REG_DISPCNT &= ~BACKBUFFER;
		videoBuffer = backBuffer;
	}
	else
	{
		REG_DISPCNT |= BACKBUFFER;
		videoBuffer = frontBuffer;
	}
	return;
}

void jamWaitVBlank(void)
{
	while (*scanlineCounter < 160);
	return;
}

//------------------------------------------------------------------------------
// Graphic Functions MODE_5
//------------------------------------------------------------------------------

void jamDrawPixel5(int x, int y, unsigned short c)
{
	videoBuffer[y * 160 + x] = c;
	return;
}

//------------------------------------------------------------------------------
// Font Functions
//------------------------------------------------------------------------------

void jamPressChar(int left, int top, char letter, unsigned short color, unsigned short bgcolor)
{
	int x, y;

	for(y = 0; y < 8; y++)
		for(x = 0; x < 8; x++)
			switch (GetMode() & 0x0F)
			{
				case MODE_3: jamDrawPixel3(left + x, top + y, bgcolor); break;
				case MODE_4: jamDrawPixel4(left + x, top + y, bgcolor); break;
				case MODE_5: jamDrawPixel5(left + x, top + y, bgcolor);
			}

	jamDrawChar(left,top,letter,color);
	return;
}


void jamDrawChar(int left, int top, char letter, unsigned short color)
{
	int x, y, draw;

	for(y = 0; y < 8; y++)
		for(x = 0; x < 8; x++)
		{
			draw = font[(letter-32) * 64 + y * 8 + x];
			if (draw)
			{
				switch (GetMode() & 0x0F)
				{
					case MODE_3: jamDrawPixel3(left + x, top + y, color); break;
					case MODE_4: jamDrawPixel4(left + x, top + y, color); break;
					case MODE_5: jamDrawPixel5(left + x, top + y, color);
				}
			}

		}
	return;
}

void jamPress(int left, int top, char *str, unsigned short color, unsigned short bgcolor)
{
	int pos = 0;
	while(*str)
	{
		jamPressChar(left + pos, top, *str++, color, bgcolor);
		pos += 8;
	}
	return;
}

void jamPrint(int left, int top, char *str, unsigned short color)
{
	int pos = 0;
	while(*str)
	{
		jamDrawChar(left + pos, top, *str++, color);
		pos += 8;
	}
	return;
}

