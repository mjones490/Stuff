#ifndef MACRO_STUFF_H
#define MACRO_STUFF_H

#define BOOL int
#define FALSE 0
#define TRUE 1

#define SWAP(x, y) x ^= y; y ^= x; x ^= y;

#ifdef DEBUG
#define LOC_INFO fprintf(stderr, "%s : %d : %s : ", __FILE__, __LINE__, __func__); 
#else
#define LOC_INFO
#endif

#define err_out(ERR, ...) do { \
    LOC_INFO \
    fprintf(stderr, __VA_ARGS__); \
     exit(ERR); \
    } while(0)

#define ARRAY_SIZE(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

#endif


