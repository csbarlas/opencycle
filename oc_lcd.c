#include <wiringPi.h>
#include <lcd.h>

#include "state.h"
#include "pin_mappings.h"

#define LCD_ROWS 2
#define LCD_COLS 16
#define BIT_MODE 4

static int lcd_handle = -1;

void init_lcd()
{
    lcd_handle = lcdInit(LCD_ROWS, LCD_COLS, BIT_MODE, RS, ENBL, DB4, DB5, DB6, DB7, 0, 0, 0, 0);
    lcdClear(lcd_handle);
    lcdPrintf(lcd_handle, "Initializing...");
}