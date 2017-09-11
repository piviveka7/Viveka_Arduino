//#include <DHT.h> //  DHT.h library
#include "ThingSpeak.h"
#include <ESP8266WiFi.h> // ESP8266WiFi.h library
#include <SPI.h>

//#define DHTPIN 2
//#define DHTTYPE  DHT11

const char* ssid     = "belkin.767";
const char* password = "7a989637";
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

//DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {

  
  // Initialize sensor
 //dht.begin();
  delay(1000);
 Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -

//  Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

    }
}

void loop() {
 // read values from pins and store as strings
  
  if((digitalRead(13) == 1)||(digitalRead(15) == 1)){
    Serial.println('!');
  }
  else{
    // send the value of analog input 0:
      Serial.println(analogRead(A0));
  }
  //Wait for a bit to keep serial data from saturating
  delay(1);
  
  String light = String(analogRead(A0), DEC); // read light value

  // find temp value
   //float voltage = analogRead(A1) * (3.3 / 1024);  // convert 0-1023 range to 3.3V range
  // int tempVal = (voltage - 0.5) * 100;            // convert voltage to temperature in *C
   //String temp = String(tempVal);

  // Print Update Response to Serial Monitor
  if (client.available()) 
  {
    char c = client.read();
    Serial.print(c);
  }
 //float humidity = dht.readHumidity();
 //float temperature = dht.readTemperature();
 // Update ThingSpeak
  if (!client.connected() && (millis() - lastConnectionTime > updateThingSpeakInterval)) 
  {
    updateThingSpeak("field1=" + light );
    Serial.println(light);
    //Serial.println(temp);

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
  //url+="&field2=";
  //url+=String(humidity);
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

