#include <stdio.h>

#include "state.h"
#include "oc_buttons.h"
#include "stdint.h"
#include "pin_mappings.h"
#include "debug.h"

extern OC_State main_state;

static void button_transition_msg(char *caller, runtime_state prev_state, runtime_state new_state)
{
    if (DEBUG) printf("%s: %d to %d\n", caller, prev_state, new_state);
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
            case STATE_READY_TO_CAPTURE:
                button_transition_msg("main_button_pressed", prev_state, STATE_GPS_CAPTURE);
                init_file();
                generate_gpx_header();
                main_state.curr_state = STATE_GPS_CAPTURE;
                break;
            case STATE_GPS_CAPTURE:
                button_transition_msg("main_button_pressed", prev_state, STATE_GPS_PROCESSING);
                main_state.curr_state = STATE_GPS_PROCESSING;
                break;
            case STATE_CLEANUP:
                button_transition_msg("main_button_pressed", prev_state, STATE_QUIT);
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

void secondary_button_pressed(void)
{
    if(isPressedDebounce(STOP_BUTTON))
    {
        runtime_state prev_state;
        switch (prev_state = main_state.curr_state)
        {
            case STATE_READY_TO_CAPTURE:
                button_transition_msg("secondary_button_pressed", prev_state, STATE_GPS_CAPTURE);
                main_state.curr_state = STATE_CLEANUP;
                break;
            default:
                if (DEBUG) printf("secondary_button_pressed: ERROR hit default (undefined) case\n");
                break;
        }
    }
}