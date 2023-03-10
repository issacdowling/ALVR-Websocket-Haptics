int MOTOR = 16; // Set MOTOR Pin to 16

// Must be floats not ints, or division happens weirdly
float min_haptic_value = 120; // Set the minimum threshold for "on" to 110 of 255
float max_haptic_value = 255; // Set max threshold to 255
int pwm_value;

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

void setup() {
    Serial.begin(9600); // open the serial port at 9600 bps:
    pinMode(MOTOR, OUTPUT);
    Serial.setTimeout(100000);
}

void loop() {
    long input_percent = Serial.parseInt();
    Serial.print(pwm_value);
    Serial.print("\n");
    vibe(1000, input_percent);
}