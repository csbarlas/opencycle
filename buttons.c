#include <time.h>

#include <wiringPi.h>

#include "state.h"
#include "buttons.h"
#include "pin_mappings.h"
#include "callbacks.h"

#define BUTTON_DEBOUNCE_VALUE 3E8L

int isPressedDebounce(int button)
{
    //prevTime is the last time @ LOW (falling edge aka button trigger)
    struct timespec currTime, prevTime;
    clock_gettime(CLOCK_MONOTONIC, &prevTime);
    while(1)
    {
        clock_gettime(CLOCK_MONOTONIC, &currTime);
        //if signal goes back up to 1 aka "bounce"
        if(digitalRead(button) == 1)
        {
            clock_gettime(CLOCK_MONOTONIC, &prevTime);
        }
        else if(currTime.tv_nsec - prevTime.tv_nsec > BUTTON_DEBOUNCE_VALUE)
        {
            break;
        }

    }
    return 1;
}

int init_buttons()
{
    pinMode(START_BUTTON, INPUT);
    pullUpDnControl(START_BUTTON, PUD_UP);
    wiringPiISR(START_BUTTON, INT_EDGE_FALLING, main_button_pressed);

    pinMode(STOP_BUTTON, INPUT);
    pullUpDnControl(STOP_BUTTON, PUD_UP);
    wiringPiISR(STOP_BUTTON, INT_EDGE_FALLING, secondary_button_pressed);

    return 0;
}