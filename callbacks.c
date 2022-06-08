#include <stdio.h>

#include "state.h"
#include "buttons.h"
#include "stdint.h"
#include "pin_mappings.h"

extern OC_State main_state;

void main_button_pressed(void)
{
    if(isPressedDebounce(START_BUTTON))
    {
        switch (main_state.curr_state)
        {
            case STATE_GPS_STANDBY:
                main_state.curr_state = STATE_GPS_CAPTURE;
        }
    }
}