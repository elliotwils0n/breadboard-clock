#include "block_manager.h"

void init_blocks(volatile uint8_t block_states[], volatile struct the_time* _the_time)
{
    block_states[0] = DIGITS[_the_time->hour / 10];
    block_states[1] = DIGITS[_the_time->hour % 10];
    block_states[2] = DASH_SEPARATOR;
    block_states[3] = DIGITS[_the_time->minutes / 10];
    block_states[4] = DIGITS[_the_time->minutes % 10];
    block_states[5] = DASH_SEPARATOR;
    block_states[6] = DIGITS[_the_time->seconds / 10];
    block_states[7] = DIGITS[_the_time->seconds % 10];
}

void update_blocks(volatile uint8_t block_states[], volatile struct the_time* _the_time)
{
    block_states[0] = DIGITS[_the_time->hour / 10];
    block_states[1] = DIGITS[_the_time->hour % 10];
    block_states[3] = DIGITS[_the_time->minutes / 10];
    block_states[4] = DIGITS[_the_time->minutes % 10];
    block_states[6] = DIGITS[_the_time->seconds / 10];
    block_states[7] = DIGITS[_the_time->seconds % 10];
}

void set_block_visibility(volatile uint8_t block_states[], volatile enum MODE mode, uint8_t blink_state, volatile struct the_time* _the_time)
{
    switch(mode)
    {
        case SET_HOUR:
        {
            block_states[0] = (blink_state) ? DIGITS[_the_time->hour / 10] : BLANK;
            block_states[1] = (blink_state) ? DIGITS[_the_time->hour % 10] : BLANK;
            break;
        }
        case SET_MINUTES:
        {
            block_states[3] = (blink_state) ? DIGITS[_the_time->minutes / 10] : BLANK;
            block_states[4] = (blink_state) ? DIGITS[_the_time->minutes % 10] : BLANK;
            break;
        }
        case SET_SECONDS:
        {
            block_states[6] = (blink_state) ? DIGITS[_the_time->seconds / 10] : BLANK;
            block_states[7] = (blink_state) ? DIGITS[_the_time->seconds % 10] : BLANK;
            break;
        }
        default:
        {
            break;
        }
    }
}
