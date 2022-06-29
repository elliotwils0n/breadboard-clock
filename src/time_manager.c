#include "time_manager.h"

void add_second(volatile struct the_time* _the_time)
{
    _the_time->seconds = (_the_time->seconds + 1) % 60;
    if(_the_time->seconds == 0)
    {
        add_minute(_the_time);
    }
}

void add_minute(volatile struct the_time* _the_time)
{
    _the_time->minutes = (_the_time->minutes + 1) % 60;
    if(_the_time->minutes == 0)
    {
        add_hour(_the_time);
    }
}

void add_hour(volatile struct the_time* _the_time)
{
    _the_time->hour = (_the_time->hour + 1) % 24;
}

void reset_seconds(volatile struct the_time* _the_time)
{
    _the_time->seconds = 0;
}
