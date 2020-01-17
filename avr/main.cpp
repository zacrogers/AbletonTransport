/*
 * AbletonTransport.cpp
 *
 * Created: 15-Jan-20 3:11:24 PM
 * Author : Zac
 */ 
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "main.h"
#include "my_uart.h"
#include "gpio.h"


/* Namespaces */
using z_lib::Gpio;

const uint8_t CC_NUM[6] = {0, 1, 2, 3, 4, 5};
const BtnFunc FUNCS[6]  = {BtnFunc::STOP, BtnFunc::PLAY, BtnFunc::REC,
                           BtnFunc::LOOP, BtnFunc::RWD,  BtnFunc::FFWD};

//Gpio dip_switch[] = {};
Gpio buttons[]    = {};
Gpio leds[1]      = {Gpio(&PORTD, PIND4, z_lib::Mode::OUT)};
	

/* Led state booleans */
bool is_playing = false;
bool is_looping = false;
bool rec_armed  = false;

uint8_t current_state  = 0x00;
uint8_t previous_state = 0x00;

uint8_t midi_channel   = 0;

int main(void)
{
	init_uart(9600);

    while (1) 
    {
		poll_buttons();
		if(previous_state != current_state)
		{
			leds_set();
		}
	}
}

void poll_buttons(void)
{
	for(uint8_t btn = 0; btn < NUM_BTNS; ++btn)
	{
		if(buttons[btn].read())
		{
			update_state(FUNCS[btn]);
			send_cc(CC_NUM[btn], 127, midi_channel);
			_delay_ms(250);
		}
	}
}

void leds_set(void)
{
    for(uint8_t led = 0; led < NUM_BTNS; ++led)
    {
	    if(current_state & (0x01 << led))
	    {
		    leds[led].set();
	    }
	    else
	    {
		    leds[led].clear();
	    }
    }	
}

void update_state(BtnFunc function)
{
    previous_state = current_state;
    
    switch(function)
    {
	    case BtnFunc::STOP :
			if(is_playing)
			{
				is_playing = false;
				state_clear(State::PLAYING);
				state_set(State::STOPPED);
			}
			break;
			
		case BtnFunc::PLAY :
			if(!is_playing)
			{
				is_playing = true;
				state_set(State::PLAYING);
				state_clear(State::STOPPED);
			}
			break;
	    
	    case BtnFunc::REC :
			if(!rec_armed)
			{
				rec_armed = true;
				state_set(State::RECORDING);
			}
			else
			{
				rec_armed = false;
				state_clear(State::RECORDING);
			}
			break;
	    
	    case BtnFunc::LOOP :
			if(!is_looping)
			{
				is_looping = true;
				state_set(State::LOOPING);
			}
			else
			{
				is_looping = false;
				state_clear(State::LOOPING);
			}
			break;
    }
}

void state_set(State state)
{
	current_state |= (0x01 << (uint8_t)state);
}

void state_clear(State state)
{
    current_state &= ~(0x01 << (uint8_t)state);	
}


void send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
	uart_putc(CC_MESSAGE | channel);
	uart_putc(cc);
	uart_putc(value);
}