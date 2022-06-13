#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "state.h"
#include "debug.h"
#include "oc_gps.h"

static int serial_port;
static char buffer[100];

extern OC_State main_state;

void init_gps()
{
    if((serial_port = serialOpen("/dev/ttyAMA1", 9600)) < 0)
    {
        printf("Cannot open serial device\n");
    }
}

int NMEA_msg_is_GGA(char *data)
{
    return (data[0] == 'G' && data[1] == 'G' && data[2] == 'A');
}

//Code inspired from www.electronicwings.com
char *parse_gga_string()
{
    char completed = 0;
    char curr_char;
    char nmea_msg_type[3];
    char is_GGA = 0;
    char GGA_index = 0;
    while(!completed)
    {
        if(serialDataAvail(serial_port))
        {
            curr_char = serialGetchar(serial_port);
            if (curr_char == '$')
            {
                //at start of nmea message string
                is_GGA = 0;
                GGA_index = 0;
            }
            else if (is_GGA)
            {
                //if we know we are currently processing 
                //a gga string
                buffer[GGA_index] = curr_char;
                GGA_index++;
                //check if at end
                if(curr_char == '\r') completed = 1;
            }
            else if (NMEA_msg_is_GGA(nmea_msg_type))
            {
                is_GGA = 1;
                nmea_msg_type[0] = 0;
                nmea_msg_type[1] = 0;
                nmea_msg_type[2] = 0;
            }
            else
            {
                nmea_msg_type[0] = nmea_msg_type[1];
                nmea_msg_type[1] = nmea_msg_type[2];
                nmea_msg_type[2] = curr_char;
            }
        }
    }
    
    printf("%s\n", buffer);
    return buffer;
}


//TODO Handle when there is no gps fix:
//$GPGGA,010617.00,,,,,0,00,99.99,,,,,,*67
void parse_raw_gps_data()
{
    char delimiters[] = ",";
    char *gga_string = parse_gga_string();
    char *gga_array[13];
    int i = 0;

    char *token = strtok(gga_string, delimiters);
    while(token != NULL)
    {
        gga_array[i++] = token;
        printf("%s is of length %d\n", token, strlen(token));
        token = strtok(NULL, delimiters);
    }

    double decimal_lat = compute_decimal_degrees(gga_array[1], gga_array[2][0]);
    double decimal_long = compute_decimal_degrees(gga_array[3], gga_array[4][0]);
    
    if (DEBUG) printf("(%lf,%lf)\n", decimal_lat, decimal_long);
}

//Converts raw GGA longitude and latitude strings into decimal degrees
double compute_decimal_degrees(char *raw_lat, char dir)
{
    double temp, frac_mins, ret;
    int whole_minutes, whole_degrees;

    //Convert GGA string into more workable types
    //and split into integer from decimal parts
    double double_raw_lat = strtod(raw_lat, NULL);
    frac_mins = modf(double_raw_lat, &temp);

    //Get whole degrees and whole minutes
    whole_degrees = (int) temp;
    whole_minutes = whole_degrees % 10;
    whole_degrees /= 10;
    whole_minutes += (whole_degrees % 10) * 10;
    whole_degrees /= 10;

    //Convert to decimal degrees
    ret = whole_degrees + (whole_minutes / 60.0) + (frac_mins / 60);

    //Change signs if appropriate
    if(dir == 'S' || dir == 'W') ret *= -1;

    return ret;
}