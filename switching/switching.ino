#include <Button.h>

float pressLength_milliSeconds = 0;
unsigned long previousMillis = 0;

// Define the *minimum* length of time, in milli-seconds, that the button must be pressed for a particular option to occur
int optionOne_milliSeconds = 100;
int optionTwo_milliSeconds = 500;        

//The Pin your button is attached to
int buttonPin = 2;
int state = HIGH;


//Pin your LEDs are attached to
int ledPin1 = 3;




void setup(){

  // Initialize the pushbutton pin as an input pullup
  // Keep in mind, when pin 2 has ground voltage applied, we know the button is being pressed
  pinMode(buttonPin, PULLDOWN);     

  //set the LEDs pins as outputs
  pinMode(ledPin1, OUTPUT); 
 // pinMode(ledPin_Option_2, OUTPUT); 


} // close setup


void loop() {

  //Record *roughly* the tenths of seconds the button in being held down
  while (digitalRead(buttonPin) == HIGH ){ 
    digitalWrite(ledPin1, HIGH);
    delay(100);  //if you want more resolution, lower this number 
    pressLength_milliSeconds = pressLength_milliSeconds + 100; 

  }//close while


  //Different if-else conditions are triggered based on the length of the button press
  //Start with the longest time option first

  //Option 2 - Execute the second option if the button is held for the correct amount of time
  if (pressLength_milliSeconds >= optionTwo_milliSeconds )
  {
      digitalWrite(ledPin1, HIGH);
      delay(10);
      
      
        digitalWrite(ledPin1, LOW);
      
    
   }    

  //option 1 - Execute the first option if the button is held for the correct amount of time
  else if(pressLength_milliSeconds >= optionOne_milliSeconds){

     if (state == HIGH)
        state = LOW;
        else
        state = HIGH;
    digitalWrite(ledPin1, state);  

  }//close if options


  //every time through the loop, we need to reset the pressLength_Seconds counter
  pressLength_milliSeconds = 0;

} // close void loop
