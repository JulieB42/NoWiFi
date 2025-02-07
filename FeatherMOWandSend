/* RF69 radio receive project by Julie Barrett.
 *  This uses a magic wand to to "throw" the color of the Neopixels in the wand to a Neopixel elsewhere.
 *  This is the transmitting end of the project. 
 * This project uses the Radiohead and Neopixel libraries. Because I had trouble with the structured data, I'm leaving the URL here for future reference.
 * https://www.airspayce.com/mikem/arduino/RadioHead/packingdata.html
 */
 // Include the libraries
#include <Adafruit_NeoPixel.h>
#include <RH_RF69.h>
#include <SPI.h>

 // NEOPIXEL CONFIG
//My magic wand used a set of Neopixels in a tradition LED package going up the staff of the wand, and a Neopixel ring in the star at the top of the wand.
//Yours may only have one set of Neopexels, in which cas you need to comment out
//all of the references to "strip2" and "NUMPIXELS2"
#define PIN 5 //Pin for neopixel
#define PIN2 10 //Pin for neopixel ring
#define NUMPIXELS  4 //How many Neopixels you have on the transmitting end.
#define NUMPIXELS2 12 //How many Neopixels on the ring.

int delayval = 200; //this is the delay for the LEDa so they turn on one at a time
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMPIXELS2, PIN2, NEO_RGB + NEO_KHZ800);

// Set up some variables
uint32_t color;
uint32_t pixeldata;
              
//Set up the push button on the wand. I wired it to pin 6
int buttonPin = 6;    // momentary push button on pin 6
bool buttonVal = LOW;
bool oldButtonVal = HIGH;

//How many light patterns are available, and the starting pattern
int nPatterns = 5;
    int lightPattern = 1;
/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
//#define RF69_FREQ 915.0
#define RF69_FREQ 434.0

//This is where you define the Arduino board you're using
#if defined(ARDUINO_SAMD_ZERO) // Feather M0 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
  #define LED           13
#endif

#if defined(ARDUINO_SAMD_FEATHER_M0) // Feather M0 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
  #define LED           13
#endif
// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

int16_t packetnum = 0;  // packet counter, we increment per xmission

//declare struct
typedef struct {
  int           nodeId; //store this nodeId
  uint32_t      pixeldata; //neopixel value
} 
Payload;
Payload theData;


void setup() {

   Serial.begin(9600);
  //while (!Serial) { delay(1); } // wait until serial console is open, remove if not tethered to computer

  pinMode(LED, OUTPUT);     
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather M0 Wand Transmit!");//This text can be changed to suit your own project
  Serial.println();

  // reset the radio
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
 if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }


  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
//initialize neopixels
 strip.begin();
  strip.show(); // Initialize all pixels to 'off'
 strip2.begin(); // Again, if you have only one set of Neopixels, remove or comment out references to strip2
    strip2.show();             // initialize the BUTTON pin as an input
    pinMode(buttonPin, INPUT_PULLUP);
    digitalWrite(buttonPin, HIGH);  // button pin is HIGH, so it drops to 0 if pressed

}
//Here are the light patterns. It's pretty simple. Both sets of Neopixels
//are set to the same color.
void pattern1() {

  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setBrightness(255);
    strip.setPixelColor(i, strip.Color(255,255,255)); // White

    delay(delayval);
    strip.show();
  }

  for (int i = 0; i < NUMPIXELS2; i++) {
    strip2.setBrightness(255);
    strip2.setPixelColor(i, strip.Color(255, 255, 255)); // White

    strip2.show();

  }

}

void pattern2() {
  strip.setBrightness(100);
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setBrightness(100);
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
    delay(delayval);
    strip.show();
  }
  for (int i = 0; i < NUMPIXELS2; i++) {
    strip2.setBrightness(100);
    strip2.setPixelColor(i, strip.Color(255, 0, 0)); // Red

    strip2.show();
  }
}
void pattern3() {
  strip.setBrightness(100);
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setBrightness(100);
    strip.setPixelColor(i, strip.Color(0, 0, 255)); // Blue
    delay(delayval);
    strip.show();
  }

  for (int i = 0; i < NUMPIXELS2; i++) {
    strip2.setBrightness(100);
    strip2.setPixelColor(i, strip.Color(0, 0, 255)); // Blue

    strip2.show();

  }

}

void pattern4() {
  strip.setBrightness(100);
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setBrightness(100);
    strip.setPixelColor(i, strip.Color(0, 255, 0)); // Green
    delay(delayval);
    strip.show();
  }
  for (int i = 0; i < NUMPIXELS2; i++) {
    strip2.setBrightness(100);
    strip2.setPixelColor(i, strip.Color(0, 255, 0)); // Green

    strip2.show();

  }

}
void pattern5() {
  strip.setBrightness(100);
  for (int i = 0; i < NUMPIXELS; i++) {
  strip.setBrightness(0);
   strip.setPixelColor(i, strip.Color(0, 0, 0)); // Black. In other words, they're all off.
  //  delay(delayval);
    strip.show();
  }
  for (int i = 0; i < NUMPIXELS2; i++) {
  strip2.setBrightness(0);
    strip2.setPixelColor(i, strip.Color(0, 0, 0)); // Black

    strip2.show();

  }
}



void loop() {

// read that state of the pushbutton value;

   buttonVal = digitalRead(buttonPin);

   // If there's a change in the button state, and/or every 1sec...
   if ((buttonVal != oldButtonVal) || (millis() % 1000 == 0)) {   
    
     if ((buttonVal == LOW) && (oldButtonVal == HIGH)) // Transition to button-pressed
     {
      lightPattern = lightPattern + 1;
      if (lightPattern > nPatterns) lightPattern = 1;
     }  
   
    switch(lightPattern) {
    case 1:
      pattern1();
      break;
    case 2:
      pattern2();
      break;
    case 3:
      pattern3();
      break;
    case 4:
      pattern4();
      break;  
 
   case 5:
        pattern5();
        break;
    }    
//Serial.println(strip.getPixelColor(0));
color = (strip.getPixelColor(0));
Serial.print("PixelColor: "); Serial.println(color); //Just want to show that we're getting neopixel data
 
     //fill in the struct with new values
                  //  theData.nodeId = NODEID;  
    theData.pixeldata = color; //This should be what the neopixel sends.
    delay(200); //A slight delay before sending the data just so the color change on the other side isn't instantaneous
  //All this below is data that gets sent to the serial console. Useful for tracking down problems.
  // Serial.print(theData.pixeldata); //testing to see if we really get this. Uncomment if you need it.
  //  Serial.print(theData.nodeId); //testing to see if the NodeID is built. Uncomment if you need it.
    Serial.print("Sending struct (");
    Serial.print(sizeof(theData));
    Serial.print(" bytes) ... ");
    Serial.print("Sending color "); 
    Serial.print(theData.pixeldata);  
    Serial.print(" ");
   Serial.print("sending");

   //Now, let's send it!
   rf69.send((const uint8_t*)(&theData), sizeof(theData));
//  if (rf69.send((theData), sizeof(theData)))
        if (rf69.send((const uint8_t*)(&theData), sizeof(theData)))
      Serial.print(" ok!");
    else Serial.print(" nothing...");
    Serial.println();
  // Send a message!

    
    Serial.println("OK");
   // Blink(LED, 50, 3); //blink LED 3 times, 50ms between blinks //useful for debutgging. 

 Serial.flush(); //make sure all serial data                         is clocked out before sleeping the MCU
  delay(100); 

  }     
  oldButtonVal = buttonVal; //this helps reset the button for the next use.
  }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
