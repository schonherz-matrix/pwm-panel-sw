#include <16F882.h>
#device adc=8




#zero_ram

//#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES WDT                 	//Use Watch Dog Timer
//#FUSES INTRC_IO              	//Internal RC Osc, no CLKOUT
#FUSES HS
#FUSES PUT                   	//Power Up Timer
#FUSES MCLR                  	//Master Clear pin enabled
#FUSES NOPROTECT             	//Code not protected from reading
//#FUSES NOBROWNOUT            	//No brownout reset
#FUSES BROWNOUT
#FUSES BORV21                
//#FUSES BORV19                
//#FUSES PLLEN                 
#FUSES NODEBUG               	//No Debug mode for ICD
//#FUSES NOVCAP                
#FUSES RESERVED              	//Used to set the reserved FUSE bits

#use delay(clock=16000000)

/*
#zero_ram

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES INTRC_IO                 //Internal RC Osc, no CLKOUT 
//#FUSES XT                    	//Crystal osc <= 4mhz for PCM/PCH , 3mhz to 10 mhz for PCD
#FUSES NOPUT                 	//No Power Up Timer
#FUSES NOPROTECT             	//Code not protected from reading
#FUSES NOBROWNOUT            	//No brownout reset
//#FUSES NOLVP                 	//No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
//#FUSES NOCPD                 	//No EE protection
//#FUSES NOWRT                 	//Program memory not write protected
#FUSES NODEBUG               	//No Debug mode for ICD
#FUSES RESERVED              	//Used to set the reserved FUSE bits

#use delay(clock=8000000)
*/

/*#define RTC_RST   PIN_A0
#define RTC_SCLK   PIN_A1
#define RTC_IO   PIN_A2*/
/*#define I2C_SCL   PIN_C3
#define I2C_SDA   PIN_C4*/
/*#use i2c(Slave,Fast,sda=PIN_C4,scl=PIN_C3,address=0x10)*/


//#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=pc)
//#use rs232(baud=128K,xmit=PIN_C6,rcv=PIN_C7,bits=9)
//#use rs232(baud=125000,xmit=PIN_C6,rcv=PIN_C7,bits=9)
//???????????
#use rs232(baud=125000,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)//?????????
//???????????



