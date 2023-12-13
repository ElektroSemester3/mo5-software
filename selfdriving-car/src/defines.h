#include <stdint.h>
#ifndef _DEFINES_H_
#define _DEFINES_H_

struct SpeedStruct
{
    uint8_t left;
    uint8_t right;
    uint8_t speed;
};

typedef struct SpeedStruct speed_struct;


#endif // _DEFINES_H_