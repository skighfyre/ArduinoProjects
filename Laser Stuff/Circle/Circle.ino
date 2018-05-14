// Open Laser Show DAC Example - Circle
// Draws a nice circle and shifts the phase so it rotates.

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

// Phase is global so it is persistant through the loop function callss
float phase = 0.0;

void loop()
{
  float x,y,a;

  // Calculate and draw a circle using sine and cosine
  // This is a little slow given the floating point math on this processor    
  for (a = 0.0; a <= (2.0 * PI); a += 0.1)
  {
    // Calculate x and y using our angle in radians
    // Include a phase shift so the circle rotates
    x = sin(a + phase);
    y = cos(a);
    // Change the +/-1.0 numbers into unsigned integers
    x = x * 32760.0 + 32760.0;
    y = y * 32760.0 + 32760.0;
    // Output the point
    dac.OutputPoint((unsigned int)x, (unsigned int)y, 255, 255, 255);
  }

  // Change the phase for next time we draw the circle
  phase += (PI / 360.0);
  if (phase > (2.0 * PI)) phase = 0.0;
}

