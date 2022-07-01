#include <stdio.h>

#include "state.h"
#include "buttons.h"
#include "stdint.h"
#include "pin_mappings.h"
#include "debug.h"

extern OC_State main_state;

static void button_transition_msg(runtime_state prev_state, runtime_state new_state)
{
    if (DEBUG) printf("main_button_pressed: %d to %d\n", prev_state, new_state);
}

void main_button_pressed(void)
{
    if(isPressedDebounce(START_BUTTON))
    {
        runtime_state prev_state;
        switch (prev_state = main_state.curr_state)
        {
            case STATE_ERR:
                if (DEBUG) printf("main_button_pressed: ERROR (STATE_ERR)\n");
                break;
            case STATE_INIT:
                button_transition_msg(prev_state, STATE_GPS_STANDBY);
                main_state.curr_state = STATE_GPS_STANDBY;
                break;
            case STATE_GPS_STANDBY:
                button_transition_msg(prev_state, STATE_READY_TO_CAPTURE);
                main_state.curr_state = STATE_READY_TO_CAPTURE;
                break;
            case STATE_READY_TO_CAPTURE:
                button_transition_msg(prev_state, STATE_GPS_CAPTURE);
                main_state.curr_state = STATE_GPS_CAPTURE;
                break;
            case STATE_GPS_CAPTURE:
                button_transition_msg(prev_state, STATE_GPS_PROCESSING);
                main_state.curr_state = STATE_GPS_PROCESSING;
                break;
            case STATE_GPS_PROCESSING:
                button_transition_msg(prev_state, STATE_CLEANUP);
                main_state.curr_state = STATE_CLEANUP;
                break;
            case STATE_CLEANUP:
                button_transition_msg(prev_state, STATE_QUIT);
                main_state.curr_state = STATE_QUIT;
                break;
            case STATE_QUIT:
                printf("hit quit");
                break;
            default:
                if (DEBUG) printf("main_button_pressed: ERROR hit default (undefined) case\n");
                break;
        }
    }
}