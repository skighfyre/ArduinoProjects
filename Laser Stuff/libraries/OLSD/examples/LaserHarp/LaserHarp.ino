// Open Laser Show DAC Example - Circle
// Draws Laser Harp Strings and sets the Harp String
// number on the IO ports if you have a HarpBoard.

#include <OLSD.h>
#include <standard.h>

OLSD dac;

void setup()
{
  dac.Init();
  // Set the point output rate really slow since floating point is really slow
  // No need to go faster
  dac.SetOutputRate(2000);
}

// String Positions For Laser Harp
unsigned int strings[13] = {0, 5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000, 55000, 60000};

void loop()
{
  int iString = 0;
  for (iString = 0; iString < 13; iString++)
  {
    // Move to the next position
    dac.OutputPoint(strings[iString], 32768, 0, 0, 0);
    delay(1);
    // Sit at this position displaying the laser
    dac.SetPositionPort(1, iString);
    dac.OutputPoint(strings[iString], 32768, 255, 255, 255);
    delay(1);
    // Turn the laser off and sit
    dac.SetPositionPort(0, iString);
    dac.OutputPoint(strings[iString], 32768, 0, 0, 0);
    delay(1);
  }
}

