/*
 * GccApplication1.c
 *
 * Created: 5/1/2017 3:00:24 PM
 *  Author: WhiteWalker
 */ 


#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

//#include "LCD_16x2_H.h"
#define iport PORTB
#define ipin PINB
#define idd DDRB
#define a1 PB0

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include "lcd.h"
//#include "lcd1.h"
#include "adc.h"
#include <stdlib.h>
#include <stdio.h>

#define DHT11_PIN 0 
uint8_t ch=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;


void set(char *buffer, float x) {
	int len = sprintf(buffer, "%d.", (int)x);
	x -= (int)x;
	x *= 10;
	len += sprintf(buffer+len, "%d", (int)x);
	x -= (int)x;
	x *= 10;
	sprintf(buffer+len, "%d", (int)x);
}

void Request()					/* Microcontroller send start pulse or request */
{
	DDRB |= (1<<DHT11_PIN);
	PORTB &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);				/* wait for 20ms */
	PORTB |= (1<<DHT11_PIN);	/* set to high pin */
}
void Response()					/* receive response from DHT11 */
{
	DDRB &= ~(1<<DHT11_PIN);
	while(PINB & (1<<DHT11_PIN));
	while((PINB & (1<<DHT11_PIN))==0);
	while(PINB & (1<<DHT11_PIN));
}
uint8_t Receive_data()			/* receive data */
{
	for (int q=0; q<8; q++)
	{
		while((PINB & (1<<DHT11_PIN)) == 0);	/* check received bit 0 or 1 */
		_delay_us(30);
		if(PINB & (1<<DHT11_PIN))	 /* if high pulse is greater than 30ms */
		ch = (ch<<1)|(0x01);		     /* then its logic HIGH */
		else				         /* otherwise its logic LOW */
		ch = (ch<<1);
		while(PINB & (1<<DHT11_PIN));
	}
	return ch;
}

int main(void)
{
	unsigned int result,d=1;
	float volt;
	char buffer[7],b[7],data[5];
	int adc_output,adc_output1,rain;
	DDRD = 0xFF;
	DDRC = 0xFF;
	int t1,h1;
	int tt,hh,dd,rr;
	//DDRB = 0xFF;
	//DDRA=0x0F;
	
	ADMUX = 0b01100000;
	ADCSRA = 0b10000111;
	
	Lcd4_Init();
	//adc_init();
	//Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
//	Lcd4_Write_String("Uncovered area:");
	_delay_ms(3000);
	//Lcd4_Clear();
	int r1=-100,c=1,rv;
	idd=0xF0;  /* Set the port as input port */
	iport=0xFF;  /* Active the internal pull-ups 
               //     of input port */
	char a11[18]="Warming Up...";
	char b11[18]="Object Detected";
	char c11[18]="Place is Clear";
	char ns[18]="SHAPE OF YOU";

	char nw[18]="ED SHEERAN";	

	char rs[18]="RAINDROPS";
	char rw[18]="BJ THOMAS";

	char nis[18]="PURNOTA";
	char niw[18]="WARFAZE";

	//LCD_String_xy(0,0,a);
//	_delay_ms(6000);
	while(1) {
		if(c<10) _delay_ms(1000);
		else{
		if(ipin==0xFE)
		{
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String(c11);
			_delay_ms(1500);
		}
		else
		{
			int temparature;
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String(b11);
			_delay_ms(8000);
			Lcd4_Clear();
			ADCSRA |= (1<<ADSC);
			adc_output=read_adc_channel(0);
			adc_output1=read_adc_channel(1)*108;
			while( ADCSRA & (1<<ADSC) )
			;
			temparature=adc_output/2;
			if(temparature>100) temparature=30;
			set(buffer,temparature);
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("TEMPARATURE:");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(buffer);
			Lcd4_Set_Cursor(2,4);
			Lcd4_Write_String("C");
			if(temparature>32) tt=1;
			else if(temparature>16) tt=0;
			else tt=-1;
			
			_delay_ms(8000);
			DDRB=0xFF;
			Lcd4_Clear();
			
					Request();		                       /* send start pulse */
					Response();			                   /* receive response */
					I_RH=Receive_data();	               /* store first eight bit in I_RH */
					D_RH=Receive_data();	               /* store next eight bit in D_RH */
					I_Temp=Receive_data();                 /* store next eight bit in I_Temp */
					D_Temp=Receive_data();	               /* store next eight bit in D_Temp */
					CheckSum=Receive_data();               /* store next eight bit in CheckSum */
					
					itoa(I_RH,data,10);
					Lcd4_Set_Cursor(1,0);
					Lcd4_Write_String("HUMIDITY");


					Lcd4_Set_Cursor(2,0);
					Lcd4_Write_String(data);
					
					Lcd4_Set_Cursor(2,4);
					Lcd4_Write_String("%");
					
					if(I_RH>65) hh=1;
					else if(I_RH>50) hh=0;
					else hh=-1;
					_delay_ms(8000);
					Lcd4_Clear();
					if(adc_output1>6000)
					{
											Lcd4_Set_Cursor(1,0);
											Lcd4_Write_String("WET");
											rr=1;

					}
					else
					{
											Lcd4_Set_Cursor(1,0);
											Lcd4_Write_String("DRY");
											rr=0;

					}
					_delay_ms(8000);
					
					if(d==1) 
					{
						Lcd4_Set_Cursor(1,0);
						Lcd4_Write_String("DAY");
					//	rr=0;

					}

			        else
										if(d==0)
										{
											Lcd4_Set_Cursor(1,0);
											Lcd4_Write_String("NIGHT");
											//	rr=0;

										}

								_delay_ms(8000);

											Lcd4_Set_Cursor(1,0);
											Lcd4_Write_String("WAITING");
			
								_delay_ms(8000);
								if(tt==0 && rr==0 && hh==0 && d==1)
								{
											Lcd4_Set_Cursor(1,0);
											Lcd4_Write_String("SHAPE OF YOU");
													Lcd4_Set_Cursor(2,0);
													Lcd4_Write_String("ED SHEERAN");
									
								}

								if(tt==0 && rr==0 && hh==0 && d==0)
								{
									Lcd4_Set_Cursor(1,0);
									Lcd4_Write_String("PURNOTA");
									Lcd4_Set_Cursor(2,0);
									Lcd4_Write_String("WARFAZE");
									
								}
								if(tt==0 && rr==1 && hh==0 && d==1)
								{
									Lcd4_Set_Cursor(1,0);
									Lcd4_Write_String("RAINDROPS");
									Lcd4_Set_Cursor(2,0);
									Lcd4_Write_String("BJ THOMAS");
									
								}
								if(tt==0 && rr==1 && hh==0 && d==0)
								{
									Lcd4_Set_Cursor(1,0);
									Lcd4_Write_String("RIMJHIM E DHARATE");
									Lcd4_Set_Cursor(2,0);
									Lcd4_Write_String("SHAAN");
									
								}
								if(tt==1 && rr==0 && hh==0 && d==1)
								{
									Lcd4_Set_Cursor(1,0);
									Lcd4_Write_String("SONALI PRANTORE");
									Lcd4_Set_Cursor(2,0);
									Lcd4_Write_String("NOCHIKETA");
									
								}
								if(tt==1 && rr==0 && hh==0 && d==0)
								{
									Lcd4_Set_Cursor(1,0);
									Lcd4_Write_String("EKHON ONEK RAT");
									Lcd4_Set_Cursor(2,0);
									Lcd4_Write_String("ANUPAM");
									
								}
								
								if(tt==-1 && rr==0 && hh==0)
								{
									Lcd4_Set_Cursor(1,0);
									Lcd4_Write_String("COLD");
									Lcd4_Set_Cursor(2,0);
									Lcd4_Write_String("MAROON 5");
									
								}
								else
								{
									Lcd4_Set_Cursor(1,0);
									Lcd4_Write_String("KENO EI NISSONGOTA");
									Lcd4_Set_Cursor(2,0);
									Lcd4_Write_String("SOULS");
									
								}
								
			if(ipin==0xFE)
			{
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String(a11);
				_delay_ms(2000);
			}
		}


/*		ADCSRA |= (1<<ADSC);
		//adc_output=read_adc_channel(0);
		adc_output1=read_adc_channel(0)*108;
		while( ADCSRA & (1<<ADSC) )
		;
		
		Request();		                       /* send start pulse */
//		Response();			                   /* receive response */
	//	I_RH=Receive_data();	               /* store first eight bit in I_RH */
		//D_RH=Receive_data();	               /* store next eight bit in D_RH */
//		I_Temp=Receive_data();                 /* store next eight bit in I_Temp */
//		D_Temp=Receive_data();	               /* store next eight bit in D_Temp */
//		CheckSum=Receive_data();               /* store next eight bit in CheckSum */
		
/*		if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
		{
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("ERROR");
		}
		else
		{
			itoa(I_RH,data,10);
			Lcd4_Set_Cursor(1,0);
			//Lcd4_Write_String(data);			
			//lcd_puts(".");
			
			/*itoa(D_RH,data,10);
			lcd_puts(data);
			lcd_puts("%");

			itoa(I_Temp,data,10);
			Lcd4_Set_Cursor(2,0);
		//	Lcd4_Write_String(data);
	/*		lcd_puts(".");
			
			itoa(D_Temp,data,10);
			lcd_puts(data);
			lcd_command(0xDF);
			lcd_puts("C ");
			
			itoa(CheckSum,data,10);
			//lcd_puts(data);
			//lcd_puts(" ");
			
		}
		
//		temperature=adc_output/2;
//		if(temperature>100) temperature=30;
//		float x=5.2;
//		result = ADCL;
//		result = (result>>6) | (ADCH<<2);
		//PORTB = result;
		
		//volt = (float)result*5.0/1024.0;
		//snprintf(buffer, sizeof(buffer), "%f", x);
		//set(buffer, temperature);
		set(b,adc_output1);
		if(c==3) rv=adc_output1;
		
	    Lcd4_Set_Cursor(1,0);
	    Lcd4_Write_String(b);
		if( c>3 && adc_output1-rv>500) r1=c;
		if(c-r1<20){
			
			Lcd4_Set_Cursor(2,1);
			Lcd4_Write_String("Wet");
		}
		
		else{ 
			Lcd4_Set_Cursor(2,1);
			Lcd4_Write_String("DRY");
			r1=-100;
			}

		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String(b);
//		Lcd4_Set_Cursor(2,1);
	//	Lcd4_Write_String("Ananya is boss");
		_delay_ms(1000);
		
/*		
		if(ipin==0xFE)
			 {
				Lcd4_Set_Cursor(1,0);
				Lcd4_Write_String(c11);
				 _delay_ms(1500);
			 }
			 else
			 {
				Lcd4_Set_Cursor(1,0);
				Lcd4_Write_String(b11);
				 _delay_ms(1500);
				 if(ipin==0xFE)
				 {
					Lcd4_Set_Cursor(2,0);
					Lcd4_Write_String(a11);
					 _delay_ms(2000);
				 }
			 }*/

		//t1=I_Temp;
		//h1=I_RH;
		c++;
		_delay_ms(10);
			 }

	}
	
}