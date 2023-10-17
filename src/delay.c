#include "delay.h"
void delay_ms(unsigned int i){
    unsigned char j;
    for(i; i > 0; i--)
    for(j = 255; j > 0; j--);
}
void delay_s(unsigned int i){
    delay_ms(i*1000);
}
void delay_m(unsigned int i){
     delay_s(i*60);
}
void delay_h(unsigned int i){
    delay_m(i*60);
}
void v_delay(int d)
{
    unsigned char i;
    for (; d > 0; d--) {
        for (i = 250; i > 0; i--)
            ;
        for (i = 248; i > 0; i--)
            ;
    }
}

void sdelay(char d)
{
    for (; d > 0; d--)
        ;
}