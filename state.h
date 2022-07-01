#include <pthread.h>

typedef enum runtime_state
{
    STATE_ERR,
    STATE_INIT,
    STATE_GPS_STANDBY,
    STATE_READY_TO_CAPTURE,
    STATE_GPS_CAPTURE,
    STATE_GPS_PROCESSING,
    STATE_CLEANUP,
    STATE_QUIT
} runtime_state;

typedef struct OC_State
{
    runtime_state curr_state;
    double lat_dd; //dd = decimal degree
    double long_dd;
    double gps_time;
    short gps_quality;
    short sats_in_view;
    pthread_t gps_thread_id;
} OC_State;
