// OLSD.CPP
// Open Laser Show DAC (OLSD)
// Library for outputting points to the OLSD board

// Open Laser Show DAC - Arduino Shield
// Copyright (c) 2011 Chris Favreau
// cfavreau@fab-favreau.com

#include "olsd.h"
#include "standard.h"

// These have moved to the OLSD.H File!
//#define V4 1
//#define V3 1
//#define VHARP 1

// SPI Data Register Definitions
#define DDR_SPI   DDRB
#define DD_MOSI   DDB3
#define DD_MISO   DDB4
#define DD_SCK    DDB5

// Output Rate Timer Related Constants
#define MIN_PPS_RATE	1000
#define MAX_PPS_RATE	30000
#define DEFAULT_PPS	12000

#define TIMER_FUDGE_FACTOR	128
#define DEFAULT_TIMEOUT  TIMER_FUDGE_FACTOR + 65002 //64202	// 12 kpps

volatile unsigned char output_flag = 0;
volatile unsigned int timer_start_value = 0;

#ifdef V4
#define LEDINIT ddrd.bit2 = 1; portd.bit2 = 1;
#define LEDBIT ddrd.bit2
#endif

#ifdef V3
#define LEDINIT ddrc.bit4 = 1; portc.bit4 = 1;
#define LEDBIT ddrc.bit4
#endif

#ifdef VHARP
#define LEDINIT ddrc.bit4 = 1; portc.bit4 = 1;
#define LEDBIT ddrc.bit4
#endif

ISR(TIMER1_OVF_vect)
{  
	// Reset the counter
	TCNT1 = timer_start_value;

	// Check to see if the output flag has been reset
// TODO - put this back in!	
/*
        if (output_flag > 5)
	{
		// BLANK!!!! 
                // TODO
		// Red = 0
		// Green = 0
		// Blue = 0
		PORTC = (PORTC & 0xF8);
		// Shutter = 1
		// LED - BIT4 - leave ALONE! 
		// Switch - BIT5 - leave ALONE!
		portc.bit3 = 1;
	}
*/
	
	// Output a point NOW!
	output_flag++;  
}

OLSD::OLSD()
{
  status_counter = 0;
  bUseRateTimer = TRUE;
  output_flag = 0;
  timer_start_value = 0;

  Init();
}

OLSD::~OLSD()
{
}

void OLSD::InitPorts(void)
{
  // Laser Harp Board uses PortC in the same manner
  // Initialize our IO Ports
  // Port C = Button + LED + Shutter + Blue
  DDRC = 0b00011111;
  PORTC = 0b00111000;

#ifdef V4
  // Color Latch Bit
  ddrb.bit0 = 1;
  portd.bit0 = 1;
#endif

#ifdef V3
   // V3 PortD
  ///* 	
  // Port D - Serial + Red and Green
  DDRD = 0b11111100;
  PORTD = 0b00000011;
#endif

#ifdef VHARP
  // Laser Harp Port D
  // Port D - Serial + Harp Enable + Harp Sync + Harp Pos.
  // Serial Port Pins
  ddrd.bit0 = 0;
  ddrd.bit1 = 0;
  // Enable the pullups on these pins
  portd.bit0 = 1;
  portd.bit1 = 1;
  // Harp Enable (Input)
  ddrd.bit2 = 0;
  // Enable the pull up
  portd.bit2 = 1;
  // Harp Sync/Clk (Output)
  ddrd.bit3 = 1;
  // Harp Pos. Bits (Outputs)
  ddrd.bit4 = 1;
  ddrd.bit5 = 1;
  ddrd.bit6 = 1;
  ddrd.bit7 = 1;
#endif

  // Initialize our SPI port to output to our DAC
  ddrb.bit2 = 1;
  portb.bit2 = 1;
  InitSPI();

  // LDAC - disable
  ddrb.bit1 = 1;
  portb.bit1 = 1;

  // CTS Pin
  //ddrb.bit0 = 1;
  //portb.bit0 = 0;

  LEDINIT;
  
  // Turn the LED ON
  LEDBIT = 1;
}

void OLSD::InitSPI(void)
{
   DDR_SPI |= (1 << DD_MOSI) | (1 << DD_SCK);
   // Enable SPI Master and Set Clock rate to CLK / 2 (I.E. 16MHz)
   // and MSB first and Clock Polarity is Rising on the Leading Edge (Mode 0).
   SPCR |= (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0);
   SPSR |= (1 << SPI2X); //We need the fast 2X mode.
}

void OLSD::InitTimers(void)
{
  // Intialize the Timer1

  // Stop the timer
  TCCR1A = 0x00;
  TCCR1B = 0x00;

  // Set the initial timeout value to the default output rate
  SetOutputRate(DEFAULT_PPS);

  // Configure the Timer1 Overflow Interrupt to be ON
  TIMSK1 = 0b00000001;

  // Select the IO clock with NO prescalar
  TCCR1A = 0x00;
  TCCR1B = 0x01;
}

void OLSD::Init(void)
{
  status_counter = 0; 
  output_flag = 0;
  timer_start_value = 0;

  // Turn Off Interrupts
  cli();
  // Initlialize our IO ports
  InitPorts();
  // Initialize our Hardware Timers
  InitTimers(); 
  // Turn ON Interrupts
  sei();
}

void OLSD::SetOutputRate(unsigned int pps)
{
  // Bounds check the points per second
  if (pps > MAX_PPS_RATE) pps = MAX_PPS_RATE;
  if (pps < MIN_PPS_RATE) pps = MIN_PPS_RATE;
  
  // Convert points per second to thousands of points per second
  pps /= 1000;

  // Calculate the timer timeout value from the
  // number of points per second we are supposed
  // to display
  // Timer ceiling - (Clock Rate in kHZ / PPS) + Fudge Factor (found with scope)
  timer_start_value = 0xFFFF - (16000 / pps) + TIMER_FUDGE_FACTOR;
  
  // Set the timer count register
  TCNT1 = timer_start_value;
}
  
void OLSD::OutputPoint(unsigned int x, unsigned int y, unsigned char red, unsigned char green, unsigned char blue)
{
  unsigned int spi_word;

  // Scale the X and Y to 12 bit numbers
  x = x >> 4;
  y = y >> 4;
  
  // Bounds check X and Y
  if (x > 4095) x = 4095;
  if (y > 4095) y = 4095;
  
  // Wait for the output flag
  while (!output_flag) asm("nop");
  // Reset the output flag
  output_flag = 0;
  
  // Pull the LDAC line high
  portb.bit1 = 1;
  
  // Load DAC A
  // X X X PowerMode Load Output AddrB AddrA 
  spi_word = 0b0111000000000000 | (y & 0x0FFF);
  // Pull down chip select so the DAC knows we are taling to it
  portb.bit2 = 0;
  // Upper Byte of the command
  SPDR = spi_word >> 8;
  while (!(SPSR & (1 << SPIF)));
  // Lower Byte of the command
  SPDR = spi_word & 0x00FF;
  while (!(SPSR & (1 << SPIF)));
  // Pull up chip select
  portb.bit2 = 1;
  
  // Load DAC B and update the output (assign a 0 to output)
  spi_word = 0b1111000000000000 | (x & 0x0FFF);
  // Pull down chip select so the DAC knows we are taling to it
  portb.bit2 = 0;
  // Upper Byte of the command
  SPDR = spi_word >> 8;
  while (!(SPSR & (1 << SPIF)));
  // Lower Byte of the command
  SPDR = spi_word & 0x00FF;
  while (!(SPSR & (1 << SPIF)));
  // Pull up chip select
  portb.bit2 = 1;

#ifdef V4

  // Color Latches
  portb.bit0 = 0;
  
  // Fifth bits are the UPPER Nibble
  _BYTE fifthbits = 0;
  if (red) fifthbits |= 0x10;
  if (green) fifthbits |= 0x20;
  if (blue) fifthbits |= 0x40;

  // Colors
  // *** REMEMBER that a shift register passes the 1st 8 bits
  // on to the 2nd shift register so we do the BLUE first!
  // B goes first
  SPDR = fifthbits | blue;
  while (!(SPSR & (1 << SPIF)));

  // RG go second
  SPDR = (green << 4) | red;
  while (!(SPSR & (1 << SPIF)));
  
  // Now latch the Colors
  portb.bit0 = 1;

#endif

#ifdef V3
  // V3 Colors
  // Reduce all colors to 3 bit resolution
  red = red >> 5;
  green = green >> 5;
  blue = blue >> 5;
  // Red = 0
  // Green = 0
  PORTD = 0b00000011 | (red << 5) | (green << 2);

  // Blue = 0
  // Shutter = 0
  PORTC = 0b00110000 | blue;
#endif

#ifdef VHARP
  // Laser Harp TTL RGB -------------------------- 
  // Write the R G B bits
  _BYTE rgb = PORTC & 0b00111000;
  if (red > 0) rgb |= 0x01;
  if (green > 0) rgb |= 0x02;
  if (blue > 0) rgb |= 0x04;
  PORTC = rgb;
#endif
  
  // Open the shutter
  // TODO
  
  // Pull the LDAC line LOW to load the DAC's values
  portb.bit1 = 0;

  // Do the Status LED
  status_counter++;
  if (status_counter < 500)
  {
    // Reset the LED to flip
    LEDBIT = 1;
  }
  else
  {
    LEDBIT = 0;
  } 
  if (status_counter > 1000) status_counter = 0;
}

// Additional function to operate the Position Port on the Laser Harp Board
void OLSD::SetPositionPort(_BOOL SyncBitOn, _BYTE nString)
{
#ifdef VHARP  
  // Set string bits
  // nString should only be from 0 to 15
  PORTD = (PORTD & 0x0F) | (nString << 4);
  
  // Toggle sync line ON
  portd.bit3 = (SyncBitOn ? 1 : 0);  
#endif
}

