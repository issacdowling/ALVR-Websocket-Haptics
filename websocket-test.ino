#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>


const String IP = "10.0.0.2";
const char* ssid = ""; //Enter SSID
const char* password = ""; //Enter Password

using namespace websockets;

WebsocketsClient client;

StaticJsonDocument<200> doc;

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps

  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait for wifi connection
  for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }

  String PC_ALVR_ADDRESS = "ws://" + IP + ":8082/api/events";
  client.connect(PC_ALVR_ADDRESS);

  client.onMessage([](WebsocketsMessage msg){
    DeserializationError error = deserializeJson(doc, msg.rawData());
    Serial.println(msg.rawData().c_str());

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

  });



  String test = doc["id"];
  Serial.println(test);

}

void loop() {
  client.poll();
}


