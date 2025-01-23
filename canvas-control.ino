#define ARDUINO_UNO

#include "wifi_secrets.h"
#include <WiFiNINA.h>
#include <SPI.h>
#include <WebThingAdapter.h>

// PORT DEFINITIONS
#define SPIWIFI         SPI
#define AIRLIFT_CS      10 // Chip select gpio
#define AIRLIFT_BUSY    7
#define AIRLIFT_RESET   5
#define AIRLIFT_GPIO0   -1 // Not connected

#define LED_STRIP_PIN 9

WebThingAdapter *adapter;
const char *canvasTypes[] = {"Light", "ColorControl", nullptr};
ThingDevice canvas("urn:dev:canvas-by-tae", "Anime Canvas", canvasTypes);

//** Thing Properties Declarations */

ThingProperty lightsOn("on", "Whether the canvas is in a static on state.", BOOLEAN, "OnOffProperty");
bool lastOn = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  while(!Serial) {
    Serial.println("Serial connecting..."); // Wait for serial port to connect. Can't communicate otherwise?
    delay(10);
  }

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

    ThingPropertyValue initialOn = {.boolean = false};
    lightsOn.setValue(initialOn);
    (void)lightsOn.changedValueOrNull();
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
}
