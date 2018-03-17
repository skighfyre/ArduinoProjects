#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
const int buttonPin = 2;
int buttonState = 0;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_RGB + NEO_KHZ800);

/*
 * Purpose: A quick example of how to use an array for a neopixel strip, this can also be applied general cases of multiplexing manipulations
 * Author: Stephen Copeland
 */


// This is a MACRO statement we will use to vary the size of the array at compile time
// REMEMBER - this is not a variable, it cannot be changed by the code. If you would like to use a more dynamic way
// of managing the PixelArray, you might need to look into re-writing this code with something like a linked list.
#define NEO_PIXEL_SIZE 24

// Now create the array with the same number of pixels as what is on the neopixel strip.
uint32_t PixelArray[NEO_PIXEL_SIZE];

// When arrays are first declared, they are creating pointers to memory that has not been cleaned. 
// This means there can be random unexpected values in there which can cause runtime issues
// So we need to call this function to clear up any garbage before using anything else
// So always call this function first at the beginning of setup
void initPixArr(){
  for(int i = 0; i < NEO_PIXEL_SIZE; i++){
    PixelArray[i] = 0;
  }
}

// This function allows you to set individual increments of the array to a certain color.
// So for example, say you have 24 neopixels.
// So your array is 24 uint32_t long.
// Now say you want pixels 3,4 and 5 to be some color.
// you would call this function to PREPARE the PixelArray:
// updateArray(2,4,color);
// Now REMEMBER, this hasn't made the change yet, we've just prepared the array.
void updateArray(uint8_t PixStart, uint8_t PixStop, uint8_t R, uint8_t G, uint8_t B){
  for(uint8_t i = PixStart; i < PixStop; i++){
    PixelArray[i] = strip.Color(R, G, B);
  }
}

void function1(){
 for(int i = 0; i<12; i++){

     updateArray(0, 6, 0,255,0);
     updateArray(6, 12, 0,0,255);
     updateArray(12, 18, 0,255,0);
     updateArray(18, 24, 0,0,255);
     updatePixels();
     delay(100);
     updateArray(0, 6, 0,0,255);
     updateArray(6, 12, 0,255,0);
     updateArray(12, 18, 0,0,255);
     updateArray(18, 24, 0,255,0);
     updatePixels();
    strip.show();

}
}
// This function will take our prepared array, and send it to the neopixel strip to update all the color values of each pixel (even if they don't change)
// then it will show it to the user.
// Just to be clear, the standard method (really the only way) these functions should be called is in order.
// As in, make a change -> update the pixels, if you make a change without updating the pixels, The user will see no effect.
// If you update the pixels without making a change, again, nothing happens.
void updatePixels(){
  for(int i = 0; i < NEO_PIXEL_SIZE; i++){
    strip.setPixelColor(i, PixelArray[i]);
  }
  strip.show();
}

void setup(){
  pinMode(buttonPin, INPUT);  
  initPixArr();
  // example
//  updateArray(5, 10, 255, 255, 255); // so again, this is saying on the array, pixels 6-11 become color (255,255,255), but don't update yet);
//  updatePixels(); // this actually communicates to the neopixels to follow the instructions we've setup on the PixelArray.
  strip.begin();
  strip.show();
}

void loop() {
    buttonState = digitalRead(buttonPin);
  // here is an example showing how one might use the updateArray function to start creating pixel patterns
if (buttonState == HIGH) {
    // turn LED on:
    updateArray(0, 24, 255,255,255);
    updatePixels();
    strip.show();
  }
else{
  
  function1;


 
/* for (int j =0; j<2000; j++){
  for (int i = 0; i < 24; i++) {
  updateArray(0,6,(0,255,0));
  updateArray(13,20,0,255,0);
  updatePixels();
  delay(1000);
    }
  updateArray(0,6,0,255,0);
  updateArray(13,20,0,255,0);
  updatePixels();
  delay(1000);
  updateArray(6,13,255,255,0);
  updatePixels();
  delay(1000);
 }
 
 // updateArray(0,6,0,255,0);
 // updatePixels();
  strip.show();
*/
}
}




