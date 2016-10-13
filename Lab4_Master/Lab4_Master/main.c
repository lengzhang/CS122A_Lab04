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
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	// Set SPCR register to enable SPI, enable master, and use sck frequency
	// of fosc/16 (pg. 168)
	// Make sure global interrupts are enabled on SREG register (pg. 9 )

	/* Set SS, MOSI and SCK output, all others input */
	DDRB = (1<<DDRB4) | (1<<DDRB5) | (1<<DDRB7);
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

unsigned char Ptrn;
unsigned char Spd;
unsigned char uC;
unsigned char data;

//----------Input Tick----------
enum Input_State {INPUT_INIT, GET_INPUT} input_state;

void Input_Init()
{
	DDRA = 0xF0; PORTA = 0x0F;				//Enable the keypad
	input_state = INPUT_INIT;
}

void Input_Tick()
{
	static unsigned char press;
	static unsigned char pressed;
	//Actions
	switch (input_state)
	{
		case INPUT_INIT:
			Ptrn = 0x01;
			Spd = 0x01;
			uC = 0x01;
			data = 0x11;
			press = 0x00;
			pressed = 0x00;
			break;

		case GET_INPUT:
			press = GetKeypadKey();
			if (pressed != press)
			{
				pressed = press;

				if (press == 'A')			//A
					Ptrn = 0x01;
				else if (press == 'B')		//B
					Ptrn = 0x02;
				else if (press == 'C')		//C
					Ptrn = 0x03;
				else if (press == 'D')		//D
					Ptrn = 0x04;
				else if (press == '1')		//1
					Spd = 0x01;
				else if (press == '2')		//2
					Spd = 0x02;
				else if (press == '3')		//3
					Spd = 0x03;
				else if (press == '4')		//4
					Spd = 0x04;
				else if (press == '5')		//5
					Spd = 0x05;
				else if (press == '6')		//6
					Spd = 0x06;
				else if (press == '7')		//7
					uC = 0x01;
				else if (press == '8')		//8
					uC = 0x02;
				else if (press == '9')		//9
					uC = 0x03;

				data = (Ptrn << 4) | Spd;	//Update data
			}
			break;

		default:

			break;
	}
	//Transitions
	switch (input_state)
	{
		case INPUT_INIT:
			input_state = GET_INPUT;
			break;
		case GET_INPUT:

			break;
		default:
			input_state = INPUT_INIT;
			break;
	}
}

//----------Display Tick----------
enum Display_State {DISPLAY_INIT, DISPLAY} display_state;

void Display_Init()
{
	DDRC = 0xFF; PORTC = 0x00;				// LCD data lines
	DDRD = 0xFF; PORTD = 0x00; 				// LCD control lines
	// Initializes the LCD display
	LCD_init();

	display_state = DISPLAY_INIT;
}

void Display_Tick()
{
	static unsigned char* string;
	static unsigned char display_Ptrn;
	static unsigned char display_Spd;
	static unsigned char display_uC;
	//Actions
	switch (display_state)
	{
		case DISPLAY_INIT:
			//Clean the screen
			LCD_ClearScreen();

			string = " Ptrn: _ Spd: _  uC: _          ";
			display_Ptrn = 0x01;
			display_Spd = 0x01;
			display_uC = 0x01;
			string[7] = display_Ptrn + '0';
			string[14] = display_Spd + '0';
			string[21] = display_uC + '0';

			LCD_DisplayString(1, string);
			break;

		case DISPLAY:
			if (display_Ptrn != Ptrn || display_Spd != Spd || display_uC != uC)
			{
				display_Ptrn = Ptrn;
				display_Spd = Spd;
				display_uC = uC;
				string[7] = display_Ptrn + '0';
				string[14] = display_Spd + '0';
				string[21] = display_uC + '0';

				LCD_DisplayString(1, string);
			}
			break;

		default:

			break;
	}

	//Transitions
	switch (display_state)
	{
		case DISPLAY_INIT:
			display_state = DISPLAY;
			break;

		case DISPLAY:

			break;

		default:
			display_state = DISPLAY_INIT;
			break;
	}
}

//----------SPI Master Tick----------
enum MasterState {MASTER_INIT, MASTER_TRANSMIT} master_state;

void Master_Init()
{
	SPI_MasterInit();						//Init SPI
	master_state = MASTER_INIT;
}

void Master_Tick()
{
	static unsigned char sent_data;
	//Actions
	switch (master_state)
	{
		case MASTER_INIT:
			sent_data = 0x00;
			break;

		case MASTER_TRANSMIT:
			if (sent_data != data)
			{
				sent_data = data;
				SPI_MasterTransmit(sent_data);
			}
			break;

		default:

			break;
	}

	//Transitions
	switch (master_state)
	{
		case MASTER_INIT:
			master_state = MASTER_TRANSMIT;
			break;

		case MASTER_TRANSMIT:

			break;

		default:
			master_state = MASTER_INIT;
			break;
	}
}

//----------Master System Task----------
void MasterSystemTask()
{
	Input_Init();
	Display_Init();
	Master_Init();
	for (;;)
	{
		Input_Tick();
		Display_Tick();
		Master_Tick();
		vTaskDelay(100);
	}
}

void StartMasterSystemPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(MasterSystemTask, (signed portCHAR *)"MasterSystemTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

//Master System Main
int main(void) 
{ 
	//Start Tasks  
	StartMasterSystemPulse(1);
	//RunSchedular 
	vTaskStartScheduler();

	return 0; 
}