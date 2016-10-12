#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <avr/pgmspace.h> 
 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 

//---------------------------
#include "bit.h"
#include "lcd.h"
#include "keypad.h"
//---------------------------

//For SPI Master
void SPI_MasterInit()
{
	//Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	//Set SPCR register to enable SPI, enable master, and use sck frequency
	// of fosc/16 (pg. 168)
	// Make sure global interrupts are enabled on SREG register (pg. 9 )

	/* Set MOSI and SCK output, all others input */
	DDRB = 0xB0;
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	sei();
}

void SPI_MasterTransmit ( unsigned char cData) {
	// data in SPDR will be transmitted, e.g. SPDR = cData ;
	SPDR = cData;
	// set SS low
	SetBit(PORTB,0,4);
	//Start transmission
	while (!( SPSR & (1<< SPIF ))) { // wait for transmission to complete
		;
	}
	// set SS high
	SetBit(PORTB,1,4);
}

//For SPI Servant
void SPI_ServantInit ( void ) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as inpu t
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168 )
	// make sure global interrupts are enabled on SREG register (pg. 9 )

	// Set MISO output, all others input
	DDRB = (1<<DDRB6);
	// Enable SPI
	SPCR = (1<<SPIE)|(1<<SPE);
	sei();
}

unsigned char receivedData;
ISR(SPI_STC_vect) { // this is enabled in with the SPCR register’s “SP I
	// Interrupt Enable”
	// SPDR contains the received data, e.g. unsigned char receivedData =
	// SPDR;

	// Wait for reception complete
	//while (!(SPSR & (1<<SPIF))) {}
	receivedData = SPDR;
}


int main(void) 
{ 
	//For Master
	
	DDRA = 0xF0; PORTA = 0x0F;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	// Initializes the LCD display
	LCD_init();
	// Starting at position 1 on the LCD screen, writes Hello World
	LCD_DisplayString(1, "Hello World");
	delay_ms(1000);
	LCD_ClearScreen();
	unsigned char press = 0x00;
	unsigned char string[5];
	string[0] = 'H';
	string[1] = 'i';
	string[2] = ':';
	string[3] = ' ';
	string[4] = 0x00;

	SPI_MasterInit();

	while(1){
		//SPI_MasterTransmit(0x0F);
		press = GetKeypadKey();
		if (press != string[3])
		{
			if (press == '1')
				SPI_MasterTransmit(0x01);
			else if (press == '2')
				SPI_MasterTransmit(0x02);
			else if (press == '3')
				SPI_MasterTransmit(0x04);
			else if (press == '4')
				SPI_MasterTransmit(0x08);
			else if (press == '5')
				SPI_MasterTransmit(0x10);
			else if (press == '6')
				SPI_MasterTransmit(0x20);
			else if (press == '7')
				SPI_MasterTransmit(0x40);
			else if (press == '8')
				SPI_MasterTransmit(0x80);
			else
				SPI_MasterTransmit(0x00);

			string[3] = press;
			LCD_DisplayString(1,string);
		}
		delay_ms(1000);
	}
	
	//For Servant
	/*
	DDRC = 0xFF; PORTC = 0x00;
	
	SPI_ServantInit();
	PORTC = 0xF0;
	delay_ms(1000);
	while(1){
		PORTC = receivedData;
	}
	*/
   return 0; 
}