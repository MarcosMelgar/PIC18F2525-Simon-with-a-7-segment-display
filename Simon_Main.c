
//---------------------------------------------------------------------
// Name: Simon_Main.c
// Date created:  03.07.2017
//--------------------------
//
// Author: Marcos Melgar
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include <p18f2525.h>		// Register definitions
#include <timers.h>
#include <delays.h>
#include <stdlib.h>
//---------------------------------------------------------------------
// Configuration bits:

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

//---------------------------------------------------------------------
// Constant declarations and definitions

#define HIGH 1
#define LOW 0
#define R1 PORTBbits.RB5
#define R2 PORTBbits.RB4
#define R3 PORTBbits.RB3
#define R4 PORTBbits.RB2
#define C1 PORTCbits.RC0
#define C2 PORTCbits.RC1
#define C3 PORTCbits.RC2
#define C4 PORTCbits.RC3

#define PORT1 PORTAbits.RA0 //g
#define PORT2 PORTAbits.RA1 //f
#define PORT3 PORTAbits.RA2 //e
#define PORT4 PORTAbits.RA3 //d
#define PORT5 PORTCbits.RC7 //c
#define PORT6 PORTCbits.RC6 //b
#define PORT7 PORTCbits.RC5 //a

#define ZERO  0b01111110
#define ONE   0b00110000
#define TWO   0b01101101
#define THREE 0b01111001
#define FOUR  0b00110011
#define FIVE  0b01011011
#define SIX   0b01011111
#define SEVEN 0b01110000
#define EIGTH 0b01111111
#define NINE  0b01111011
#define AA    0b01110111
#define BB    0b00011111
#define CC    0b01001110
#define DD    0b00111101
#define EE    0b01001111
#define FF    0b01000111
#define HH    0b00010111
#define LL    0b00001110
#define LENGTHGAME 4 


//---------------------------------------------------------------------
// Global variable declarations
//---------------------------------------------------------------------

// Function declarations

/*%hhu*/
void Setup(void);
unsigned char KeyRead(void);
void shifter(unsigned char);

//---------------------------------------------------------------------
// ISR
//#pragma interrupt high_isr

#pragma code isrcode=0x0008 // interrupt vector


//---------------------------------------------------------------------
// Begin code section
#pragma code

//---------------------------------------------------------------------
// main()
//---------------------------------------------------------------------

void main(void){
	unsigned char counter = 1;
	unsigned char i;

	unsigned char key;
	unsigned char temkey;
	unsigned char collectionofkeys[16] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGTH, NINE, AA, BB, CC, DD, EE, FF};
	unsigned char buffer[LENGTHGAME];
	unsigned char pressedkeybuffer[LENGTHGAME];
	unsigned char randomselector;
	unsigned char failflag = 0;

	Setup();	    // Initialize variables and peripherals

	//WELCOME MESSAGE UNTIL 0key PRESSED

	do 
	{	//H
		shifter(HH);
		Delay100TCYx(2);
		//E
		shifter(EE);
		Delay100TCYx(2);
		//L
		shifter(LL);
		Delay100TCYx(2);
		//L
		shifter(LL);
		Delay100TCYx(2);
		//O
		shifter(ZERO);
		Delay100TCYx(2);

	} while(KeyRead() != ZERO);

	while(1){
		
		srand(ReadTimer0());
		randomselector = rand() % 16;
		buffer[counter-1] = collectionofkeys[randomselector];

		/*display key*/
		for (i = 0; i < counter; ++i)
		{
			shifter(buffer[i]);
			Delay100TCYx(2);
		}
		
		//check timer, if timer> 10s -> failflag = 1

		do
		{
			temkey = KeyRead();
			Delay10TCYx(2);
			key = KeyRead();
		} while ((temkey == key) && (KeyRead() != 0x00));

			pressedkeybuffer[counter-1] = key;

		//comparator
		for (i = 0; i < counter; ++i)
		{
			if(buffer[i] != pressedkeybuffer[i])
				failflag = 1;
		}

		if(failflag){
			//FAIL MESSAGE UNTIL 0key PRESSED
			
			do {
			//F
			shifter(FF);
	        Delay100TCYx(2);
			//A
			shifter(AA);
	        Delay100TCYx(2);
			//I
			shifter(ONE);
	        Delay100TCYx(2);
			//L
			shifter(LL);
	        Delay100TCYx(2);
				
	        } while(KeyRead() != ZERO);
			counter = 0;
			
			/*clearing buffers*/
			for (i = 0; i < LENGTHGAME; ++i)
			{
				buffer[i] = 0;
				pressedkeybuffer[i] = 0;
			}

		}

		counter++;
		if(counter == LENGTHGAME){
			// YEAH MESSAGE
			do {
			//Y
			shifter(FOUR);
			Delay100TCYx(2);
			
			//E
			shifter(EE);
			Delay100TCYx(2);
			
			//A
			shifter(AA);
			Delay100TCYx(2);
			
			//H
			shifter(HH);
			Delay100TCYx(2);

			} while(KeyRead() != ZERO);

			counter = 0;
			
			/*clearing buffers*/
			for (i = 0; i < LENGTHGAME; ++i)
			{
				buffer[i] = 0;
				pressedkeybuffer[i] = 0;
			}

			
		}


		
    }
}

//---------------------------------------------------------------------
// Declared functions

//---------------------------------------------------------------------
//Setup();   Initialize variables and peripherals
//---------------------------------------------------------------------
void Setup(void){

	//OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_EXT & T0_EDGE_RISE & T0_PS_1_256);
	//timer0 remaining time
	T0CON = 0b00100111;

	TMR0H = 0X0B;
	TMR0L = 0XDC;

	ADCON1=0xff; //Digital inputs

	PORTB=0x00;
	TRISB=0xff; //inputs

	PORTC=0x00;
	TRISC=0x00; //outputs

	PORTA=0x00;
	TRISA=0x00; //outputs
    INTCON2bits.RBPU = 0; //enable pull-ups

}
//---------------------------------------------------------------------
//unsigned char KeyRead();   Reads the key from the 4x4 matrix keyboard
//---------------------------------------------------------------------

unsigned char KeyRead(void){
	C1 = LOW;
	C2 = HIGH;
	C3 = HIGH;
	C4 = HIGH;

	if(R1 == LOW)
			return ZERO; 
	if(R2 == LOW)
			return FOUR; 
	if(R3 == LOW)
			return EIGTH; 
	if(R4 == LOW)
		  return CC; 

	C1 = HIGH;
	C2 = LOW;
	C3 = HIGH;
	C4 = HIGH;

	if(R1 == LOW)
		return ONE; 
	if(R2 == LOW)
		return FIVE; 
	if(R3 == LOW)
		return NINE; 
	if(R4 == LOW)
		return DD; 

	C1 = HIGH;
	C2 = HIGH;
	C3 = LOW;
	C4 = HIGH;

	if(R1 == LOW)
		return TWO; 
	if(R2 == LOW)
		return SIX; 
	if(R3 == LOW)
		return AA; 
	if(R4 == LOW)
		return EE; 

	C1 = HIGH;
	C2 = HIGH;
	C3 = HIGH;
	C4 = LOW;

	if(R1 == LOW)
		return THREE; 
	if(R2 == LOW)
		return SEVEN; 
	if(R3 == LOW)
		return BB; 
	if(R4 == LOW)
		return FF; 
 return 0x00;
}

//---------------------------------------------------------------------
//shifter(unsigned char k);   Splits a 7-bit literal and writes it in the 7-segment display
//---------------------------------------------------------------------

void shifter(unsigned char k){
	PORT1 = k & 0x1;
	PORT2 = (k & 0x2) >> 1;
	PORT3 = (k & 0x4) >> 2;
	PORT4 = (k & 0x8) >> 3;
	PORT5 = (k & 0x10) >> 4;
	PORT6 = (k & 0x20) >> 5;
	PORT7 = (k & 0x40) >> 6;
}
