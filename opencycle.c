#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include <wiringPi.h>

#include "state.h"
#include "pin_mappings.h"

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

    //init_lcd();

    init_gps();

    parse_raw_gps_data();

    // while(1)
    // {
    //     clock_t t;
    //     t = clock();
    //     parse_gga_string();
    //     t = clock() - t;
    //     double elapsed = ((double) t)/CLOCKS_PER_SEC;
    //     printf("Took %lf seconds\n", elapsed);
    // }

    // while(1)
    // {
    //     switch (main_state.curr_state)
    //     {
    //         case STATE_INIT:
    //             break;
    //         case STATE_GPS_STANDBY:
    //             break;  
    //     }
    // }

    return EXIT_SUCCESS;
}