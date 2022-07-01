#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include <wiringPi.h>

#include "state.h"
#include "pin_mappings.h"
#include "debug.h"

#include "opencycle.h"
#include "buttons.h"
#include "oc_gps.h"

//Struct representing current program state
OC_State main_state = { STATE_INIT };

int main()
{
    //wPi Setup (wPiSetup always returns zero as of v2)
    wiringPiSetup();

    init_buttons();

    if(LCD_ENABLE) init_lcd();

    init_gps();
    main_state.curr_state = STATE_READY_TO_CAPTURE;
    while(1)
    {
        printf("main: current state = %d", main_state.curr_state);
        sleep(1);
    }

    return EXIT_SUCCESS;
}


void print_divider(int length)
{
    for(int i = 0; i < length; i++)
    {
        printf("*");
    }
    printf("\n");
}