#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

int main(void)
{
    int rate = 0;
    int colour = 0;
    int switch1Value = 1;
    int switch2Value = 1;
    int delay;

    SYSCTL_RCGC2_R |= 0x00000020;

    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R |= 0x01;

    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;

    while(1)
    {
        switch(rate){
            case 0:
                delay = 1000;
                break;
            case 1:
                delay = 500;
                break;
            case 2:
                delay = 250;
                break;
            default:
                delay = 250;
                break;
        }

        switch(colour){
            case 0:
                GPIO_PORTF_DATA_R = 0x02;
                break;
            case 1:
                GPIO_PORTF_DATA_R = 0x08;
                break;
                case 2:
                GPIO_PORTF_DATA_R = 0x04;
                break;
                case 3:
                GPIO_PORTF_DATA_R = 0x0E;
                break;
            default:
                GPIO_PORTF_DATA_R = 0x0E;
                break;
        }
        for(int i = 0; i < delay; i++)
        {
            if((GPIO_PORTF_DATA_R & 0x10) == 0 &&
               switch1Value != 0)
            {
                rate++;

                if(rate > 2)
                    rate = 0;
            }

            if((GPIO_PORTF_DATA_R & 0x01) == 0 &&
               switch2Value != 0)
            {
                colour++;

                if(colour > 3)
                    colour = 0;
            }

            switch1Value = GPIO_PORTF_DATA_R & 0x10;
            switch2Value = GPIO_PORTF_DATA_R & 0x01;


            for(int j = 0; j < 1000; j++)
            {
            }
        }
        GPIO_PORTF_DATA_R = 0x00;
        for(int i = 0; i < delay; i++)
        {
            if((GPIO_PORTF_DATA_R & 0x10) == 0 &&
               switch1Value != 0)
            {
                rate++;

                if(rate > 2)
                    rate = 0;
            }

            if((GPIO_PORTF_DATA_R & 0x01) == 0 &&
               switch2Value != 0)
            {
                colour++;

                if(colour > 3)
                    colour = 0;
            }

            switch1Value = GPIO_PORTF_DATA_R & 0x10;
            switch2Value = GPIO_PORTF_DATA_R & 0x01;

            for(int j = 0; j < 1000; j++)
            {
            }
        }
    }
}
