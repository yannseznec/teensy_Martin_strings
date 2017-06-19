/* some code designed for using the Teensy 3 as a USB MIDI controller
v1.0, December 17 2013
by Yann Seznec www.yannseznec.com

no copyright or anything, use however you want

remember to select MIDI as your USB Type in the Tools menu

this should also work with other Teensy boards, apart from the "touch" pins

things that are kind of dumb in this code:
- the touch threshold is hard coded (int touchThreshold)
- touch pins only send midi note on/off with no velocity change
- no system for sending touch pin as CC
- the CC assignments are sort of random, probably should have another array for them

*/
 //HEY HEY this is the value to change for touch sensitivity:
  int touchThreshold = 50000; 
  
  
// STRING CONTROLLER
int const numPins = 1; //  number of analog inputs 
int currentVal[numPins];
int newVal[numPins];
int analogPins[] = {  
  19,20,21   // which analog pins to use (19 is pull, 20/21 are x/y)
};


//FOOTSWITCH SENDING NOTE
int const numDigPins = 0; // number of digital pins to send note values
int currentDig[numDigPins];
int digitalpin[] = {
  2    // which digital pins to use for sending note values
};
int digitalpitch[] = {
  48}; // which midi notes to send from the digital pins selected above
int digInput[numDigPins];

//FOOTSWITCH SENDING CC
int const numDigPinsCC = 1; // number of digital pins to send CC
int currentDigcc[numDigPinsCC];
int digitalpincc[] = {
   2 // which digital pins to use for sending CC
};
int digitalcc[] = {
  64}; // which CC vales to send from the digital pins selected above
int digInputcc[numDigPinsCC];


int const numTouchPins = 9 ; // number of pins to use as touchpins, sending note values
int touch[numTouchPins];
int touchon[numTouchPins];
int touchpin[] = {
  0,1,3,4,22,23,15,16,17}; // which digital pins to use as touch pins
  int touchpitch[] = {
  48,54,60,66,72,78,84,90,96}; // which midi notes to send from the touch pins




// the MIDI channel number to send messages
const int channel = 1;



void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);  
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP); 
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.begin(38400);

}

void loop() {

//  touchpads
  for (int i = 0; i < numTouchPins; i++) {
    touch[i] = touchRead(touchpin[i]); 

    if (touch[i] > touchThreshold && touchon[i] == 0) {
usbMIDI.sendNoteOn(touchpitch[i], 100, channel);
      touchon[i] = 1;
    }
    if (touch[i] < touchThreshold && touchon[i] == 1) {
usbMIDI.sendNoteOff(touchpitch[i], 100, channel);
      touchon[i] = 0;
    }
    Serial.println(touch[0]);

  }
 

// digital pins sending notes
  for (int i = 0; i < numDigPins; i++) {
    if (digitalRead(digitalpin[i]) == 1 && currentDig[i] == 0) {
      usbMIDI.sendNoteOff(digitalpitch[i], 100, channel); 
      currentDig[i] = 1;
    }  
    if (digitalRead(digitalpin[i]) == 0  && currentDig[i] == 1) {
      usbMIDI.sendNoteOn(digitalpitch[i], 100, channel);
      currentDig[i] = 0;
    }  
  }

// digital pins sending CC

  for (int i = 0; i < numDigPinsCC; i++) {
    if (digitalRead(digitalpincc[i]) == 1 && currentDigcc[i] == 0) {
      usbMIDI.sendControlChange(digitalcc[i], 0, channel); 
      currentDigcc[i] = 1;
    }  
    if (digitalRead(digitalpincc[i]) == 0  && currentDigcc[i] == 1) {
      usbMIDI.sendControlChange(digitalcc[i], 127, channel);
      currentDigcc[i] = 0;
    }  
  }

// analog pins

  for (int i = 0; i < numPins; i++) {

    newVal[i] = analogRead(analogPins[i]);

    if (abs(newVal[i] - currentVal[i])>3) {
      usbMIDI.sendControlChange(i+1, newVal[i]>>3, channel); 
      currentVal[i] = newVal[i];
    }  
  }
  
  // i think if you remove these last two lines everything breaks and things are sad and people cry
  while (usbMIDI.read()); // read and discard any incoming MIDI messages
  delay(25); 
}







