int incomingByte = 0;
#define sensor A0 // Pressure sensor
void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600
}

void loop() {
  Serial.println(analogRead(sensor)); // prints pressure sensor value
  delay(1000);
}