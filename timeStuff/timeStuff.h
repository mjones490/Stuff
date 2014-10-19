#ifndef TIMESTUFF_H
#define TIMESTUFF_H
#include <sys/time.h>
unsigned int get_ms_diff(struct timeval *tv1, struct timeval *tv2);
float timestring_to_seconds(char* timestring);
char* seconds_to_timestring(float seconds);

#endif
