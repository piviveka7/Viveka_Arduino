//Connect A0 pin to the analog input to the Heart beat and the ECG pins 


#include "ThingSpeak.h"
#include <ESP8266WiFi.h> // ESP8266WiFi.h library
#include <SPI.h>



const char* ssid     = "belkin.767";
const char* password = "password";
const char* host = "api.thingspeak.com";
const char* writeAPIKey = "ZF9ZSUO5KVUXHHL1";

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String APIKey = "ZF9ZSUO5KVUXHHL1";             // enter your channel's Write API Key
const int updateThingSpeakInterval = 5 * 1000; // 20 second interval at which to update ThingSpeak

// Variable Setup
long lastConnectionTime = 0;
boolean lastConnected = false;

// Initialize Arduino Ethernet Client
WiFiClient client;


void setup() {
  
  delay(1000);

//  Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
  delay(500);
  }
}

void loop() {
 // read values from pins and store as strings
  String light = String(analogRead(A0), DEC); // read light value

  // Print Update Response to Serial Monitor
  if (client.available()) 
  {
    char c = client.read();
    Serial.print(c);
  }
 // Update ThingSpeak
  if (!client.connected() && (millis() - lastConnectionTime > updateThingSpeakInterval)) 
  {
    updateThingSpeak("field1=" + light );
    Serial.println(light);

  }

  lastConnected = client.connected();

  // make TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(light);
  url+="\r\n";
  
  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    delay(1000);

}

void updateThingSpeak(String tsData) {
  if (client.connect(thingSpeakAddress, 80)) 
  {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + APIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
    lastConnectionTime = millis();

    if (client.connected()) 
    {
      Serial.println("Connecting to ThingSpeak...");
      Serial.println();
    }
  }

}

