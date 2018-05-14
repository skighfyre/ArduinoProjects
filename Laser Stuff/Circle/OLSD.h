// OLSD.H
// Open Laser Show DAC (OLSD)
// Library for outputting points to the OLSD board

// Open Laser Show DAC - Arduino Shield
// Copyright (c) 2011 Chris Favreau
// cfavreau@fab-favreau.com

#ifndef __OLSD_H__
#define __OLSD_H__

#include <avr/interrupt.h>		// For Interrupt Handling
#include "standard.h"			// Standard types and definitions

// One of these must be defined!
//#define V4 1
#define V3 1
//#define VHARP 1

class OLSD
{
public:
  
  OLSD();
  ~OLSD();

  void Init(void);
  
  void SetOutputRate(unsigned int pps);
  
  void OutputPoint(unsigned int x, unsigned int y, unsigned char red, unsigned char green, unsigned char blue);

  void SetPositionPort(_BOOL SyncBitOn, _BYTE nString);

private:

  // LED Status Counter
  unsigned int status_counter;

  // Flag to use output rate timer
  _BOOL bUseRateTimer;

  // Initialization functions
  void InitTimers(void);
  void InitPorts(void);
  void InitSPI(void);  
};

#endif // __OLSD_H__

