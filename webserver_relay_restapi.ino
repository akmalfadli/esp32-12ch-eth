
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>  // Include Preferences library for storage
#include <WebServer_ESP32_W5500.h>

#define SCREEN_WIDTH 128 // Orelay display width, in pixels
#define SCREEN_HEIGHT 64 // Orelay display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define Orelay_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, Orelay_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   35
#define LOGO_WIDTH    35

#if !( defined(ESP32) )
#error This code is designed for (ESP32 + W5500) to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_       3


static const unsigned char PROGMEM logo_bmp[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00,
  0xff, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xf8, 0x00, 0x03, 0xe8,
  0x0f, 0xf8, 0x00, 0x03, 0xe8, 0x03, 0xfc, 0x00, 0x07, 0xea, 0x09, 0xfc, 0x00, 0x07, 0xea, 0x04,
  0xfe, 0x00, 0x0f, 0xea, 0x12, 0xfe, 0x00, 0x0f, 0xea, 0x08, 0x7f, 0x00, 0x1f, 0xea, 0xe0, 0x7f,
  0x80, 0x3f, 0xea, 0xe4, 0x7f, 0x80, 0x3f, 0xeb, 0xc8, 0x7f, 0xc0, 0x3f, 0xe8, 0x0a, 0xff, 0xc0,
  0x7f, 0xef, 0xc2, 0xff, 0xc0, 0x7f, 0xe8, 0x0c, 0xff, 0xc0, 0x3f, 0xea, 0x01, 0xff, 0xc0, 0x3f,
  0xea, 0x07, 0xff, 0x80, 0x1f, 0xea, 0xff, 0xff, 0x80, 0x1f, 0xea, 0xff, 0xff, 0x00, 0x0f, 0xea,
  0xff, 0xff, 0x00, 0x0f, 0xea, 0xff, 0xfe, 0x00, 0x07, 0xea, 0xff, 0xfc, 0x00, 0x03, 0xee, 0xff,
  0xfc, 0x00, 0x03, 0xe0, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xf0,
  0x00, 0x00, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


int relay[12] = {2, 16, 17, 13, 15, 14, 12, 25, 26, 33, 32, 27};
int relayState[12] = {0};  // Array to hold relay states (0 = OFF, 1 = ON)
int relayLength = sizeof(relay) / sizeof(relay[0]);
// the setup function runs once when you press reset or power the board



WiFiServer server(80);
Preferences preferences;  // Preferences instance for NVS storage

// Enter a MAC address and IP address for your controller below.
#define NUMBER_OF_MAC      20


#define DEFAULT_SECRET_KEY "Abc123"
String secretKey;

byte mac[][NUMBER_OF_MAC] =
{
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14 },
};

// Select the IP address according to your local network
IPAddress myIP(192, 168, 11, 153);
IPAddress myGW(192, 168, 11, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);
IPAddress localIP;

int reqCount = 0;                // number of requests received

void setup() {
  Serial.begin(115200);
  while (!Serial && (millis() < 5000));

  // Initialize Orelay Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation fairelay"));
    for (;;); // Stop execution
  }

  drawLogo();
  ESP32_W5500_onEvent();
  ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST );
  
  bool config(IPAddress myIP, IPAddress myGW, IPAddress mySN, IPAddress myDNS);
  ETH.config(myIP, myGW, mySN, myDNS);

  // Static IP, leave without this line to get IP via DHCP
  bool config(IPAddress myIP, IPAddress myGW, IPAddress mySN, IPAddress myDNS);
  ETH.config(myIP, myGW, mySN, myDNS);

  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 10000; // 10 seconds timeout

  preferences.begin("auth", false);
  secretKey = preferences.getString("key", DEFAULT_SECRET_KEY);
  preferences.end();
  displayMessage("Waiting for Ethernet connection...");

  while (!ETH.linkUp() && (millis() - startAttemptTime) < timeout) {
    Serial.println(F("Waiting for Ethernet connection..."));
    delay(500);
  }

  if (!ETH.linkUp()) {
    Serial.println(F("Ethernet not connected! Running in offline mode."));
    displayMessage("Ethernet not connected!");
  } else {
    Serial.println(F("Ethernet Connected!"));
    displayMessage("Ethernet Connected!");
  }

  server.begin();

  // Get Ethernet IP address
  localIP = ETH.localIP();

  // Initialize Preferences and Load Saved relay States
  preferences.begin("relay_states", false);
  for (int i = 0; i < relayLength; i++) {
    pinMode(relay[i], OUTPUT);
    relayState[i] = preferences.getInt(("relay" + String(i)).c_str(), 0);  // Load state, default to OFF
    digitalWrite(relay[i], relayState[i]);  // Restore relay state
  }
  preferences.end();
  updateOrelay(localIP);  // Draw initial relay states
}

// the loop function runs over and over again forever
void loop() {

  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println(F("New client"));
    String request = "";
    bool currentLineIsBlank = true;
    String authHeader = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        Serial.write(c);

        if (request.indexOf("Authorization: Bearer ") >= 0) {
          int authIndex = request.indexOf("Authorization: Bearer ") + 22;
          authHeader = request.substring(authIndex, request.indexOf("\r\n", authIndex));
        }
        if (c == '\n' && currentLineIsBlank) {
          if (request.indexOf("GET /set-auth") >= 0) {
            handleSetAuth(client, request, authHeader);
          } else if (request.indexOf("GET /relay?") >= 0) {
            handleRelayControl(client, request, authHeader);
          } else if (request.indexOf("GET /relay-state") >= 0) {
            handleRelayState(client, request, authHeader);
          } else {
            client.print("HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
            client.print("{\"status\": \"error\", \"message\": \"Invalid request\"}");
          }
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(10);
    client.stop();
    Serial.println(F("Client disconnected"));
  }
}

void handleRelayControl(WiFiClient &client, String &request, String &authHeader) {

  if (authHeader != secretKey) {
    client.print("HTTP/1.1 401 Unauthorized\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print("{\"status\": \"error\",\"message\": \"Unauthorized\"}");
    return;
  }
  // Turn ON/OFF relay Request
  int indexPos = request.indexOf("ch=") + 3;
  int index = request.substring(indexPos).toInt() - 1;

  int statePos = request.indexOf("state=") + 6;
  int state = request.substring(statePos).toInt();

  if (index >= 0 && index < relayLength && (state == 0 || state == 1)) {
    digitalWrite(relay[index], state);
    relayState[index] = state;  // Update state

    // Save state to Preferences
    preferences.begin("relay_states", false);
    preferences.putInt(("relay" + String(index)).c_str(), state);
    preferences.end();

    updateOrelay(localIP); // Update Orelay display

    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print("{\"status\": \"success\"}");
  } else {
    client.print("HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print("{\"status\": \"error\", \"message\": \"Invalid relay index or state\"}");
  }
}

void handleRelayState(WiFiClient &client, String &request, String &authHeader) {

  if (authHeader != secretKey) {
    client.print("HTTP/1.1 401 Unauthorized\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print("{\"status\": \"error\",\"message\": \"Unauthorized\"}");
    return;
  }

  // API to get all relay states
  if (request.indexOf("GET /relay-state") >= 0) {
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");

    if (request.indexOf("ch=") >= 0) {
      // Get individual relay state
      int indexPos = request.indexOf("ch=") + 3;
      int index = request.substring(indexPos).toInt() - 1;

      if (index >= 0 && index < relayLength) {
        preferences.begin("relay_states", false);
        int state = preferences.getInt(("relay" + String(index)).c_str(), 0);
        preferences.end();

        client.print("{\"status\": \"success\", \"data\": {\"channel\": " + String(index + 1) + ", \"state\": " + String(state) + "}}");
      } else {
        client.print("{\"status\": \"error\", \"message\": \"Invalid relay index\"}");
      }
    } else {
      // Get all relay states
      preferences.begin("relay_states", false);
      client.print("{\"status\": \"success\", \"data\": [");
      for (int i = 0; i < relayLength; i++) {
        if (i > 0) client.print(",");
        client.print(preferences.getInt(("relay" + String(i)).c_str(), 0));
      }
      preferences.end();
      client.print("]}");
    }
  }
}

void handleSetAuth(WiFiClient &client, String &request, String &authHeader) {

  if (authHeader != secretKey) {
    client.print("HTTP/1.1 401 Unauthorized\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print("{\"status\": \"error\",\"message\": \"Unauthorized\"}");
    return;
  }

  int keyPos = request.indexOf("new-key=") + 8;
  if (keyPos > 7) {
    String newKey = request.substring(keyPos, request.indexOf(" ", keyPos));
    preferences.begin("auth", false);
    preferences.putString("key", newKey);
    preferences.end();
    secretKey = newKey;

    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print("{\"status\": \"success\",\"message\": \"Key updated successfully\"}");
  } else {
    client.print("HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print("{\"status\": \"error\", \"message\": \"Missing new-key parameter\"}");
  }

}

void displayMessage(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}


void drawLogo(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(2000);
}

void updateOrelay(IPAddress ip) {
  display.clearDisplay();

  // Display IP Address at the top (centered)
  display.setTextSize(1);
  display.setTextColor(WHITE);
  String ipString = "IP: " + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
  int ipLength = ipString.length() * 6;  // Calculate the width of the IP string
  int ipX = (SCREEN_WIDTH - ipLength) / 2;  // Center the IP string
  display.setCursor(ipX, 5);
  display.print(ipString);

  // relay Display Grid Settings (center the grid)
  int xStart = (SCREEN_WIDTH - (6 * 10 + 5 * 5)) / 2;  // Calculate starting X to center the grid
  int yStart = 25;  // Start a little lower to avoid overlap with IP text
  int size = 10;    // Square size
  int padding = 5;  // Space between squares
  int columns = 6;  // 6 relays per row

  // Draw relay squares and labels (numbers above the squares)
  for (int i = 0; i < relayLength; i++) {
    int x = xStart + (i % columns) * (size + padding);  // Calculate X position for each square
    int y = yStart + (i / columns) * (size + padding + 5);  // Calculate Y position for each square

    // Draw the relay number above the square
    display.setCursor(x + size / 2 - 3, y - 8);  // Center the number
    display.print(i + 13);  // ganti nomor display kotak-kotak indikator

    // Draw the relay square (filrelay if ON, empty if OFF)
    if (relayState[i] == 1) {
      display.fillRect(x, y, size, size, WHITE);  // Filrelay square for ON
    } else {
      display.drawRect(x, y, size, size, WHITE);  // Empty square for OFF
    }
  }
  display.display();
}
