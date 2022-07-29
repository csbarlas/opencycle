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
#include "oc_buttons.h"
#include "oc_gps.h"

//Struct representing current program state
OC_State main_state = { STATE_INIT , 0.0, 0.0, 0.0, 0, 0 };

int main()
{
    //wPi Setup (wPiSetup always returns zero as of v2)
    wiringPiSetup();

    init_buttons();

    if(LCD_ENABLE) init_lcd();

    init_gps();

    if(SPOOF_GPS)
    {
        main_state.curr_state=STATE_READY_TO_CAPTURE;
    }
    else
    {
        main_state.curr_state = STATE_GPS_STANDBY;
    }

    int points_captured = 0;
    while(1)
    {
        switch (main_state.curr_state)
        {
            case STATE_GPS_STANDBY:
                if(main_state.gps_quality != 0) 
                {
                    main_state.curr_state = STATE_READY_TO_CAPTURE;
                    break;
                }
                standby_output();
                print_dots(3.0);
                break;
            case STATE_READY_TO_CAPTURE:
                points_captured = 0;
                ready_output();
                sleep(1);
                break;
            case STATE_GPS_CAPTURE: ; //added ; for some label after statement error
                gps_capture_output(points_captured++);
                sleep(1);
                break;
            case STATE_GPS_PROCESSING:
                gps_processing_output();
                sleep(3);
                main_state.curr_state = STATE_GPS_STANDBY;
                break;
            case STATE_CLEANUP:
                cleanup_output();
                sleep(3);
                main_state.curr_state = STATE_QUIT;
                break;
            case STATE_QUIT:
                quit_output();
                exit(0);
                break;
            default:
                if (DEBUG) printf("main_button_pressed: ERROR hit default (undefined) case\n");
                break;
        }
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