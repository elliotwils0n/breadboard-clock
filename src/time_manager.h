#ifndef __time_manager__H
#define __time_manager__H 1

#include "headers.h"
#include "structures.h"

void add_second(volatile struct the_time* _the_time);
void add_minute(volatile struct the_time* _the_time);
void add_hour(volatile struct the_time* _the_time);
void reset_seconds(volatile struct the_time* _the_time);

#endif /* time_manager.h */
