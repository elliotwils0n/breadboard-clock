#ifndef __structures__H
#define __structures__H 1

#include "headers.h"

struct the_time
{
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;
};

enum MODE
{
    RUNNING,
    SET_HOUR,
    SET_MINUTES,
    SET_SECONDS
};

#endif /* structures.h */
