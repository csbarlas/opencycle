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
    short gps_quality;
    short sats_in_view;
    double lat;
    double lng;
    double gps_time;
} OC_State;