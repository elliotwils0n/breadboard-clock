#ifndef __block_manager__H
#define __block_manager__H 1

#include "headers.h"
#include "structures.h"

/* Digits defined for 8-segment display block (0-9). */
volatile static uint8_t DIGITS[] = 
{
    0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001,
    0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000
};
/* Separator '-' */
volatile static uint8_t DASH_SEPARATOR = 0b10111111;
/* Blank block */
volatile static uint8_t BLANK = 0b11111111;
/* Array of 8 blocks from two 8-segment displays. */
volatile static uint8_t BLOCKS[] = { 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0x20, 0x10 };

void init_blocks(volatile uint8_t block_states[], volatile struct the_time* _the_time);
void update_blocks(volatile uint8_t block_states[], volatile struct the_time* _the_time);
void set_block_visibility(volatile uint8_t block_states[], volatile enum MODE mode, uint8_t blink_state, volatile struct the_time* _the_time);

#endif /* block_manager.h */
