// Open Laser Show DAC Example - Line
// Draws a horizontal line.

#include <OLSD.h>
#include <standard.h>

OLSD dac;

void setup()
{
  dac.Init();
  // Set the output rate to 12kpps
  dac.SetOutputRate(12000);
}

void loop()
{
  unsigned int x,y;
  
  // Draw the line
  for (x = 0; x < 65000; x = x + 1000)
  {
    dac.OutputPoint(x, 32767, 255, 255, 255);
  }
  
  // Draw the line in reverse so we don't make any big jumps
  // and annoy or break the scanners.
  for (x = 0; x < 65000; x = x + 1000)
  {
    dac.OutputPoint(65000 - x, 32767, 255, 255, 255);
  }
}
