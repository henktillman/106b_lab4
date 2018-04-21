/******************************************************************************
Flex_Sensor_Example.ino
Example sketch for SparkFun’s flex sensors
(https://www.sparkfun.com/products/10264)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

Create a voltage divider circuit combining a flex sensor with a 47k
resistor.
- The resistor should connect from A0 to GND.
- The flex sensor should connect from A0 to 3.3V
As the resistance of the flex sensor increases (meaning it’s being bent
the
voltage at A0 should decrease.
Development environment specifics:
Arduino 1.6.7
*****************************************************************************/
const int PRESSURE_PIN = A0; // Pin connected to pressure sensor output
const int FLEX_PIN = A1; // Pin connected to voltage divider output
// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor
// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.

const bool is_flex = false; // What type of controller to use

const float DESIRED_ANGLE = 90.0;

const float STRAIGHT_RESISTANCE = 13316.65; // resistance when straight
const float BEND_RESISTANCE = 36424.0; // resistance at 90 deg
void setup()
{
  Serial.begin(9600);
  if (is_flex) {
    pinMode(FLEX_PIN, INPUT);
  } else {
    pinMode(PRESSURE_PIN, INPUT);
  }
  pinMode(3, OUTPUT);
}

float exp_error = 0;
void loop()
{
  float pump_command = 0;
  // Flex angle calculation
  if (is_flex) {
    // Read the ADC, and calculate voltage and resistance from it
    int flexADC = analogRead(FLEX_PIN);
    float flexV = flexADC * VCC / 1023.0;
    float flexR = R_DIV * (VCC / flexV - 1.0);
    // Use the calculated resistance to estimate the sensor’s
    // bend angle:
    float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
    0, 90.0);
    // Serial.println("Bend: " + String(angle) + " degrees");
    // Serial.println();

    float error = max(0, DESIRED_ANGLE - angle);
    pump_command = 30.0 * (error / DESIRED_ANGLE) + 25;
  } else {
    float pressure = analogRead(PRESSURE_PIN);
    // We want to keep the pressure at about 110 for a fully contracted finger.
    float error = 110 - pressure;
    exp_error = 0.2*error + 0.8*exp_error;
    
    Serial.println("Pressure: " + String(pressure));
    pump_command = 70 + 6*min(10, exp_error);
  }
  pump_command = min(pump_command, 100);
//  Serial.println("Pump: " + String(pump_command));
  analogWrite(3, pump_command);
  delay(500);
}
