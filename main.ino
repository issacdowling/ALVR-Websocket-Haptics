#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>

int MOTOR = 16; // Set MOTOR Pin to 16

// Must be floats not ints, or division happens weirdly
float min_haptic_value = 120; // Set the minimum threshold for "on" to 110 of 255
float max_haptic_value = 255; // Set max threshold to 255
int pwm_value;

const char* ssid = ""; //Enter SSID
const char* password = ""; //Enter Password
const char* websockets_server = "ws://10.0.0.2:8082/api/events-legacy"; //server adress and port

using namespace websockets;

void vibe(int duration, int intensity){
  //Normalise intensity from 0-100 to range 120-255
  pwm_value = min_haptic_value+(intensity*((max_haptic_value-min_haptic_value)/100));
  //Jumpstart the motor at max intensity for 20ms to allow starting at low intensities
  analogWrite(MOTOR, 255);
  delay(20);
  //Set motor to selected intensity for as long as input
  analogWrite(MOTOR, pwm_value); // Set MOTOR speed
  delay(duration);
  //After duration complete, stop motor
  analogWrite(MOTOR, 0);
}

void onMessageCallback(WebsocketsMessage msg) {
  //PARSE JSON
    StaticJsonDocument<999> json;
    String message = msg.data();
    // Deserialize the JSON document IF not empty
    if(!msg.isEmpty()){
      DeserializationError error = deserializeJson(json, message);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      //CHECK WHETHER MESSAGE IS FOR THIS DEVICE
      //Check if a haptic message
      String id = json["id"];
      if (id=="Haptics") {
        //Here will go controller checking later
        //String extra = json["data"]["path"]; ("/user/hand/right" or left)
        String length = json["data"]["duration"]["nanos"];
        String amplitude = json["data"]["amplitude"];
        Serial.println(length.toInt()/1000000);
        Serial.println(amplitude.toInt()*100);
        vibe((length.toInt()/1000000), (amplitude.toInt()*100));
      }
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
    pinMode(MOTOR, OUTPUT);
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