/*
 * main.h
 *
 * Created: 15-Jan-20 3:25:03 PM
 *  Author: Zac
 */ 
#ifndef MAIN_H_
#define MAIN_H_

#define NUM_BTNS 6
#define NUM_LEDS 6

/* MIDI command defines */
#define NOTE_OFF      (uint16_t)0x80
#define NOTE_ON       (uint16_t)0x90
#define AFTERTOUCH    (uint16_t)0xA0
#define CC_MESSAGE    (uint16_t)0xB0
#define PATCH_CHANGE  (uint16_t)0xC0
#define CHAN_PRESSURE (uint16_t)0xD0
#define PITCH_BEND    (uint16_t)0xE0

/* States for led display. 
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

enum class BtnFunc
{
	STOP = 0,
	PLAY,
	REC,
	LOOP,
	RWD,
	FFWD
};


/* Pin defines for buttons */
enum class BtnNum
{
	STOP = 4, 
	PLAY = 3,
	REC  = 5, 
	LOOP = 2, 
	RWD  = 6,
	FFWD = 7
};


/* Function prototypes */
void poll_buttons (void);
void leds_set     (void);
void start_seq    (void);
void update_state (BtnFunc function);
void state_set    (State state);
void state_clear  (State state);
void send_cc      (uint8_t cc, uint8_t value, uint8_t channel);

#endif /* MAIN_H_ */