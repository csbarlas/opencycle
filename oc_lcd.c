#include <wiringPi.h>
#include <lcd.h>
#include <unistd.h>
#include <string.h>

#include "state.h"
#include "pin_mappings.h"

#define LCD_ROWS 2
#define LCD_COLS 16
#define BIT_MODE 4

static int lcd_handle = -1;

extern OC_State main_state;

void init_lcd()
{
    lcd_handle = lcdInit(LCD_ROWS, LCD_COLS, BIT_MODE, RS, ENBL, DB4, DB5, DB6, DB7, 0, 0, 0, 0);
    lcdClear(lcd_handle);
    lcdPrintf(lcd_handle, "Initializing");
}

void print_dots(double total_time)
{
    char dots[4] = "";
    const char dot = '.';
    double equal_time = total_time / 3;
    for(int i = 0; i < 4; i++)
    {
        sleep(equal_time);
        lcdPutchar(lcd_handle, '.');
    }
}

void standby_output()
{
    lcdClear(lcd_handle);
    lcdPrintf(lcd_handle, "Waiting for GPS      Fix");
}

void ready_output()
{
    lcdClear(lcd_handle);
    lcdPrintf(lcd_handle, "Aquired GPS Fix Press Start/Quit");
}

void gps_capture_output(int points_captured)
{
    lcdClear(lcd_handle);
    lcdPrintf(lcd_handle, "lt:%.6lf|%03dlng:%.4lf", main_state.lat_dd, points_captured, main_state.long_dd);
}

void gps_processing_output()
{
    lcdClear(lcd_handle);
    lcdPrintf(lcd_handle, "Processing...");
}

void cleanup_output()
{
    lcdClear(lcd_handle);
    lcdPrintf(lcd_handle, "Cleaning Up...");
}

void quit_output()
{
    lcdClear(lcd_handle);
}