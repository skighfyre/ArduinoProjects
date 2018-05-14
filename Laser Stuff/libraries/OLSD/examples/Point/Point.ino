// Open Laser Show DAC Example - Point
// Draws a point.  As simple as it gets.

#include <OLSD.h>
#include <standard.h>

OLSD dac;

void setup()
{
  dac.Init();
  dac.SetOutputRate(12000);
}

void loop()
{
  // Draw a point in the middle of the X and Y axis
  // For unsigned integers (16 bit) the middle
  // is 32767 I think... close enough.
  dac.OutputPoint(32767, 32767, 255, 255, 255);
}
