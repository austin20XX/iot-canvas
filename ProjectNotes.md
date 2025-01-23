
## Hardware Considerations
  The LED Strip uses 5v logic. Microcontroller must have 5v power out, 5v logic 

## Webthing-Arduino
  Makefile specific 6.13.0 of ArduinoJson. 6.14.0 has breaking changes.
  Use WiFiNina library by defining macro to trigger that load path

  Thing Devices take id and title, but Thing Properties take id and description; So title must be set on property manually.
  adapter->update() adds significant program memory; Too much for my arduino uno :( . Goes from 15610(48% usage) to 47340(146%) usage....

  ## W3C Web Of Things
  ### Thing Devices 
  The context property is a mandatory property for things. It is used used to provide a URI for a schema repository which defines standard schemas for common "types" of device capabilities. For webthings-arduino library, context is https://webthings.io/schema.

## General Learnings
  When a program is monitoring the serial port of the device, uploads fail. Commonly happens to me when I'm listening through VS Code Serial Monitor, just stop monitoring to fix.

