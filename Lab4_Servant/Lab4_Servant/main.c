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
	//For Servant
	
	DDRC = 0xFF; PORTC = 0x00;
	
	SPI_ServantInit();
	PORTC = 0xF0;

	while(1){
		PORTC = receivedData;
	}
	
	return 0; 
}