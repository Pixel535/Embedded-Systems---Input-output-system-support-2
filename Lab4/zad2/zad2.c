#include <REGX52.H>

#define Port P2
#define No_Key 0x0C
#define Key_is_Off P3_2==1
#define ON  0
#define OFF 1

volatile unsigned char data ind;
unsigned char Counter;
sbit LED = P0^0;

sbit Key_1 = Port^4;
sbit Key_2 = Port^5;
sbit Key_3 = Port^6;
sbit Key_4 = Port^7;


void Init(void)						
  {
    TMOD = 0x22;          // Konfiguracja Timera 1                      
    TH1  = 0x00;          // konfiguracja Timera 1
    TL1  = 0x00;          // konfiguracja Timera 1
    TCON = 0x10;          // konfiguracja Timera 1
    ET1  = 1;             // dolaczenie przerwania od Timer1 do systemu przerwan         
	  
    EX0 = 1;              // wlaczenie przerwania od INT0
    IT0 = 1;              // przerwanie aktywne zboczem opadajacym
    
    SCON  = 0x40;         // serial control - 8-Bit var. Baud Rate
    TCLK  = 1;            // dolacz taktowanie do nadajnika portu szeregowego
    T2CON = 0x10;         // Timer2 jako Baud Rate Generator 2400b/sek
    RCAP2H = TH2 = 0xFF;  // wartosc poczatkowa licznika T2 - czesc H
    RCAP2L = TL2 = 0xEE;  // wartosc poczatkowa licznika T2 - czesc L
    TR2 = 1;              // uruchom Timer 2 - taktowanie nadajnika RS 
    TR1 = 1;              // wlaczenie timera 1
    EA = 1;    	          // wlaczenie systemu przerwan  	          
  }

void ISR_Timer1 (void) interrupt 3 	
  {
    unsigned char code Kod[] = {0x7F,0xBF,0xDF,0xEF};     
    Port = Kod[ind];                                      
    if(Key_is_Off) {if(ind<3) {ind++;} else {ind=0;}}                                                                                                                
  }

void sleep(void)              
 {
   unsigned char data x,y;
   for(x=0;x<140;x++) 
		for(y=0;y<140;y++){;}
 }

void ISR_INT0 (void) interrupt 0	  
  {
    unsigned char data Key, i = 0;
		TR1 = 0;
		Key = Port;
		if(Key_1 == 0) Counter = 1;
		else if(Key_2 == 0) Counter = 2;
		else if(Key_3 == 0) Counter = 3;
		else if(Key_4 == 0) Counter = 4;
		while(i < Counter)
		{
			LED = ON; sleep(); LED = OFF; sleep();
			i++;
		}
		sleep(); sleep();
		LED = ON; sleep(); LED = OFF;
		TR1 = 1;
  }  
void Default(void)
  {
    ind  = 0;              
    Port = 0x7F; 
  }

void main(void)  
  {
    Default();   
    Init();      
    while(1) {;} 
  }