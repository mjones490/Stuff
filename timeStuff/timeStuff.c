#include <stdio.h>
#include "timeStuff.h"

/***
 * FUNCTION:    get_ms_diff   
 * PARAMETERS:  struct timeval *tv1 - First time value
 *              struct timeval *tv2 - Second time value
 * RETURNS:     int
 * DESCRIPTION: Calculate difference between 2 time values
 ***/ 
unsigned int get_ms_diff(struct timeval *tv1, struct timeval *tv2)
{
    int ms_diff;

    // Add microseconds
    ms_diff = (tv2->tv_usec - tv1->tv_usec) ; 

    // Fix for daily rollover
    if (tv2->tv_sec < tv1->tv_sec)
        tv2->tv_sec += 86400;

    // Add seconds
    ms_diff += ((tv2->tv_sec < tv1->tv_sec)? 86400 : 0 +
        tv2->tv_sec - tv1->tv_sec) * 1000000;

    return ms_diff;
}

float timestring_to_seconds(char* timestring)
{
    int minutes = 0;
    float seconds = 0;

    sscanf(timestring, "%dm%fs", &minutes, &seconds);
    seconds += minutes * 60;

    return seconds;
}

char* seconds_to_timestring(float seconds)
{
    int minutes = 0;
    static char timestring[32];

    minutes = seconds / 60;
    seconds -= (minutes * 60);
    sprintf(timestring, "%dm%fs", minutes, seconds);

    return timestring;
}

