# kotiheating-arduino

IoT KotiHeating project for remote heating control realized with Arduino HW.

## Functionality

KotiHeating offers 3 heating modes:
1) OFF - heating is set to non-freezing level (5°C)
2) MANUAL - allows set heating level from 18°C to 28°C
3) AUTOMATIC - reads temperature from locally saved matrix


## WiFi Connectivity

KotiHeating exposes following endpoints via WIFI:
- TBD.
- TBD.

## Internet Connectivity

Kotiheating is periodically connecting to server endpoint to update AUTOMATIC matrix 
and to save last known state of the device.

Server matrix can be changed on the server TBD.

Server expose last known state of the KotiHeating via API TBD.
