#ifndef MATH_STUFF_H
#define MATH_STUFF_H
float sind(float a);
float cosd(float a);
#define sinr(a) sin((a))
#define cosr(a) cos((a))


int fibonacci(int f); 
extern int (*fibb)(int f); // __attribute__ ((deprecated));
float (*dsin)(float a); // __atribute__ ((deprecated));
extern float (*dcos)(float a);  //__atribute__ ((deprecated));

#endif
