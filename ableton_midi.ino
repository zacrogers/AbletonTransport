#include "dip_switch.h"

/* MIDI command defines */
#define NOTE_OFF      (uint16_t)0x80
#define NOTE_ON       (uint16_t)0x90
#define AFTERTOUCH    (uint16_t)0xA0
#define CC_MESSAGE    (uint16_t)0xB0
#define PATCH_CHANGE  (uint16_t)0xC0
#define CHAN_PRESSURE (uint16_t)0xD0
#define PITCH_BEND    (uint16_t)0xE0

#define NUM_BUTT0NS  6

/* States for leds. 
   Note: RECORDING and LOOPING can be active at the 
         same time as PLAYING and STOPPED
*/
enum class State
{
    STOPPED = 0,  
    PLAYING,
    RECORDING,
    LOOPING  
};

/* Pin defines for buttons */
enum class BtnNum
{
    STOP = 2,
    PLAY = 3,
    REC  = 4,
    LOOP = 5,
    RWD  = 6,
    FFWD = 7    
};

const BtnNum button_pins[] = {BtnNum::STOP, BtnNum::PLAY, BtnNum::REC, 
                              BtnNum::LOOP, BtnNum::RWD,  BtnNum::FFWD}; 


/* Button cc message numbers  */
enum class CCNum
{
    STOP = 0,
    PLAY = 1,
    REC  = 2,
    LOOP = 3,
    RWD  = 4,
    FFWD = 5           
};

const CCNum cc_numbers[] = {CCNum::STOP, CCNum::PLAY, CCNum::REC, 
                            CCNum::LOOP, CCNum::RWD,  CCNum::FFWD}; 

uint8_t led_pins[NUM_BUTT0NS] = {A1, A0, A5, A4, A3, A2};

/* Master midi channel */
DipSwitch dip_switch = DipSwitch(8, 9, 10, 11);
uint8_t midi_channel = 0;

/* Led state booleans */
bool is_playing = false;
bool is_looping = false;
bool rec_armed  = false;

uint8_t curr_state = 0;
uint8_t prev_state = 0;


void setup() 
{
    midi_channel = dip_switch.get_bin_state();
    
    Serial.begin(115200);
    
    for(uint8_t i = 0; i < NUM_BUTT0NS; i++)
    {
        pinMode((uint8_t)button_pins[i], INPUT);
        pinMode(led_pins[i], OUTPUT);
    }
}


void loop() 
{
    poll_buttons();
    if(prev_state != curr_state)
    {
        set_leds();
    }
}

void poll_buttons(void)
{
    for(uint8_t btn = 0; btn < NUM_BUTT0NS; btn++)
    {
        if(digitalRead((uint8_t)button_pins[btn]) == HIGH)
        {
            update_state(button_pins[btn]);
            send_cc((uint8_t)cc_numbers[btn], 127, midi_channel);
            delay(250);
        }
    }
}


void update_state(BtnNum btn)
{
    prev_state = curr_state;
    
    switch(btn)
    {
       case BtnNum::STOP : 
            if(is_playing)
            {
                is_playing = false;
                clear_state(State::PLAYING);
                set_state(State::STOPPED);
            }
            break;
            
       case BtnNum::PLAY : 
            if(!is_playing)
            {
                is_playing = true;
                set_state(State::PLAYING);
                clear_state(State::STOPPED);
            }
            break;
            
       case BtnNum::REC :
            if(!rec_armed)
            {
                rec_armed = true;
                set_state(State::RECORDING);
            }
            else
            {
                rec_armed = false;
                clear_state(State::RECORDING);            
            }
            break;
            
       case BtnNum::LOOP :
            if(!is_looping)
            {
                is_looping = true;
                set_state(State::LOOPING);
            }
            else
            {
                is_looping = false;
                clear_state(State::LOOPING);            
            }
            break; 
    }
}


void set_state(State st)
{
    curr_state |= (0x01 << (uint8_t)st);
}


void clear_state(State st)
{
    curr_state &= ~(0x01 << (uint8_t)st);
}


void set_leds(void)
{
    for(int led = 0; led < NUM_BUTT0NS; led++)
    {
        if(curr_state & (0x01 << led))
        {
            digitalWrite(led_pins[led], HIGH);
        }
        else
        {
            digitalWrite(led_pins[led], LOW);
        }
    }
}


void send_cc(uint8_t cc_num, uint8_t value, uint8_t channel)
{
    Serial.write(CC_MESSAGE | channel);
    Serial.write(cc_num);
    Serial.write(value);    
}

