#include <math.h>
#include "mathStuff.h"
 
const float pi = 3.14159265;
/***
 * FUNCTION:    sind
 * PARAMETERS:  float a
 * RETURNS:     float
 * DESCRIPTION: Converts degrees to radians and calls sin().
 ***/ 
extern float (*dsin)(float) = sind;
float sind(float a)
{
    return sin(2 * pi * a / 360);
}

/***
 * FUNCTION:    cosd
 * PARAMETERS:  float a
 * RETURNS:     float
 * DESCRIPTION: Converts degrees to radians and calls cos().
 ***/ 
float (*dcos)(float) = cosd;
float cosd(float a)
{
    return cos(2 * pi * a / 360);
}

/***
 * FUNCTION:    fibb
 * PARAMETERS:  int f
 * RETURNS:     int
 * DESCRIPTION: Calculates the Fibonacci number of f.
 ***/ 
int(*fibb)(int) = fibonacci;
int fibonacci(int f)
{
    int value = 0;
    int old_value = 1;

    while (f--) {
        value += old_value;
        old_value = value - old_value;
    }

    return value;
}
