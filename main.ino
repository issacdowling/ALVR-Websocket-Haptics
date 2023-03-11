#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>


//DEBOUNCE INPUTS BEFORE BEING SENT TO VIBE()
//Clean up inputs

const int MOTOR = 16; // Set MOTOR Pin to 16

// Must be floats not ints, or division happens weirdly
const float MIN_HAPTIC_VALUE = 120; // Set the minimum threshold for "on" to 110 of 255
const float MAX_HAPTIC_VALUE = 255; // Set max threshold to 255
int pwm_value;

const char* SSID = ""; //Enter SSID
const char* PASSWORD = ""; //Enter Password
const char* ALVR_ENDPOINT = "ws://10.0.0.2:8082/api/events-legacy"; //ALVR endpoint
const char* PATH = "/user/hand/right";

using namespace websockets;

void vibe(int duration, int intensity){

  Serial.println(duration);
  Serial.println(intensity);

  //Normalise intensity from 0-100 to range 120-255
  pwm_value = MIN_HAPTIC_VALUE+(intensity*((MAX_HAPTIC_VALUE-MIN_HAPTIC_VALUE)/100));
  //Jumpstart the motor at max intensity for 20ms when at low intensities
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
    // Deserialize the JSON document IF not empty
    if(!msg.isEmpty()){
      StaticJsonDocument<999> json;
      DeserializationError error = deserializeJson(json, msg.data());

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
        //Check that the message is for the correct hand
        String recieved_path = json["data"]["path"];
        if (recieved_path==PATH){
          String length = json["data"]["duration"]["nanos"];
          String amplitude = json["data"]["amplitude"];
            
          // Debounces
          now_called_millis = millis();
          if (now_called_millis - last_called_millis  > 50) {
            last_called_millis = now_called_millis;
            vibe((length.toInt()/1000000), (amplitude.toInt()*100));

          }
        }

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
    WiFi.begin(SSID, PASSWORD);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Setup Callbacks
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    
    // Connect to server
    client.connect(ALVR_ENDPOINT);
}

void loop() {
    client.poll();
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
        //Check that the message is for the correct hand
        String recieved_path = json["data"]["path"];
        if (recieved_path==PATH){
          String length = json["data"]["duration"]["nanos"];
          String amplitude = json["data"]["amplitude"];
          Serial.println(length.toInt()/1000000);
          Serial.println(amplitude.toInt()*100);
          vibe((length.toInt()/1000000), (amplitude.toInt()*100));
        }

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
    WiFi.begin(SSID, PASSWORD);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Setup Callbacks
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    
    // Connect to server
    client.connect(ALVR_ENDPOINT);
}

void loop() {
    client.poll();
}