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
    TMOD = 0x22;                           
    TH1  = 0x00;          
    TL1  = 0x00;          
    TCON = 0x10;          
    ET1  = 1;                    
	  
    EX0 = 1;              
    IT0 = 1;              
    
    SCON  = 0x40;         
    TCLK  = 1;            
    T2CON = 0x10;         
    RCAP2H = TH2 = 0xFF;  
    RCAP2L = TL2 = 0xEE;  
    TR2 = 1;              
    TR1 = 1;              
    EA = 1;    	          
  }

unsigned char Szukaj(unsigned char Kod_przycisku) 
 {
   unsigned char data Value, licznik;                               
   unsigned char code Koder[] = {0xE7,0xEB,0xED,0xD7,0xDB,0xDD,0xB7,0xBB,0xBD,0x77,0x7B,0x7D};    
   unsigned char code New_Char[] = {'1','2','3','4','5','6','7','8','9','*','0','#'};                         
   
   licznik=0x00;                                                    
   while((Koder[licznik]!=Kod_przycisku)&&(licznik<No_Key)){licznik++;} 
   Value = No_Key;
   if(licznik!=No_Key){Value = New_Char[licznik];}                  
   return Value;                                                    
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
 
 void sleep2(void)              
 {
   unsigned char data x,y;
   for(x=0;x<100;x++) 
		for(y=0;y<100;y++){;}
 }

void ISR_INT0 (void) interrupt 0	  
  {
    unsigned char data Value, Key, i = 0;
		TR1 = 0;
		Key = Port;
    Value = Szukaj(Key);     
    if(Value != No_Key)        
      {
        if(Key_1 == 0) Counter = 1;
				else if(Key_2 == 0) Counter = 2;
				else if(Key_3 == 0) Counter = 3;
				else if(Key_4 == 0) Counter = 4;
				LED = ON; sleep2(); LED = OFF; sleep(); sleep(); sleep();
				while(i < Counter)
				{
					LED = ON; sleep(); LED = OFF; sleep();
					i++;
				}
				sleep(); sleep();
				LED = ON; sleep(); LED = OFF; 
				TI = 0;             
        SBUF = Value;      
           
				while(TI==0){;}    
         
				 IT0 = 0;             
         IT0 = 1;                         
      }  
		
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