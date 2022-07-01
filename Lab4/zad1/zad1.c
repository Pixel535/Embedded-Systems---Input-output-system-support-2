#include <REGX52.H>

sbit LEDZielona = P2^0; 
sbit LEDZolta = P2^7; 
sbit SW = P3^2;

void timer0_isr() interrupt 1
{
TH0 = 0XF9; 
TL0 = 0X40;
LEDZielona =!LEDZielona; 
}

void main()
{
   SW = 1; 
   TMOD = 0x01; 
   TH0 = 0XFF; 
   TL0 = 0X00;
   ET0 = 1; 
   EA = 1; 
   TR0 = 1; 
   while(1)
    {
       LEDZolta= SW; 
    }
}