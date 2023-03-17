<b><h2><center>Version 1.0 updated 3/16/2023</center></h1></b>
1. Initial library posting


<b><h2><center>Menu System for Adafruit_ST7735 displays with mechanical input</center></h1></b>

This library will simplify creating and implementing a menu system for TFT displays that use the Adafruit_ST7735 displays with microcontrollers such as the ESP32 or Teensy LC. See my other repository for the ILI8341_t3 for Teensy-based projects.
<br>

<b>This library does not support Arduino UNO, NANO, however it may work with the MEGA.</b>

This library includes two menu objects
<br>

  1) A simple selection menu (ItemMenu) where the user can scroll up/down and select
  an item and some programmer defined action can be performed such as drawing a screen or activating a sub menu
  2) A menu with in-line edit capability (EditMenu) where the user can scroll up/down the list and select an item
  however, the difference is that move/up down on a selected item will then scroll the values stored with that
  menu item, selecting a selected item will restore control to moving up/down the menu list

<b>Watch this YouTube video to see the library with touch input:</b> https://youtu.be/wfMcTC3_8bI
<br>
<b>Watch this YouTube video to see the library with mechanical input:</b> https://youtu.be/KUi9f4hcV6A

<br>
To activate touch usage specify true in the menu object creation step

  <li>EditMenu(Adafruit_ILI9341 *Adafruit_ST7735);</li>
  <li>ItemMenu(Adafruit_ILI9341 *Adafruit_ST7735);</li>

<br>
<br>
 <b>Library highlights</b>
  <ul>
  <li>Support for menu selections using mechanical input such as rotary encoders or buttons</li> 
  <li>Automatic wrapping for menu rows that exceed screen size</li> 
  <li>For EditMenu objects, edits can be within a defined range or from an array</li> 
  <li>Menus can include no icon, mono icon, or color icons</li> 
  <li>Programmer has full control over margins, colors, fonts, etc. </li> 
  <li>Menus can have up to 15 items, but expandable to more by editing the #define MAX_OPT 15</li> 
  <li>Menu items can be enabled or disabled through code</li> 
  <li>Automatic wrapping during editing for items within editable menus</li> 
  <li>Menu item text can be changed at any time through code</li> 
  <li>Initial data in EditMenu objects can be set based on stored or set values</li>
  <li>Menu selector bars can have round or sharp corners and have a border to accentuate selection</li>
  <li>Automatic up/down arrows on top and bottom screen rows to indicate to the user "there's more"</li>
</ul> 
  <br>
  <br>
While this library was created for Adafruit_ST7735 drivers, the library can be adapted to other display drivers. Note the driver must use the Adafruit_GFX libraries. To allow other display drivers, modify the lines in the .h and .cpp files to allow a different driver to be passed in. For example change call such as SliderH(Adafruit_ST7735 *disp,... to SliderH(Adafruit_RA8875 *disp,... and in the .cpp files you may have to change how fonts are passed in using either setFont(&SomeFont) or setFont(SomeFont)
<br>
<br>

![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0003.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0006.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0007.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0021.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0023.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0044.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0073.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0074.jpg)

<br>
<br>

I have included an example that shows how to create multi-menu system with both a simple select menu that call editable menus
<br>
 <br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0003.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0073.jpg)

<br>
<br>
<b><h2><center>Revision history</center></h1></b>
<br>
1.0		3/16/23   initial code<br>

