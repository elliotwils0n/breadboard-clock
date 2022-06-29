#include "headers.h"
#include "time_manager.h"
#include "block_manager.h"
#include "structures.h"

volatile struct the_time _the_time = { 0, 0, 0 };
volatile enum MODE current_mode = RUNNING;
volatile uint8_t input_enabled = 1;
volatile uint8_t blink = 0;
volatile uint8_t block_states[8];

/* TIMER1 counting 1 second */
ISR(TIMER1_COMPA_vect)
{
    if(current_mode == RUNNING)
    {
        add_second(&_the_time);
        update_blocks(block_states, &_the_time);
    }
}

/* TIMER0 counting 0.25 seconds */
ISR(TIMER0_COMP_vect)
{
    // Enabling input after 0.25 seconds.
    input_enabled = 1;
    // Change blinking state (used for current value in SET_XXX mode).
    if(current_mode != RUNNING)
    {
        blink = (blink + 1) % 2;
        set_block_visibility(block_states, current_mode, blink, &_the_time);
    }
}

/* INT2 external interrupt for changing current mode. */
ISR(INT2_vect)
{
    if(input_enabled)
    {
        current_mode = (current_mode + 1) % 4;
        update_blocks(block_states, &_the_time);
        /* Disabling input and reseting timer for input blocking. */
        input_enabled = 0;
        TCNT0 = 0;
    }
}

ISR(INT1_vect)
{
    if(input_enabled)
    {
        /* Handling interrupt based on current mode. */
        switch(current_mode)
        {
            case RUNNING: { break; }
            case SET_HOUR: { add_hour(&_the_time); break; }
            case SET_MINUTES: { add_minute(&_the_time); break; }
            case SET_SECONDS: { reset_seconds(&_the_time); break; }
            default: { break; }
        }
        /* Disabling input and reseting timer for input blocking. */
        input_enabled = 0;
        TCNT0 = 0;
    }
}

int main(void)
{
    /* Disabling JTAG interface to use PORTC as I/O. Documentation says it has to be done twice. */
    MCUCSR |= (1 << JTD);
    MCUCSR |= (1 << JTD);

    /*
    Setting direction registers to 1 (output).
    DDRC is responsible for setting the current 8-segment display block to update.
    DDRA is responsible for setting values for the 8-segment display block DDRC is pointing to.
    */
    DDRA = 0xFF;
    DDRC = 0xFF;
    /* Setting direction register to 0 for PD3 (input, INT1 external interrupt) */
    DDRD &= ~(1<<PD3);
    /* Setting direction register to 0 for PB2 (input, INT2 external interrupt) */
    DDRB &= ~(1<<PB2);

    /*
    Setting TIMER1 to count seconds.
    Mode: CTC(clear timer on compare match), TOP: OCR1A.
    TIME_IN_SECONDS * ATMEGA_FREQ / PRESCALER = OCR1A
                  1 * 1000000     / 64        = 15625 
    */
    OCR1A = 15625;
    TCCR1A = 0x00;
    TCCR1B &= ~(1 << WGM13);
    TCCR1B |= (1 << WGM12);
    /* Setting prescaler of TIMER1 to 64 */
    TCCR1B &= ~(1 << CS12);
    TCCR1B |= (1 << CS11);
    TCCR1B |= (1 << CS10);

    /*
    Setting TIMER2 blocking user input for 0.25 s.
    Mode: CTC(clear timer on compare match), TOP: OCR2.
    0.25 * 1000000 / 1024 = ~244
    */
    OCR0 = 244;
    TCCR0 |= (1 << WGM01);
    TCCR0 &= ~(1 << WGM00);
    /* Disconnecting OC2 pin from timer. */
    TCCR0 &= ~(1 << COM01);
    TCCR0 &= ~(1 << COM00);
    /* Setting prescaler of TIMER2 to 1024 */
    TCCR0 |= (1 << CS02);
    TCCR0 &= ~(1 << CS01);
    TCCR0 |= (1 << CS00);

    cli();
    /* Setting Output Compare A Match Interrupt Enable (ISR for TIMER1_COMPA_vect). */
    TIMSK |= (1 << OCIE1A);
    /* Setting Output Compare Match Interrupt Enable (ISR for TIMER0_COMP_vect). */
    TIMSK |= (1 << OCIE0);
    /* Enabling INT2 external interrupt (ISR for INT2_vect) */
    GICR |= (1 << INT2);
    /* Setting INT2 external interrupt to trigger at falling edge */
    MCUCSR &= ~(1 << ISC2);
    /* Enabling INT1 external interrupt (ISR for INT1_vect) */
    GICR |= (1 << INT1);
    /* Setting INT1 external interrupt to trigger at falling edge */
    MCUCR |= (1 << ISC11);
    MCUCR &= ~(1 << ISC10);
    sei();

    /* Initializing blocks before program loop */
    init_blocks(block_states, &_the_time);

    while(1)
    {
        /*
        In order to trick the human eye that there is no transition between blocks,
        the display need to be refreshed in ~16 ms.
        There is a 2 ms delay after each updated digit in the display.
        */
        for(int i = 0; i < 8; i++)
        {
            /* Clear data before changing the block to avoid ghosting */
            PORTA = BLANK;
            /* Change block and update it's value */
            PORTC = BLOCKS[i];
            PORTA = block_states[i];
            _delay_ms(2);
        }
    }
    return 0;
}
