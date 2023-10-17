#include <stdio.h>
#include <stdint.h>
#include <8052.h>
#include <STC89xx.h>
#include <lint.h>
#include <delay.h>
#include <config.h>
#include <serial.h>
#include <ir-nec.h>

#define led P0
SBIT(sw1, _P2, 0);
SBIT(sw2, _P2, 1);
SBIT(sw3, _P2, 2);
SBIT(sw4, _P2, 3);

unsigned long int d1 = 0;
unsigned long int d2 = 0;
unsigned long int d3 = 0;
unsigned long int d4 = 0;
uint32_t keycode;
unsigned char buf[5];
unsigned char ch[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; // 0-9[9],a[10],b[11],c[12],d[13],e[14],f[15]

// unsigned double low;
void display(unsigned long int, unsigned long int, unsigned long int, unsigned long int);
void char2display(uint32_t); // 字元拆解顯示


void display(unsigned long int n1, unsigned long int n2, unsigned long int n3, unsigned long int n4)
{
    led = ch[n1];
    sw1 = 0;
    delay_ms(5);
    sw1 = 1;

    led = ch[n2];
    sw2 = 0;
    delay_ms(5);
    sw2 = 1;

    led = ch[n3];
    sw3 = 0;
    delay_ms(3);
    sw3 = 1;

    led = ch[n4];
    sw4 = 0;
    delay_ms(3);
    sw4 = 1;
}
int putchar(int c)
{
    while (!(SCON & 0x02));
    SCON &= ~0x02;
    SBUF = (char)c;
    return c;
}
void char2display(uint32_t kc) // 字元拆解顯示
{
    
    buf[0] = kc >> 24;
    buf[1] = kc >> 16;
    buf[2] = kc >> 8;
    buf[3] = kc ;
 
   // printf("buffer is %d,%d\n\r", buf[2], buf[3]);
    unsigned lcm[4];
    lcm[0]=buf[2] >> 4;
    lcm[1]=buf[2] - (lcm[0] * 16) ;
    lcm[2]=buf[3] >> 4;
    lcm[3]=buf[3] - (lcm[2] * 16) ;
   // printf("LCM is %d,%d\n\r", lcm[0], lcm[1]);
    if(buf[3]>15)
    {
        d1=lcm[0];
        d2=lcm[1];
        d3=lcm[2];
        d4=lcm[3];
    }else{
        d3=0;d4=buf[3];  
        //printf("lowbytes(<15) is %d,%d\n\r", d3, d4);
    }
    
    
}
void main(void)
{

    serial_init();
    ir_nec_init();
    while (1) {
        
        if (ir_nec_available()) {
            keycode = ir_nec_get_code();
            printf("NEC Remote code : 0x%08lX\n\r", keycode);
            char2display(keycode);
            P3_6 = 0;//蜂鳴器致動
            delay_ms(50);
            P3_6 = 1;
        }
        display(d1, d2, d3, d4);
    }
}
