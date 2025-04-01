
## Hardware Considerations
  The LED Strip uses 5v logic. Microcontroller must have 5v power out, 5v logic
  WS2812b IS GRB color

  Gamma corrections! Use when feeding uint32 colors into setPixelColor. Needed because of phenomenom with our eyes that I won't/cant explain here

## Webthing-Arduino
  Makefile specific 6.13.0 of ArduinoJson. 6.14.0 has breaking changes.
  Use WiFiNina library by defining macro to trigger that load path

  Thing Devices take id and title, but Thing Properties take id and description; So title must be set on property manually.
  adapter->update() adds significant program memory; Too much for my arduino uno :( . Goes from 15610(48% usage) to 47340(146%) usage....

  Anything over one property seems to require the LARGE_JSON_BUFFERS

  Adding a delay in the loop seems to prevent the properties from updating? I'm not 100% sure, but I think this is an example of the differences between HTTP vs MQTT; MQTT is an observable protocol and can queue messages when the client is asleep/disconnected. HTTP has no guarantees that the message is processed or recieved by the client.

  ## W3C Web Of Things
  ### Thing Devices 
  The context property is a mandatory property for things. It is used used to provide a URI for a schema repository which defines standard schemas for common "types" of device capabilities. For webthings-arduino library, context is https://webthings.io/schema.

  The brightness property is in percent, so should map from the 0 - 100 range to desired brightness range

## General Learnings
  When a program is monitoring the serial port of the device, uploads fail. Commonly happens to me when I'm listening through VS Code Serial Monitor, just stop monitoring to fix.
  Waiting for Serial connection hangs your code when not connected to a PC (or any device with serial connection). Solution I think could be #ifndef WAIT_FOR_SERIAL macro? Allow for easy switching in and out instead of commenting out code

  USING STRINGS WITH ARDUINO
    Typically frowned upon to use the String object, because it can lead to heap fragmentation since it uses dynamic memory allocation. Using C character array 'strings' is preferred instead. Use n versions of str functions (such as strncpy vs strcpy) to prevent buffer overflow attacks, because you'll only write the intended amount of memory
    char *string = "This is text"; Intended to be read only, might misbehave if modified. Size is 13 chars
    char string[] = "This is text"; Intended to be writable. Size is 13 chars
    char string[30] = "This is text"; Writable, size is 30 chars

## Board Notes
  Adafruit Feather M0 SAMD21 will use Wifi101 Adapter, but WiFiNina library (M0 processor, ATWINC 1500 wifi chip)
  Arduino Nano 33 IoT will use WiFi101 adapter, WiFi 101 library(M0 processor, ESP32 wifi blox)