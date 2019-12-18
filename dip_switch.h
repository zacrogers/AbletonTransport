#ifndef DIP_SWITCH_H
#define DIP_SWITCH_H


#define NUM_SWITCHES 4

class DipSwitch{
    public:
        DipSwitch(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4)
        {
            _pins[0] = p1;
            _pins[1] = p2;
            _pins[2] = p3;
            _pins[3] = p4;
            
            for(int i = 0; i < NUM_SWITCHES; i++)
            {
                pinMode(_pins[i], INPUT);
            }
        }
        
        uint8_t get_bin_state()
        {
            uint8_t state = 0;
            for(int i = 0; i < NUM_SWITCHES; i++)
            {
                if(digitalRead(_pins[i]) == HIGH)
                {
                    state |= (0x01 << i);
                }
            }            
            return state;
        }
        
    private:
        uint8_t _pins[NUM_SWITCHES] = {0, 0, 0, 0};
};

#endif /*DIP_SWITCH_H*/
