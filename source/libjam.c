//------------------------------------------------------------------------------
//
// Library JAM Driver
//
//------------------------------------------------------------------------------

#include <libjam.h>

MULTIBOOT

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main (void)
{
	jamEnableIRQ();
	SetMode( MODE_3 | BG2_ON );

	char keys[10];

	char* clai = "PETECANDER 1 2 3 ! CLAI";
	int pos = 0;

	jamPrint(1,1,"PROVA GUAI!", RED);
	jamPrint(1,20,"()*+,-.0123456789:;<=>?@^_", GREEN);

	jamPrint(1,30,"ABCDEFGHIJKLMNOPQRTTUVWXYZ[/]", BLUE);

	while(*clai)
	{
		jamDrawChar(1 + pos, 80, *clai++, 0xBB + pos * 16);
		pos += 8;
	}

	while (1)
	{
		VBlankIntrWait();
		jamGetKeys(keys);

		jamPress(1,60,(keys[0]==0)?" ":"U",WHITE,BLACK);
		jamPress(1+(8*1),60,(keys[1]==0)?" ":"D",WHITE,BLACK);
		jamPress(1+(8*2),60,(keys[2]==0)?" ":"<",WHITE,BLACK);
		jamPress(1+(8*3),60,(keys[3]==0)?" ":">",WHITE,BLACK);
		jamPress(1+(8*4),60,(keys[4]==0)?" ":"A",WHITE,BLACK);
		jamPress(1+(8*5),60,(keys[5]==0)?" ":"B",WHITE,BLACK);
		jamPress(1+(8*6),60,(keys[6]==0)?" ":"L",WHITE,BLACK);
		jamPress(1+(8*7),60,(keys[7]==0)?" ":"R",WHITE,BLACK);
		jamPress(1+(8*8),60,(keys[8]==0)?"  ":"ST",WHITE,BLACK);
		jamPress(1+(8*10),60,(keys[9]==0)?"  ":"SL",WHITE,BLACK);
	}
	return 0;
}


