/*
 * gpio.h
 *
 * Created: 15-Jan-20 3:39:04 PM
 *  Author: Zac
 */ 
#ifndef GPIO_H_
#define GPIO_H_
#include <avr/io.h>
#include <stdint.h>

/* Macros to get data direction, and input register from port definition */
#define DDR(x)  (*(&x - 1))
#define PIN(x)  (*(&x - 2))
#define PORT(x) (x)

namespace z_lib
{
	enum class Mode
	{
		IN = 0,
		OUT,
		PULLUP		
	};
		
	class Gpio
	{
		public:
			Gpio(volatile uint8_t *port, uint8_t pin, Mode mode);
			
			void    set         (void);
			void    clear       (void);
			void    toggle      (void);
			uint8_t read        (void);
			void    set_pullup  (void);
		
		private:
			volatile uint8_t *port;
			uint8_t pin;
	};
}

#endif /* GPIO_H_ */