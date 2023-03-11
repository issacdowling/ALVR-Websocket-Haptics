#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>

const char* ssid = ""; //Enter SSID
const char* password = ""; //Enter Password
const char* websockets_server = "ws://10.0.0.2:8082/api/events-legacy"; //server adress and port

using namespace websockets;

void onMessageCallback(WebsocketsMessage msg) {
  StaticJsonDocument<999> json;
  String message = msg.data();
  
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(json, message);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  String id = json["id"];

  if (id=="Haptics") {
    Serial.println("Test");
  }

}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    }
}

WebsocketsClient client;
void setup() {
    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Setup Callbacks
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    
    // Connect to server
    client.connect(websockets_server);
}

void loop() {
    client.poll();
}