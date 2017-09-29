
#include <stdbool.h>
#include <stdint.h>
#define LCDPORT GPIO_PORTB_BASE
#define LCDPORTENABLE SYSCTL_PERIPH_GPIOB
#define RS GPIO_PIN_0
#define E GPIO_PIN_1
#define D4 GPIO_PIN_4
#define D5 GPIO_PIN_5
#define D6 GPIO_PIN_6
#define D7 GPIO_PIN_7
#include "inc/hw_ints.h"
#include "inc/hw_types.h"//Genel türleri ve makroları tanımlar
#include "inc/hw_memmap.h"//Anıların ve çevre birimlerinin taban adresini tanımlar
#include "driverlib/gpio.h"//GPIO API'sı için Tanımlar ve Makrolar
/* Sistem kontrol sürücüsü için prototipler */
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"
typedef unsigned char LCDubyte;
void Export(char kor_x, char kor_y){

        if(kor_x==1)
        	Instruction(0x80+((kor_y-1)%16));
        else
        	Instruction(0xC0+((kor_y-1)%16));
}

void Directive (LCDubyte LCDData){//EMİR

	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,  (LCDData) );
	        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x02);
	        SysCtlDelay(10);
	        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);

	        Ms_Delay(15);
}

void init() {

        SysCtlPeripheralEnable(LCDPORTENABLE);//UART pimlerini periferi kontrollü yap
        GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);//Konsolu G / Ç için UART'ı başlat

        Ms_Delay(15);

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0,  0x00 );
        Directive(0x30);
        Directive(0x30);
        Directive(0x30);
        Directive(0x20);
        Ms_Delay(15);

        Instruction(0x28);
        Instruction(0xC0);
        Instruction(0x06);
        Instruction(0x80);
        Instruction(0x28);
        Instruction(0x0f);
        Clear();
}

void Clear(void){
		Instruction(0x01);
		SysCtlDelay(10);
}

void Port_Init_A() {
   volatile unsigned long Gecikme;
   SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
   Gecikme = SYSCTL_RCGC2_R;

   GPIO_PORTA_AFSEL_R &= ~0xFF; // alternatif fonksiyo kapali
      GPIO_PORTA_DEN_R |= 0xFF; // hepsi aktif
   GPIO_PORTA_DIR_R |= 0xFF; // 0xff == 0b11111111, hepsini cikis olarak ayarla

}



void Process(unsigned char c) {

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, (c & 0xf0) );
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x01);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);

        Ms_Delay(15);

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, (c & 0x0f) << 4 );
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x01);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);

        Ms_Delay(15);

}


void Register ( char* come){

        while(*come)
        Process(*come++);

}

void Ms_Delay(unsigned int delay){

		unsigned int k, l;

		for (k = 0; k < delay; k++)

		for (l = 0; l < 275; l++);//Calibrated for a 1 ms delay in MPLAB
}


void Instruction(unsigned char g) {

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, (g & 0xf0) );
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x00);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);

        Ms_Delay(15);

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, (g & 0x0f) << 4 );
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x00);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x02);

        SysCtlDelay(10);

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);

        Ms_Delay(15);

}
int main(void) {


        SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
        //Saatlemeyi 16 MHz'de doğrudan kristalden çalışacak şekilde ayarlayın

        while(1){

        int sayi;
       sayi=rand()%5;


       Port_Init_A();
        init();

        if(sayi==1){
        	GPIO_PORTA_DATA_R |= 0b00001000; //kırmızı yandı

        	        Export(1,1);
        	        Register("  Kocaeli uni");
        	        Ms_Delay(20000);

        	        Export(2,1);
        	        Register(" Muhammed Sara");

        	        Ms_Delay(30000);
        	        Clear();
        	    	GPIO_PORTA_DATA_R &= ~(0b00001000);//kırmızı söndü
        }
        else if(sayi==3){

        	GPIO_PORTA_DATA_R |= 0b00100000; //beyaz yandı

        	Export(2,1);
        	Register(" Muhammed Sara");

        	Ms_Delay(20000);
       	     Export(1,1);
       	  Register("  Kocaeli uni");

       	Ms_Delay(30000);
       	Clear();
       	GPIO_PORTA_DATA_R &= ~(0b00100000);//beyaz söndü

       }

        else if(sayi==2){
        	GPIO_PORTA_DATA_R |= 0b01000000; //yeşil yandı

       	for(int i=12;i>=0; i--){
       		char yazi[]="  Kocaeli uni";
        	        	        Export(1,1);
        	        	        Register(yazi+i);
        	        	        Ms_Delay(5000);
        	        	        int j=0;

        	        	}

       	Ms_Delay(20000);
    	for(int i=14;i>=0; i--){
           		char yazi[]=" Muhammed Sara";
            	        	        Export(2,1);
            	        	        Register(yazi+i);
            	        	        Ms_Delay(5000);
            	        	        int j=0;

            	        	}
        	Ms_Delay(30000);
        	Clear();

        		GPIO_PORTA_DATA_R &= ~(0b01000000);//yeşil söndü

               }
        else if(sayi==4){

        	GPIO_PORTA_DATA_R |= 0b10000000; //mavi yandı

        	for(int i=16;i>=3; i--){
        		Export(1,i);
        		Register("Kocaeli uni        ");
        		Ms_Delay(5000);

        	        int j=0;
        	        while(j<16){
        	        	Export(1,j);
        	        	Register("");
        	        	 j++;
        	        }


        	}
        	Ms_Delay(20000);
        	for(int i=16;i>=2; i--){
        		Export(2,i);
        		Register("Muhammed Sara      ");
        		Ms_Delay(5000);

        	        	        int j=0;
        	        	  while(j<16){
        	        		  Export(1,j);
        	        		  Register("");
        	        	j++;
        	      }
        	        	}

        	Ms_Delay(30000);
               	Clear();

        GPIO_PORTA_DATA_R &= ~(0b10000000);//mavi söndü
        }

}
}
