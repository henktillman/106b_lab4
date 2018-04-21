int incomingByte = 0;

void setup() {
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming entered byte
    incomingByte = Serial.parseInt();
    Serial.print("I received: ");
    Serial.println(incomingByte);
    incomingByte = min(incomingByte, 100);
    analogWrite(3, incomingByte); // write the user entered value (0-254) to pwm output
  }
  delay(1000); // change this to be whatever frequency you want
}
