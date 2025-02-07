/* RF69 radio receive project by Julie Barrett.
 *  This uses a magic wand to to "throw" the color of the Neopixels in the wand to a Neopixel elsewhere.
 *  This is the receiving end of the project. 
 *  Using a Feather RFM69HCW. Select Adafruit Feather M0 (SAMD 21) as the board.
 * This project uses the Radiohead and Neopixel libraries. Because I had trouble with the structured data, I'm leaving the URL here for future reference.
 * https://www.airspayce.com/mikem/arduino/RadioHead/packingdata.html
 */
 // Include the libraries
#include <Adafruit_NeoPixel.h>
#include <RH_RF69.h>
#include <SPI.h>

// NEOPIXEL CONFIG
#define PIN 6 //pin for Neopixel. Change this for whatever board you're using 
#define NUMPIXELS 50 //receiving end.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Set up some variables
uint32_t color;
uint32_t pixeldata;
uint32_t      colorstate = 0; //store the value to check for change


/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
//#define RF69_FREQ 915.0
#define RF69_FREQ 434.0

//This is where you define the Arduino board you're using
#if defined (__AVR_ATmega32U4__) // Feather 32u4 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     7
  #define RFM69_RST     4
  #define LED           13
#endif

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
/*#if defined (__AVR_ATmega328P__)  // Feather 328P w/wing
  #define RFM69_INT     3  // 
  #define RFM69_CS      4  //
  #define RFM69_RST     2  // "A"
  #define LED           13
#endif

#if defined(ESP8266)    // ESP8266 feather w/wing
  #define RFM69_CS      2    // "E"
  #define RFM69_IRQ     15   // "B"
  #define RFM69_RST     16   // "D"
  #define LED           0
#endif

#if defined(ESP32)    // ESP32 feather w/wing
  #define RFM69_RST     13   // same as LED
  #define RFM69_CS      33   // "B"
  #define RFM69_INT     27   // "A"
  #define LED           13
#endif

 Teensy 3.x w/wing
#define RFM69_RST     9   // "A"
#define RFM69_CS      10   // "B"
#define RFM69_IRQ     4    // "C"
#define RFM69_IRQN    digitalPinToInterrupt(RFM69_IRQ )
*/
 
/* WICED Feather w/wing 
#define RFM69_RST     PA4     // "A"
#define RFM69_CS      PB4     // "B"
#define RFM69_IRQ     PA15    // "C"
#define RFM69_IRQN    RFM69_IRQ
*/



// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

//Set up your structured data here. It should match the data types on the transmitter end.
typedef struct {
  int           nodeId; //store this nodeId
  uint32_t      pixeldata; //neopixel value coming from xmiter

} Payload;
Payload theData;

void setup() {

  Serial.begin(9600);
//while (!Serial) {} //comment this line out unless you need the serial monitor. Otherwise, it may not receive data.
pinMode(LED, OUTPUT);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);
  
  Serial.println("Feather Wand Receiver");//You can change this to whatever you wish to identify your project
  
    // manual reset radio
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
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
   strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //pinMode(LED, OUTPUT);

 Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");

}
String scolor="theData.pixeldata";
void sparks(const char* colorName){
  // Random sparks - just one LED on at a time!
  Serial.printf("sparks: %s\n", theData);
  Serial.printf("scolor: ", scolor);
  unsigned long color = 0;
  sscanf(colorName + 1, "%X", &scolor);
  for (int count = 0; count < 25; count++)
  {
    int i = random(NUMPIXELS);
    strip.setPixelColor(i, theData.pixeldata);
    strip.show();
    delay(50);
    strip.setPixelColor(i, 0);
  }   
}

void loop() {

     if (rf69.available()) {
    // The transmission end should have sent you something
      uint8_t datalen = sizeof(theData);
  
   if (   rf69.recv((uint8_t*)&theData, &datalen)
    && datalen == sizeof(theData));
 
    //The stuff below is for the serial monitor  
 Serial.println(" ");

 Serial.print(" ");
      Serial.print("Received: [");
        Serial.print("RSSI: ");
      Serial.println(rf69.lastRssi(), DEC);
     Serial.print("pixeldata: ");
      Serial.println(theData.pixeldata);
   //  //   Serial.print("Pixel Data Before");
    Serial.print(theData.pixeldata);
        Serial.print(" ");
    Serial.print(" ");
     Serial.print("Color State ");
      Serial.println(colorstate);
//this block of code sends the color received to the Neopixel
       // if (colorstate != theData.pixeldata) {
   // for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
//strip.show();
  //  strip.setPixelColor(i,(theData.pixeldata));
    strip.clear();
  sparks(scolor.c_str());
    colorstate=(theData.pixeldata);
  }   
        }
  
 
//       delay(10);

                                                                                             
     
    //And you're done!
