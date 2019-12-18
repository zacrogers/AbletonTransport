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
enum class PlayState
{
    PLAYING,
    STOPPED,   
    RECORDING,
    LOOPING  
};

/* Pin defines for buttons */
enum class BtnNum
{
    STOP = 2,
    PLAY,
    REC,
    LOOP,
    RWD,
    FFWD    
};

/* Button cc message numbers  */
enum class CCNum
{
    STOP = 0,
    PLAY,
    REC,
    LOOP,
    RWD,
    FFWD            
};

const BtnNum button_pins[] = {BtnNum::STOP, 
                              BtnNum::PLAY, 
                              BtnNum::REC, 
                              BtnNum::LOOP, 
                              BtnNum::RWD, 
                              BtnNum::FFWD}; 

const CCNum cc_numbers[] = {CCNum::STOP, 
                            CCNum::PLAY, 
                            CCNum::REC, 
                            CCNum::LOOP, 
                            CCNum::RWD, 
                            CCNum::FFWD}; 

uint8_t led_pins[NUM_BUTT0NS] = {A1, A0, A5, A4, A3, A2};

DipSwitch dip_switch = DipSwitch(8, 9, 10, 11);

uint8_t midi_channel = 0;

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
    set_leds();
}

void poll_buttons(void)
{
    for(uint8_t btn = 0; btn < NUM_BUTT0NS; btn++)
    {
        if(digitalRead((uint8_t)button_pins[btn]) == HIGH)
        {
            send_cc((uint8_t)cc_numbers[btn], 127, midi_channel);
            delay(250);
        }
    }
}

void set_leds(void)
{
    
}

void send_cc(uint8_t cc_num, uint8_t value, uint8_t channel)
{
    Serial.write(CC_MESSAGE | channel);
    Serial.write(cc_num);
    Serial.write(value);    
}


