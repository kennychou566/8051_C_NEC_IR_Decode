#include<8052.h> 
#include <STC89xx.h>
#define led P0
SBIT(sw1,_P2,0);
SBIT(sw2,_P2,1);
SBIT(sw3,_P2,2);
SBIT(sw4,_P2,3);

unsigned char ch[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//void delay (int);
void display (unsigned long int);
void sdelay (char);

/*void delay (int d)
{
	unsigned char i;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--);
		for(i=248;i>0;i--);
	}
}*/

void sdelay (char d)
{
	for(;d>0;d--);
}

void display (unsigned long int n)
{
	led=ch[n/1000];
	sw1=1;
	sdelay(30);
	sw1=0;
	
	led=ch[(n/100)%10];
	sw2=1;
	sdelay(30);
	sw2=0;
	
	led=ch[(n/10)%10];
	sw3=1;
	sdelay(30);
	sw3=0;
	
	led=ch[n%10];
	sw4=1;
	sdelay(30);
	sw4=0;
}

void main(void)
{
	unsigned long int i=1;
	
	while(1)
	{
		i=++i;
		display(i);
		if(i==9999)
			i=0;
	}
}