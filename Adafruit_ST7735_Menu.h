/*
  The MIT License (MIT)

  library writen by Kris Kasprzak
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  On a personal note, if you develop an application or product using this library 
  and make millions of dollars, I'm happy for you!

	rev		date			author				change
	1.0		1/2022			kasprzak			initial code
	2.0   1/2022      kasprzak      added touch support

	// Website for generating icons
	// https://javl.github.io/image2cpp/


*/

#ifndef ADAFRUITST7735_MENU_H
#define ADAFRUITST7735_MENU_H

#if ARDUINO >= 100
	 #include "Arduino.h"
	 #include "Print.h"
#else
	
#endif

#ifdef __cplusplus
	
#endif

#include <Adafruit_ST7735.h>  
#include "Adafruit_GFX.h"


#define MAX_OPT 5				// max elements in a menu, increase as needed
#define MAX_CHAR_LEN 10			// max chars in menus, increase as needed
#define MENU_C_DKGREY 0x4A49	// used for disable color, method to change
#define EXIT_TEXT "Exit"		// default text for menu exit text, change here or use a method
#define DEBOUNCE_TIME 50

#define IOX 0
#define IOY 25

#define TOX 0
#define TOY 25
#define MM  0

class  EditMenu {
		
public:

	EditMenu(Adafruit_ST7735 *Display);

	void init(uint16_t TextColor, uint16_t BackgroundColor, 
		uint16_t HighlightTextColor, uint16_t HighlightColor,
		uint16_t SelectedTextColor, uint16_t SelectedColor,
		uint16_t MenuColumn, uint16_t ItemRowHeight,uint16_t MaxRow,
		const char *TitleText, const GFXfont &ItemFont, const GFXfont &TitleFont);

	int addNI(const char *ItemText, float Data, float LowLimit, float HighLimit, 
		float Increment, byte DecimalPlaces = 0, const char **ItemMenuText = NULL);

	int selectRow();

	void draw();

	void MoveUp();

	void MoveDown();
	
	void setIncrement(int ItemID, float Value);

	void setItemValue(int ItemID, float ItemValue);

	bool getEnableState(int ID);
	
	void drawRow(int ID);
		
	float value[MAX_OPT];

	int item;

private:

	void drawHeader(bool hl);

	void up();

	void down();

	void incrementUp();
	
	void incrementDown();

	void drawItems();

	Adafruit_ST7735 *d;
	char itemlabel[MAX_OPT][MAX_CHAR_LEN];
	char ttx[MAX_CHAR_LEN];
	char etx[MAX_CHAR_LEN]; 
	GFXfont itemf;
	GFXfont titlef;
	uint16_t itc, ibc, ihtc, ihbc, istc, isbc;	// item variables
	uint16_t tbl, tbt, tbw, tbh;	// title variables
	// margins
	uint16_t imr, isx, itx, isy, irh, irw;
	int i;
	int totalID;
	int MaxRow;
	int currentID;
	int cr;
	int sr, pr;
	uint16_t col;
	float data[MAX_OPT];
	float low[MAX_OPT];
	float high[MAX_OPT];
	float inc[MAX_OPT];
	byte dec[MAX_OPT];
	char **itemtext[MAX_OPT];
	bool rowselected = false;
	bool haslist[MAX_OPT];
	bool drawTitleFlag = true;
	bool redraw = false;
	uint16_t temptColor = 0;
	bool redrawh;

};


class  ItemMenu {

	
public:
	ItemMenu(Adafruit_ST7735 *Display);
	
	void init(uint16_t TextColor, uint16_t BackgroundColor,
		uint16_t HighlightTextColor, uint16_t HighlightColor, 
		uint16_t ItemRowHeight,uint16_t MaxRow,
		const char *TitleText, const GFXfont &ItemFont, const GFXfont &TitleFont);
		
	int addNI(const char *ItemLabel);

	void draw();
	
	void MoveUp();

	void MoveDown();

	int selectRow();

	bool getEnableState(int ID);

	void drawRow(int ID);

	float value[MAX_OPT];

	int item;

private:

	void drawHeader(bool hl);
void drawItems();
	
	
	Adafruit_ST7735 *d;

	char itemlabel[MAX_OPT][MAX_CHAR_LEN];
	char ttx[MAX_CHAR_LEN];
	GFXfont itemf;
	GFXfont titlef;
	uint16_t bkgr, isx, itx, isy, irh, itc, ibc, ihbc, ihtc, isc, imr, irw;	// item variables
	uint16_t tbl, tbt, tbw, tbh, ttc, tfc, di;	// title variables
	uint16_t temptColor;
	int i;
	int totalID;
	int MaxRow;
	int currentID;
	int cr;
	int sr, pr;
	bool rowselected = false;
	bool drawTitleFlag = true;
	bool redraw = false;

};


#endif
