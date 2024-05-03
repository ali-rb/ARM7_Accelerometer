/*
  ADXL335 accelerometer interfacing with LPC2148(ARM7)
  Ali Ranjbar
*/

#include <lpc214x.h>
#include <stdint.h>
#include <stdio.h>

void delay_ms(uint16_t j)
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);    
	}
}

void UART0_init(void)
{
	PINSEL0 = PINSEL0 | 0x00000005;	
	U0LCR = 0x83;	
	U0DLM = 0x00;	
	U0DLL = 0x61;	
	U0LCR = 0x03; 
}

void UART0_TxChar(char ch) 
{
	U0THR = ch;
	while( (U0LSR & 0x40) == 0 );	
}

void UART0_SendString(char* str) 
{
	uint8_t i = 0;
	while( str[i] != '\0' )
	{
		UART0_TxChar(str[i]);
		i++;
	}
}

uint32_t ADC_READ1(void)
{
	uint32_t x_out;
	PINSEL1 = (PINSEL1 | 0x01000000); 
	AD0CR = 0x00200402; 
	AD0CR = AD0CR | (1<<24);
	while ( !(AD0DR1 & 0x80000000) );
	x_out = AD0DR1;
	x_out = (x_out>>6);
	x_out = (x_out & 0x000003FF);	
	return x_out;
}

uint32_t ADC_READ2(void)
{
	uint32_t y_out;
	PINSEL1 = (PINSEL1 | 0x04000000); 
	AD0CR = 0x00200404; 
	AD0CR = AD0CR | (1<<24); 
	while ( !(AD0DR2 & 0x80000000) );
	y_out = AD0DR2;
	y_out = (y_out>>6);
	y_out = (y_out & 0x000003FF);	
	return y_out;
}

uint32_t ADC_READ3(void)
{
	uint32_t z_out;
	PINSEL1 = (PINSEL1 | 0x10000000); 
	AD0CR = 0x00200408; 
	AD0CR = AD0CR | (1<<24); 
	while ( !(AD0DR3 & 0x80000000) );
	z_out = AD0DR3;
	z_out = (z_out>>6);
	z_out = (z_out & 0x000003FF);	
	return z_out;
}

int main(void)
{
	char result[70];
	uint32_t x_out1, y_out1, z_out1;
	double x_val, y_val, z_val;
	UART0_init();
	while(1)
	{
		x_out1 = ADC_READ1();
		delay_ms(10);
		y_out1 = ADC_READ2();
		delay_ms(10);
		z_out1 = ADC_READ3();
		delay_ms(10);
		x_val = ( ( ( (x_out1 * 3.3) / 1024 ) - 1.6 ) / 0.33 );
		y_val = ( ( ( (y_out1 * 3.3) / 1024 ) - 1.6 ) / 0.33 );
		z_val = ( ( ( (z_out1 * 3.3) / 1024 ) - 1.6 ) / 0.33 );
		sprintf(result,"x=%lfg, y=%lfg, z=%lfg",x_val,y_val,z_val);
		UART0_SendString(result);
		UART0_SendString("\r\n");
		delay_ms(1000);
	}
}

