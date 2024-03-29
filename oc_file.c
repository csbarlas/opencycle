#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

#include "state.h"
#include "debug.h"

#define GPX_NAME "OpenCycle Track"

extern OC_State main_state;

int init_dir()
{
    DIR* dir = opendir("./oc_output");
    if(!dir)
    {
        //TODO: Document/Find out why 0755 gives correct user perms
        if (DEBUG) printf("init_dir: oc_output directory created.");
        mkdir("./oc_output/", 0755);
    }
}

int init_file()
{
    init_dir();
    char filename[40];
    generate_filename(filename);

    FILE *temp = fopen(filename, "w");
    int flag = (temp == NULL);
    if(flag)
    {
        if(DEBUG) printf("file_init: ERROR opening file %s\n", filename);
    }
    else
    {
        if(DEBUG) printf("file_init: successfully opened file %s\n", filename);
        main_state.gpx_file = temp;
    }
    return flag;
}

int close_file()
{
    int flag = 0;

    if(main_state.gpx_file)
    {
        flag = fclose(main_state.gpx_file);
        if(flag)
        {
            if (DEBUG) printf("close_file: ERROR could not close GPX output file.\n");
        }
        else
        {
            if (DEBUG) printf("close_file: successfully closed GPX output file.\n");
        }
    }

    return flag;
}

void generate_gpx_header()
{
    FILE *fp = main_state.gpx_file;
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<gpx version=\"1.0\">\n");
    fprintf(fp, "\t<name>%s</name>\n", GPX_NAME);
    fprintf(fp, "\t<trk>\n");
    fprintf(fp, "\t\t<name>Track 1</name>\n");
    fprintf(fp, "\t\t<trkseg>\n");
}

void generate_gpx_footer()
{
    FILE *fp = main_state.gpx_file;
    fprintf(fp, "\t</trkseg></trk>\n");
    fprintf(fp, "</gpx>");
}

void print_trkpt()
{
    FILE *fp = main_state.gpx_file;
    char buffer[25];
    get_utc_time(buffer);

    fprintf(fp, "\t\t\t<trkpt lat=\"%lf\" lon=\"%lf\">\n", main_state.lat_dd, main_state.long_dd);
    fprintf(fp, "\t\t\t\t<time>%s</time>\n", buffer);
    fprintf(fp, "\t\t\t</trkpt>\n");
}

void get_utc_time(char *buffer)
{
    time_t raw_time = time(NULL);
    struct tm *utc_time = gmtime(&raw_time);
    snprintf(buffer, 25, "%4d-%02d-%02dT%02d:%02d:%02dZ", 
        (utc_time->tm_year + 1900),
        (utc_time->tm_mon + 1),
        utc_time->tm_mday,
        utc_time->tm_hour,
        utc_time->tm_min,
        utc_time->tm_sec);
}

void generate_filename(char *buffer)
{
    time_t raw_time = time(NULL);
    struct tm *utc_time = gmtime(&raw_time);
    snprintf(buffer, 40, "oc_output/opencycle-%4d-%02d-%02d %02d %02d %02d",
        (utc_time->tm_year + 1900),
        (utc_time->tm_mon + 1),
        utc_time->tm_mday,
        utc_time->tm_hour,
        utc_time->tm_min,
        utc_time->tm_sec);
}