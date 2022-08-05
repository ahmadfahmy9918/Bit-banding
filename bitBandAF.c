#include "LPC17xx.h" // Device header
#include "Board_LED.h"
#include <stdio.h>
#include <stdlib.h>


//------- ITM Stimulus Port definitions for printf ------------------- //
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
//------------------------------------------------------------------- //

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))

volatile unsigned long * bit;
#define ADCR_Bit24   (*((volatile unsigned long *)0x42680060))

#define GPIO_Bit28	 (*((volatile unsigned long *)0x23380670)) 

#define GPIO_Bit2	 	 (*((volatile unsigned long *)0x23380A08)) 
	
#define LPC_GPIO(n)		((LPC_GPIO_TypeDef *)(LPC_GPIO0_BASE + 0x00020*n))

void delay(unsigned int e){
	int c, d;
   
   for (c = 1; c <= e; c++)
       for (d = 1; d <= e; d++)
       {}
}

int barrelShift(int onOff){
	//barrel shifter code

	 int r1 = 1, r2 = 0, r3 = 5;
	
	while(r2 < 1){
		if((r1 - r2) > 0){
		  r1 = r1 + 2;
			r2 = (r1 + (r3*4)) / (r1 + (r3*4));
			r3 = r3/2;
	}
		else{
			r2 = onOff;
		}
	}
	return(r2);
}

void condEx1(int port, int bit, int low, int high, int onOff) {
//conditional execution code for masking method
	
	if(port == 1 && bit == 28) {
		if(onOff == 1) {
			LPC_GPIO(port)->FIOMASK |=  ( low << high);                  
			printf("Addr: 0x%08X \n LED ON\n", LPC_GPIO(port)->FIOMASK);
		}
		
		else {
			LPC_GPIO(port)->FIOMASK &= ~( low << high);                  
			printf("Addr: 0x%08X \n LED OFF\n", LPC_GPIO(port)->FIOMASK);
		}
	}
	
	else {
		if(onOff == 1) {
			LPC_GPIO(port)->FIOMASK |=  ( low << high);                  
			printf("Addr: 0x%08X \n LED ON\n", LPC_GPIO(port)->FIOMASK);
		}
		
		else {
			LPC_GPIO(port)->FIOMASK &= ~( low << high);                  
			printf("Addr: 0x%08X \n LED OFF\n", LPC_GPIO(port)->FIOMASK);
		}
	}
}


void condEx2(volatile unsigned long * bit, int port, int bitNum, int onOff) {
//conditional execution code for function method
	
	if(port == 1 && bitNum == 28) {
		bit = &BitBand(&LPC_GPIO(port) -> FIOMASK, bitNum);
		
		if(onOff == 1) {	
			*bit = 1;
			printf("Addr: 0x%08X \n LED ON\n", LPC_GPIO(port)->FIOMASK);
		}
		
		else {
			*bit = 0;
			printf("Addr: 0x%08X \n LED OFF\n", LPC_GPIO(port)->FIOMASK);
		}
	}
	
	else {
		bit = &BitBand(&LPC_GPIO(port) -> FIOMASK, bitNum);
		
		if(onOff == 1) {
			*bit = 1;
			printf("Addr: 0x%08X \n LED ON\n", LPC_GPIO(port)->FIOMASK);
		}
		
		else {
			*bit = 0;
			printf("Addr: 0x%08X \n LED OFF\n", LPC_GPIO(port)->FIOMASK);
		}
	}
}


void condEx3(int port, int bitNum, int onOff) {
//conditional execution code for function method
	
	if(port == 1 && bitNum == 28) {
		if(onOff == 1) {	
			GPIO_Bit28 = onOff;
			printf("Addr: 0x%08X \n LED ON\n", LPC_GPIO(port) -> FIOMASK);
		}
		
		else {
			GPIO_Bit28 = onOff;
			printf("Addr: 0x%08X \n LED OFF\n", LPC_GPIO(port) -> FIOMASK);
		}
	}
	
	else {
		if(onOff == 1) {
			GPIO_Bit2 = onOff;
			printf("Addr: 0x%08X \n LED ON\n", LPC_GPIO(port) -> FIOMASK);
		}
		
		else {
			GPIO_Bit2 = onOff;
			printf("Addr: 0x%08X \n LED OFF\n", LPC_GPIO(port) -> FIOMASK);
		}
	}
}

void maskMode(void) {
	//mask mode
	printf("\nMask mode: \n");
	
	//LED 1
	printf("\nLED 1\n");
	condEx1(1, 28, 1, 28, 0); //turn Port1.28 LED off
	delay(2000);
	condEx1(1, 28, 1, 28, 1); //turn Port1.28 LED on
	delay(2000);
	
	//LED 2
	printf("\nLED 2\n");
	condEx1(2, 2, 1, 2, 0); //turn Port2.2 LED off
	delay(2000);
	condEx1(2, 2, 1, 2, 1); //turn Port2.2 LED on
	delay(2000);
	
}

void functionMode(void) {
	//function mode	
	printf("\nfunction mode: \n");
	
	//LED 1
	printf("\nLED 1\n");
	condEx2(bit, 1, 28, 0); //turn Port1.28 LED off 
	delay(2000);
	condEx2(bit, 1, 28, 1); //turn Port1.28 LED on
	delay(2000);
	
	//LED 2
	printf("\nLED 2\n");
	condEx2(bit, 2, 2, 0); //turn Port2.2 LED off 
	delay(2000);
	condEx2(bit, 2, 2, 1); //turn Port2.2 LED on
	delay(2000);
	
}

void bitBandMode(void) {
	//bit band mode
	printf("\nBit Masking mode: \n");	
	
	int signalON = 1;
	signalON = barrelShift(signalON);
	
	//LED 1
	printf("\nLED 1\n");
	condEx3(1, 28, 0);
	delay(2000);
	condEx3(1, 28, 1);
	delay(2000);
	
	//LED 2
	printf("\nLED 2\n");
	condEx3(2, 2, 0);
	delay(2000);
	condEx3(2, 2, 1);
	delay(2000);
	
}

 
int main(void){
	
	maskMode();
	functionMode();
	bitBandMode();
	
	
}
