int LED = 2; // Set LED Pin to 2


void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:

pinMode(LED, OUTPUT);

}

void loop() {

    for(int duty_cycle = 255;  duty_cycle > 0; duty_cycle--) {
    Serial.print(duty_cycle);
    Serial.print("\n");
    analogWrite(LED, duty_cycle); // Set LED speed
    delay(200); // wait
    }

}