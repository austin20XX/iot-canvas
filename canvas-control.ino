#define ARDUINO_UNO

#include "wifi_secrets.h"
#include <WiFiNINA.h>
#include <SPI.h>
#include <WebThingAdapter.h>
#include <Adafruit_NeoPixel.h>

// PORT DEFINITIONS
#define SPIWIFI         SPI
#define AIRLIFT_CS      10 // Chip select gpio
#define AIRLIFT_BUSY    7
#define AIRLIFT_RESET   5
#define AIRLIFT_GPIO0   -1 // Not connected

#define LED_STRIP_PIN 2
#define NUM_PIXELS 10

WebThingAdapter *adapter;
const char *canvasTypes[] = {"Light", "ColorControl", nullptr};
ThingDevice canvas("urn:michigan-ave:canvas-by-tae", "Anime Canvas", canvasTypes);

Adafruit_NeoPixel ws2812b(NUM_PIXELS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

//** Thing Properties Declarations */

ThingProperty lightsOn("on", "Whether the canvas is in a static on state.", BOOLEAN, "OnOffProperty");
bool lastOn = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  // while(!Serial) {
  //   Serial.println("Serial connecting..."); // Wait for serial port to connect. Can't communicate otherwise?
  //   delay(10);
  // }

  connectToWifi();

  adapter = new WebThingAdapter("led-lamp", WiFi.localIP());

  canvas.description = "The electric floating canvas";
  lightsOn.title = "Light Strip";

  canvas.addProperty(&lightsOn);
  
  adapter->addDevice(&canvas);
  adapter->begin();

  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(canvas.id);

  digitalWrite(LED_BUILTIN, LOW);
  ThingPropertyValue initialOn = {.boolean = false};
  lightsOn.setValue(initialOn);
  // Casted b/c this can actually return nullptr?
  (void)lightsOn.changedValueOrNull();


  ws2812b.begin();
  ws2812b.setPixelColor(0, ws2812b.Color(0, 127, 0));
  ws2812b.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  adapter->update();

  bool on = lightsOn.getValue().boolean;

    if (on) {
    digitalWrite(LED_BUILTIN, HIGH);
    } else {
    digitalWrite(LED_BUILTIN, LOW);
    }
}

void connectToWifi() {
    digitalWrite(LED_BUILTIN, HIGH);
  // This is an added function in the Adafruit WiFiNINA library
  // Needed ofc because the wifi is coming from a breakout
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

  digitalWrite(LED_BUILTIN, LOW);
}
