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
	DDRB = (1<<DDB6);
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
//global varaible
unsigned char pattern;
unsigned char speed;
//----------Check Tick----------
enum Check_State {Check_INIT, Change} check_state;

void Check_Init()
{
	check_state = Check_INIT;
}

void Check_Tick()
{
	static unsigned char data;
	static unsigned char temp;
	//Actions
	switch (check_state)
	{
		case Check_INIT:
			pattern = 0x00;
			speed = 0x00;
			data = 0x00;
			temp = 0x00;
			break;

		case Change:
			temp = receivedData;
			if (temp != data)
			{
				data = temp;
				pattern = data >> 4;
				speed = data & 0x0F;
			}
			break;

		default:

			break;
	}
	//Transitions
	switch (check_state)
	{
		case Check_INIT:
			check_state = Change;
			break;

		case Change:
			check_state = Change;
			break;

		default:
			check_state = Check_INIT;
			break;
	}
}

//----------Display Tick----------
enum Display_State {Display_INIT, Display, Speed} display_state;

void Display_Init()
{
	check_state = Display_INIT;
}

void Display_Tick()
{
	static unsigned char count;
	static unsigned char display_pattern;
	static unsigned char display_speed;
	static unsigned char display_variable;
	static unsigned char insideThree;
	//Actions
	switch (display_state)
	{
		case Display_INIT:
			count = 0x00;
			display_pattern = 0x00;
			display_speed = 0x00;
			display_variable = 0x00;
			insideThree = 0x00;
			break;
		case Display:
			//Change Pattern
			if (display_pattern != pattern)
			{
				display_pattern = pattern;
			}
			//Pattern
			if(display_pattern == 0x01){
				if(display_variable == 0x0F){
					display_variable = 0xF0;
				}
				else{
					display_variable = 0x0F;
				}
				insideThree = 0x00;
			}
			else if (display_pattern == 0x02){
				if(display_variable == 0xAA){
					display_variable = 0x55;
				}
				else{
					display_variable = 0xAA;
				}
				insideThree = 0x00;
			}
			else if(display_pattern == 0x03)
			{
				if (insideThree == 0x00)
				{
					display_variable = 0x80;
				}
				else if(display_variable == 0x01)
				{
					display_variable = 0x80;
				}
				else
				{
					display_variable = display_variable >> 1;
				}
				insideThree = 0x01;
			}
			else if(display_pattern == 0x04)
			{
				if(display_variable == 0xFF)
				{
					display_variable = 0x0F;
				}
				else
				{
					display_variable = 0xFF;
				}
				insideThree = 0x00;
			}
			//Change Speed
			if (display_speed != speed)
			{
				display_speed = speed;
			}
			//Speed
			if(display_speed == 0x01)		//2000ms
			{
				count = 0x4F;
			}
			else if(display_speed == 0x02)	//1000ms
			{
				count = 0x27;
			}
			else if (display_speed == 0x03)	//500ms
			{
				count = 0x19;
			}
			else if (display_speed == 0x04)	//250ms
			{
				count = 0x09;
			}
			else if (display_speed == 0x05)	//100ms
			{
				count = 0x03;
			}
			else if (display_speed == 0x06)	//50ms
			{
				count = 0x01;
			}
			PORTC = display_variable;
			break;
		case Speed:
			count--;
			break;
		default:

			break;
	}

	//Transitions
	switch (display_state)
	{
		case Display_INIT:
			display_state = Display;
			break;
		case Display:
			display_state = Speed;
			break;
		case Speed:
			if(count <= 0x00 || display_speed != speed || display_pattern != pattern)
			{
				display_state = Display;
			}
			break;
		default:
			display_state = Display_INIT;
			break;
	}
}

//----------Master System Task----------
void CheckTask()
{
	SPI_ServantInit();
	Check_Init();
	Display_Init();
	for (;;)
	{
		Display_Tick();
		Check_Tick();
		vTaskDelay(25);
	}
}

void StartSlaveSystemPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(CheckTask, (signed portCHAR *)"MasterSystemTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

int main(void)
{
	//For Servant
	DDRC = 0xFF; PORTC = 0x00;
	
	//Start Tasks
	StartSlaveSystemPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	return 0;
}