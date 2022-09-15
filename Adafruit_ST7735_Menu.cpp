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

  rev   date      author        change
  1.0   1/2022      kasprzak      initial code
  2.0   1/2022      kasprzak      added touch support

*/

#include "Adafruit_ST7735_Menu.h"
//#include <Adafruit_ST7735.h>     // fast display driver lib

EditMenu::EditMenu(Adafruit_ST7735 *Display) {
  d = Display;
}

void EditMenu::init(uint16_t TextColor, uint16_t BackgroundColor,
                    uint16_t HighlightTextColor, uint16_t HighlightColor,
                    uint16_t SelectedTextColor, uint16_t SelectedColor,
                    uint16_t MenuColumn, uint16_t ItemRowHeight, uint16_t MaxRow,
                    const char *TitleText, const GFXfont &ItemFont, const GFXfont &TitleFont) {

  itc = TextColor;
  ibc = BackgroundColor;
  ihtc = HighlightTextColor;
  ihbc = HighlightColor;
  istc = SelectedTextColor;
  isbc = SelectedColor;

  tbt = 0;        // title bar top
  tbl = 0;        // title bar left
  tbh = ItemRowHeight;       // title bar height
  tbw = d->width(); // title bar width, default to screen width
  isx = 10;       // where to start the menu item from left, default indent 10

  isy = tbt + tbh + MM;  // where to start the menu item from top, note because array is 1 based we start drawing down a row--so back off a row, but pad 10

  irh = ItemRowHeight;  // select bar height
  irw = tbw - isx;    // select bar width, default to full width
  col = MenuColumn;
  imr = MaxRow;     // user has to indicate this
  itemf = ItemFont;     // item font
  titlef = TitleFont;     // title font
  strncpy(ttx, TitleText, MAX_CHAR_LEN);
  currentID = 1;    // id of current highlighted or selected item (1 to ID)
  cr = 1;       // current selected row on the screen (1 to mr-1)
  totalID = 0;    // maximum number of items (where first position is 1)
  sr = 0;       // draw offset for the menu array where first postion is 0
  pr = 1;       // previous selected rown (1 to mr - 1)
  rowselected = false;

}

int EditMenu::addNI(const char *ItemText, float Data, float LowLimit, float HighLimit, float Increment,
                    byte DecimalPlaces, const char **ItemMenuText) {

  totalID++;
  strcpy(itemlabel[totalID], ItemText);
  data[totalID] = Data;
  low[totalID] = LowLimit;
  high[totalID] = HighLimit;
  inc[totalID] = Increment;
  dec[totalID] = DecimalPlaces;
  haslist[totalID] = false;
  value[totalID] = Data;

  if (ItemMenuText) {
    haslist[totalID] = true;
    itemtext[totalID] = (char **) ItemMenuText;
  }

  return (totalID);

}

int EditMenu::selectRow() {

  if (currentID == 0) {
    // trigger to exit out of the menu
    item = 0;
    return 0;
  }
  // otherwise this is the trigger to enable editing in the row

  rowselected = !rowselected;

  drawRow(currentID);
  item = currentID;
  return currentID;

}


void EditMenu::drawHeader(bool hl) {

	d->setFont(&titlef);
	
    d->setCursor(tbl + TOX, tbt + TOY);
      

    if (hl) {
		d->fillRect(tbl, tbt, tbw, tbh, ihbc);
		d->setTextColor(ihtc);
		d->print(EXIT_TEXT);
    }

    else {
		d->fillRect(tbl, tbt, tbw, tbh, ibc);
		d->setTextColor(itc);
		d->print(ttx);
    }

}



void EditMenu::setIncrement(int ItemID, float Value) {
	
	if ((ItemID < 0) | (ItemID > totalID)) {
		return;
	}
	inc[ItemID] = Value;

}

void EditMenu::up() {
  cr--;
  currentID--;
  drawItems();

}

void EditMenu::down() {

  cr++;
  currentID++;
  drawItems();


}


void EditMenu::MoveUp() {

  if (rowselected) {
    incrementUp();
  }
  else {
    up();
  }

}

void EditMenu::MoveDown() {

  if (rowselected) {
    incrementDown();
  }
  else {
    down();
  }
}



void EditMenu::draw() {
	drawHeader(false);
	drawItems();
}

void EditMenu::drawItems() {

  if (imr > totalID) {
    imr = totalID;
  }
  redraw = false;
  // determine if we need to pan or just increment the list

  if ((currentID >  totalID) && (sr >= 0) ) {
    // up to top
    cr = 0;
    currentID = 0;
    sr = 0;
    redraw = true;
  }
  else if ( (cr < 0) && (sr == 0) ) {
    //  pan whole menu to bottom
    cr = imr;
    currentID = totalID;
    sr = totalID - imr;
    redraw = true;
  }
  else if ( (cr > imr) && ((sr + cr) > totalID) ) {
    //  pan whole menu to top
    cr = 1;
    currentID = 1;
    sr = 0;
    redraw = true;
  }
  else if ((cr > imr) && (sr >= 0))   {
    // scroll whole list up one by one
    sr++;
    cr = imr;
    redraw = true;
  }
  else if ((cr < 1) && (sr > 0))   {
    // scroll whole list down one by one
    sr--;
    cr = 1;
    redraw = true;
  }

  // determine if we need to redraw the header and draw
  if ((currentID == 0) && (cr == 0)) {
    drawHeader(true);
    redrawh = true;
  }
  else if ((pr == 0) && (cr > 0)) {
    if (redrawh) {
      redrawh = false;
      drawHeader(false);
    }

  }
	
  d->setFont(&itemf);

  // now draw the items in the rows
  for (i = 1; i <= imr; i++) {
    // menu bar start

    // text start
    itx = isx + IOX;

    temptColor = itc;


    if (redraw) {
      // scroll so blank out every row including icon since row will get scrolled
      d->fillRect(isx, isy - irh + (irh * i), irw - isx, irh, ibc); // back color
    }

    if (i == pr) {
      // maybe just row change so blank previous
      d->fillRect(isx, isy - irh + (irh * pr) , irw - isx, irh, ibc); // back color
    }

    if (i == cr) {

        d->fillRect(isx,       isy - irh + (irh * i) ,      irw - isx  ,        irh, ihbc);
        //d->fillRect(bs,  isy - irh + (irh * i), irw - bs , irh , ihbc);

      temptColor = ihtc;

    }

    // write text
    d->setTextColor(temptColor);
    d->setCursor(itx , isy - irh + (irh * i) + IOY);
    d->print(itemlabel[i + sr]);

    // write new val
    d->setCursor(col , isy - irh + (irh * i) + IOY);
    if (haslist[i + sr]) {
      d->print(itemtext[i + sr][(int) data[i + sr]]);
    }
    else {
      d->print(data[i + sr], dec[i + sr]);
    }

  }

  pr = cr;

}


void EditMenu::drawRow(int ID) {

  int hr = ID - sr;

  uint16_t textcolor, backcolor;

  // compute starting place for text
  itx = isx ;
  textcolor = itc;

  if (ID == 0) {
    drawHeader(false);
  }

  if (ID <= 0) {
    return;
  }

  if (ID <= sr) {
    // item is off screen
    return;
  }

  if (ID > (sr + imr)) {
    // item is off screen
    return;
  }

    if ((cr + sr) == ID) {

      if (!rowselected) {
        // case 1 draw som is draw row AND highlighted
        textcolor = ihtc;
        backcolor = ihbc;
      }
      else if (rowselected) {
        // case 2 = current row is draw row AND highlighted
        textcolor = istc;
        backcolor = isbc;

      }
    }
    else  {
      // current row is not selected
      textcolor = itc;
      backcolor = ibc;
    }

  if (ID == 0) {
    if (rowselected) {
      // draw new menu bar
      d->setCursor(tbl + TOX, tbt + TOY);
      d->fillRect(tbl, tbt, tbw, tbh, ibc);
      d->setTextColor(itc);
      d->print(ttx);
    }
    else {
      // draw new menu bar
      d->setCursor(tbl + TOX, tbt + TOY);
      d->fillRect(tbl, tbt, tbw, tbh, ihbc);
      d->setTextColor(ihtc);
      d->print(ttx);
    }
  }
  else {
	  
	d->fillRect(isx, isy - irh + (irh * hr), irw - isx  ,        irh, backcolor);
    // write text
    itx = isx + IOX;
    //d->setFont(&itemf);
    d->setTextColor(textcolor);
    d->setCursor(itx , isy - irh + (irh * hr) + IOY);
    d->print(itemlabel[ID]);
    d->setCursor(col , isy - irh + (irh * (ID - sr)) + IOY);
	
    if (haslist[ID]) {
      d->print(itemtext[ID][(int) data[ID]]);
    }
    else {
      d->print(data[ID], dec[ID]);
    }
		
  }
}

void EditMenu::incrementUp() {

  //d->setFont(&itemf);
  d->setTextColor(istc);


  if (haslist[currentID]) {

    if ((data[currentID] + inc[currentID]) < high[currentID]) {
      data[currentID] += inc[currentID];
      d->fillRect(col, isy - irh + (irh * cr) , irw - col , irh , isbc);
      d->setCursor(col + IOX , isy - irh + (irh * cr) + IOY);
      d->print(itemtext[currentID][(int) data[currentID]]);
    }
    else {
      data[currentID] = low[currentID];
      d->fillRect(col, isy - irh + (irh * cr) , irw - col , irh , isbc);
      d->setCursor(col, isy - irh + (irh * cr) + IOY);
      d->print(itemtext[currentID][(int) data[currentID]]);
    }

  }
  else {

    data[currentID] += inc[currentID];
    if (data[currentID] > high[currentID]) {
      data[currentID] = low[currentID];
    }
    d->fillRect(col, isy - irh + (irh * cr) , irw - col , irh , isbc);
    d->setCursor(col +  IOX, isy - irh + (irh * cr) + IOY);
    d->print(data[currentID], dec[currentID]);
  }

  delay(DEBOUNCE_TIME);
  value[currentID] = data[currentID];
  item = currentID;
}

void EditMenu::incrementDown() {

  //d->setFont(&itemf);
  d->setTextColor(istc);
  if (haslist[currentID]) {
    if ((data[currentID] - inc[currentID]) >= low[currentID]) {
      data[currentID] -= inc[currentID];
      d->fillRect(col , isy - irh + (irh * cr) , irw - col , irh , isbc);
      d->setCursor(col , isy - irh + (irh * cr) + IOY);
      d->print(itemtext[currentID][(int) data[currentID]]);
    }
    else {
      data[currentID] = high[currentID] - 1;
      d->fillRect(col , isy - irh + (irh * cr) , irw - col, irh , isbc);
      d->setCursor(col, isy - irh + (irh * cr) + IOY);
      d->print(itemtext[currentID][(int) data[currentID]]);
    }
  }
  else {
    data[currentID] -= inc[currentID];
    if (data[currentID] < low[currentID]) {
      data[currentID] = high[currentID];
    }
    d->fillRect(col, isy - irh + (irh * cr) , irw - col , irh , isbc);
    d->setCursor(col , isy - irh + (irh * cr) + IOY);
    d->print(data[currentID], dec[currentID]);
  }

  delay(DEBOUNCE_TIME);
  value[currentID] = data[currentID];
  item = currentID;

}

void EditMenu::setItemValue(int ID, float ItemValue) {
  value[ID] = ItemValue;
  data[ID] = ItemValue;
}


/*

  object type to create a simple selection only menu unlike previous where selecting a line item would allow in-line editing


*/

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


ItemMenu::ItemMenu(Adafruit_ST7735 *Display) {
  d = Display;
}

void ItemMenu::init(uint16_t TextColor, uint16_t BackgroundColor,
                    uint16_t HighlightTextColor, uint16_t HighlightColor,
                    uint16_t ItemRowHeight, uint16_t MaxRow,
                    const char *TitleText, const GFXfont &ItemFont, const GFXfont &TitleFont) {

  itc = TextColor;
  ibc = BackgroundColor;
  ihtc = HighlightTextColor;
  ihbc = HighlightColor;
  tbt = 0;      // title bar top
  tbl = 0;      // title bar left
  tbh = ItemRowHeight;     // title bar height
  tbw = d->width(); // title bar width, default to screen width
  
  isx = 0;      // where to start the menu item from left, default indent 10
  isy = tbt + tbh + MM; // where to start the menu item from top, note because array is 1 based we start drawing down a row--so back off a row, but pad 10
  irh = ItemRowHeight;  // select bar height
  irw = tbw - isx;    // select bar width, default to full width
  imr = MaxRow;     // user has to indicate this
  itemf = ItemFont;     // item font
  titlef = TitleFont;   // menu bar font, default to item font
  strncpy(ttx, TitleText, MAX_CHAR_LEN);
  item = 1;
  drawTitleFlag = true; // flag if we draw menu bar again
  currentID = 1;    // id of current highlighted or selected item (1 to ID)
  cr = 1;       // current selected row on the screen (1 to mr-1)
  totalID = 0;      // maximum number of items (where first position is 1)
  sr = 0;       // draw offset for the menu array where first postion is 0
  pr = 1;       // previous selected rown (1 to mr - 1)
  rowselected = false;
  redraw = true;

}

int ItemMenu::addNI(const char *ItemLabel) {

  totalID++;
  strncpy(itemlabel[totalID], ItemLabel, MAX_CHAR_LEN);
  return (totalID);

}

void ItemMenu::drawHeader(bool hl) {

	//d->setFont(&titlef);
	d->setCursor(tbl + TOX, tbt + TOY);

	if (hl) {
		d->fillRect(tbl, tbt, tbw, tbh, ihbc);
		d->setTextColor(ihtc);
		d->print(EXIT_TEXT);
	}

	else {
		d->fillRect(tbl, tbt, tbw, tbh, ibc);
		d->setTextColor(itc);
		d->print(ttx);
	}

}


void ItemMenu::drawRow(int ID) {

  int  bs;
  int hr = ID - sr;

  if (ID == 0) {
    drawHeader(false);
  }

  if (ID <= 0) {
    return;
  }

  bs = isx;

  if (ID == 0) {

      // draw new menu bar
      d->setCursor(tbl + TOX, tbt + TOY);
      d->fillRect(tbl, tbt, tbw, tbh, tfc);
      d->setTextColor(ttc);
      d->print(ttx);
    
  }
  else {

    d->fillRect(bs, isy - irh + (irh * hr) , irw - bs, irh, ibc); // back color

    // write text
    itx = bs + IOX;
    //d->setFont(&itemf);
    d->setTextColor(ihtc);
    d->setCursor(itx , isy - irh + (irh * hr) + IOY);
    d->print(itemlabel[ID]);

  }

}


void ItemMenu::draw() {

	drawHeader(false);
	drawItems();
  
}

void ItemMenu::drawItems() {

  if (imr > totalID) {
    imr = totalID;
  }
  redraw = false;
  // determine if we need to pan or just increment the list

  if ((currentID >  totalID) && (sr >= 0) ) {
    // up to top
    cr = 0;
    currentID = 0;
    sr = 0;
    redraw = true;
  }
  else if ( (cr < 0) && (sr == 0) ) {
    //  pan whole menu to bottom
    cr = imr;
    currentID = totalID;
    sr = totalID - imr;
    redraw = true;
  }
  else if ( (cr > imr) && ((sr + cr) > totalID) ) {
    //  pan whole menu to top
    cr = 1;
    currentID = 1;
    sr = 0;
    redraw = true;
  }
  else if ((cr > imr) && (sr >= 0))   {
    // scroll whole list up one by one
    sr++;
    cr = imr;
    redraw = true;
  }
  else if ((cr < 1) && (sr > 0))   {
    // scroll whole list down one by one
    sr--;
    cr = 1;
    redraw = true;
  }

  // determine if we need to redraw the header and draw
  if ((currentID == 0) && (cr == 0)) {
    drawHeader(true);
  }
  else if ((pr == 0) && (cr > 0)) {
    drawHeader(false);
  }


  //d->setFont(&itemf);

  // now draw the items in the rows
  for (i = 1; i <= imr; i++) {
    // menu bar start
    // text start
    itx = isx + IOX;

      temptColor = itc;

    if (redraw) {
      // scroll so blank out every row including icon since row will get scrolled
      d->fillRect(isx, isy - irh + (irh * i), irw - isx, irh, ibc); // back color
    }

    if (i == pr) {
      // maybe just row change so blank previous
      d->fillRect(isx, isy - irh + (irh * pr) , irw - isx, irh, ibc); // back color
    }

    if (i == cr) {

        d->fillRect(isx,       isy - irh + (irh * i) ,      irw - isx  ,        irh, ihbc);
       // d->fillRect(bs  ,  isy - irh + (irh * i) , irw - bs - (2 * thick) , irh - (2 * thick), ihbc);

      temptColor = ihtc;

    }

    // write text
    d->setTextColor(temptColor);
    d->setCursor(itx , isy - irh + (irh * i) + IOY);
    d->print(itemlabel[i + sr]);
  }

  pr = cr;

}

void ItemMenu::MoveUp() {

  cr--;
  currentID--;
  drawItems();

	  
}

void ItemMenu::MoveDown() {

  cr++;
  currentID++;
  drawItems();


}

int ItemMenu::selectRow() {
  if (currentID == 0) {
    cr = 0;
    sr = 0;
  }
  item = currentID;
  return currentID;
}

/////////////////////////////////
// end of this menu library
/////////////////////////////////
