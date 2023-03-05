int LED = 2; // Set LED Pin to 2


void setup() {
    Serial.begin(9600); // open the serial port at 9600 bps:
    pinMode(LED, OUTPUT);
    Serial.setTimeout(100000);
}

void loop() {

    // for(int duty_cycle = 255;  duty_cycle > 113; duty_cycle--) {
    //     Serial.print(duty_cycle);
    //     Serial.print("\n");
    //     analogWrite(LED, 120); // Set LED speed
    //     delay(200); // wait
    // }
    long myInt = Serial.parseInt();
    Serial.print(myInt);
    Serial.print("\n");
    analogWrite(LED, myInt); // Set LED speed
}