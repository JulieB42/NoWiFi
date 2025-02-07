//Code by Julie Barrett, but highly based on the following:
/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
//I also used Adafruit's code for the Adafruit_TCS34725 color sensor.

//I used a QTPy ESP32 C3 for this project.

#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#include "Adafruit_TCS34725.h"
// REPLACE WITH YOUR RECEIVER MAC Address
//To get the MAC address in the Arduino IDE,connect your board, choose the board and port, then select Get Board Info under Tools.
uint8_t broadcastAddress1[] = {0xF4, 0x12, 0xFA, 0x44, 0x1F, 0xF0};
uint8_t broadcastAddress2[] = {0xD4, 0xF9, 0x8D, 0x70, 0x85, 0x9C};
uint8_t broadcastAddress3[] = {0x9C, 0xF9E, 0x6E, 0xF7, 0x2D, 0x54};
  uint32_t color = 0;
  uint32_t col = 0;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {

  uint32_t color;
  //uint32_t col;
} 
struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;
// Variable to store if sending data was successful
String success;
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// our RGB -> eye-recognized gamma color
byte gammatable[256];


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
  //Neopixels
  #define NUMPIXELS        1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
   
//register peers. One peer per broadcastAddressX above.
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register first peer  
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer1");
    return;

  }
  // register second peer  
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer2");
    return;
  }
    // register second peer  
  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer3");
    return;
  }
 //this bit of code is via Adafruit
  Serial.println("Color View Test!");
  
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
  
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
      
    gammatable[i] = x;      
    //Serial.println(gammatable[i]);
  }
  
  for (int i=0; i<3; i++){ //this sequence flashes the first pixel three times as a countdown to the color reading.
   pixels.setPixelColor (0, pixels.Color(188, 188, 188)); //white, but dimmer-- 255 for all three values makes it blinding!
    
    pixels.show();
    delay(1000);
    pixels.setPixelColor (0, pixels.Color(0, 0, 0));
    pixels.show();
    delay(500);
  }
  
  uint16_t clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED
  
  Serial.print("C:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(red);
  Serial.print("\tG:\t"); Serial.print(green);
  Serial.print("\tB:\t"); Serial.print(blue);

  // Figure out some basic hex code for visualization
  uint32_t sum = red;
  sum += green;
  sum += blue;
  //sum += clear; // clear contains RGB already so no need to re-add it
  
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  Serial.print("\t");
  Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  Serial.println();
  
  Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );
  colorWipe(pixels.Color(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]), 0);

}

// Fill the dots one after the other with a color. This will fill any Neopixels connected to the board. 
//For this example we're using the on board Neopixel.
//The actual fill command is in the main program loop.
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, c);
      pixels.show();
      delay(wait);
col = pixels.getPixelColor(i);
Serial.print("pixel color: ");
Serial.print(col);
Serial.println();     
  }
  //neopixels
  
#if defined(NEOPIXEL_POWER)
  // If this board has a power control pin, we must set it to output and high
  // in order to enable the NeoPixels. We put this in an #if defined so it can
  // be reused for other boards without compilation errors
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(20); // not so bright
}
 //Here is where we send the color data to the other device(s) on the network
void loop() {
  Serial.print("sent pixel color: ");
Serial.print(col);
Serial.println();  
 // Set values to send

  myData.color = col;

  // Send message via ESP-NOW
esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));

   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);

  //Neopixels
    pixels.fill(myData.color);
  pixels.show();

 
   }
