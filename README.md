# ESP32 12-Channel SSR Relay Controller with Ethernet (W5500) and OLED Display

## Overview
This project is an ESP32-based 12-channel Solid State Relay (SSR) controller that uses an Ethernet module (W5500) for network communication and a 0.96-inch OLED display for real-time status updates. The controller allows remote relay operation via an HTTP-based API.

## Features
- **ESP32-based**: Uses an ESP32 microcontroller for processing and control.
- **12-Channel SSR Relay Control**: Supports individual relay control over Ethernet.
- **W5500 Ethernet Module**: Provides stable wired network connectivity.
- **0.96" OLED Display**: Displays relay states and network status.
- **Web API for Remote Control**: Allows HTTP requests to toggle relays and retrieve their states.
- **Secure Access with Authorization**: Uses a secret key for authentication.
- **Persistent Storage**: Saves relay states in non-volatile storage (Preferences).

## Hardware Requirements
- ESP32 development board
- 12-channel SSR relay module
- W5500 Ethernet module
- 0.96-inch OLED display (I2C)
- Power supply (5V or as required by your components)

## Software Requirements
- Arduino IDE
- ESP32 board support package
- Required libraries:
  - `SPI.h`
  - `Wire.h`
  - `Adafruit_GFX.h`
  - `Adafruit_SSD1306.h`
  - `Preferences.h`
  - `WebServer_ESP32_W5500.h`

## Wiring Diagram
| Component | ESP32 Pin |
|-----------|------------|
| OLED SDA  | GPIO 21 |
| OLED SCL  | GPIO 22 |
| W5500 MISO | GPIO 19 |
| W5500 MOSI | GPIO 23 |
| W5500 SCK  | GPIO 18 |
| W5500 CS   | GPIO 5  |
| W5500 INT  | GPIO 4  |
| Relay 1    | GPIO 2  |
| Relay 2    | GPIO 16 |
| Relay 3    | GPIO 17 |
| Relay 4    | GPIO 13 |
| Relay 5    | GPIO 15 |
| Relay 6    | GPIO 14 |
| Relay 7    | GPIO 12 |
| Relay 8    | GPIO 25 |
| Relay 9    | GPIO 26 |
| Relay 10   | GPIO 33 |
| Relay 11   | GPIO 32 |
| Relay 12   | GPIO 27 |

## API Endpoints
### 1. **Set Authentication Key**
```
GET /set-auth?key=NEW_SECRET_KEY
```
- Changes the secret key for authentication.

### 2. **Control Relay**
```
GET /relay?ch=X&state=Y
```
- `X` = Relay channel (1-12)
- `Y` = Relay state (0 for OFF, 1 for ON)

### 3. **Get Relay State**
```
GET /relay-state
```
- Returns the state of all relays.

```
GET /relay-state?ch=X
```
- Returns the state of a specific relay.

## Setup & Installation
1. Install the required libraries in Arduino IDE.
2. Upload the sketch to the ESP32.
3. Connect the W5500 Ethernet module and power up the system.
4. Configure a static IP or use DHCP.
5. Control relays using the provided API endpoints.

## Display Functions
- **Startup Message**: Displays a startup message and network status.
- **Relay Status Update**: Updates the screen with relay states when changed.

## Security & Notes
- The project uses a secret key stored in Preferences for authentication.
- Make sure to use a secure network to prevent unauthorized access.
- Default secret key: `Abc123` (change it using `/set-auth` API).

## Future Enhancements
- Add MQTT support for better IoT integration.
- Implement a web-based control panel.
- Add real-time clock (RTC) for scheduled relay operations.

## License
This project is open-source under the MIT License.

---
**Author:** Akmal Fadli
**Date:** 2025

