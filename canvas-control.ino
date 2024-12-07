#include "wifi_secrets.h"
#include <WiFiNINA.h>
#include <SPI.h>


// PORT DEFINITIONS
#define SPIWIFI SPI
#define AIRLIFT_CS 10 // Chip select gpio
#define AIRLIFT_BUSY 7
#define AIRLIFT_RESET 5
#define AIRLIFT_GPIO0 -1 // Not connected


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial) {
    Serial.println("Serial connecting..."); // Wait for serial port to connect. Can't communicate otherwise?
    delay(10);
  }

  // This is an added function in the Adafruit WiFiNINA library
  // Needed ofc because the wifi is coming from is a breakout
  WiFi.setPins(AIRLIFT_CS, AIRLIFT_BUSY, AIRLIFT_RESET, AIRLIFT_GPIO0, &SPIWIFI);

  // WL_NO_MODULE and WL_NO_SHIELD are equivalent
  while(WiFi.status() == WL_NO_MODULE) {
    Serial.println("No connection with WiFi module");
    delay(500);
  }

  // There is also a string data type. As well as String class
  // Arduino frowns on the 'new' and other dynamic allocations, because it can eventually lead to memory fragmentation 
  // Making the memory unstable and eventually unusable; causing program to fail
  char wifi_ssid[] = WIFI_SSID;
  char wifi_pass[] = WIFI_PASS;

  Serial.print("Attempting to connect to network SSID: ");
  Serial.println(wifi_ssid);

  do {
    WiFi.begin(wifi_ssid, wifi_pass);
    delay(100);
  } while (WiFi.status() != WL_CONNECTED);

  Serial.print("Successfully connected to: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
