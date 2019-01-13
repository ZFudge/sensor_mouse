#include <CapacitiveSensor.h>
//#include <Mouse.h>

CapacitiveSensor pressRelease = CapacitiveSensor(2,3);
CapacitiveSensor xPositive = CapacitiveSensor(4,5);
CapacitiveSensor xNegative = CapacitiveSensor(6,7);
CapacitiveSensor yPositive = CapacitiveSensor(8,9);
CapacitiveSensor yNegative = CapacitiveSensor(10,11);
CapacitiveSensor slowMovement = CapacitiveSensor(12,13);

const int sensorDelay = 500;
const int sensorThreshold = 500;
const int slowUnit = 3;
const int fastUnit = 6;

int sensors[] = {
  pressRelease,
  xPositive,
  xNegative,
  yPositive,
  yNegative,
  slowMovement
};

void setup() {
  Serial.begin(9600);
  // turn off autocalibrate on channel 1 - just as an example
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  // Mouse.begin();
}

void loop() {
  long start = millis();
  
  long pressReleaseReading = touchBinary(0);
  long xPositiveReading = touchBinary(1);
  long xNegativeReading = touchBinary(2);
  long yPositiveReading = touchBinary(3);
  long yNegativeReading = touchBinary(4);
  long slowMovementReading = touchBinary(5);

  Serial.println(millis() - start);        // check on performance in ms
  Serial.println("PRESS: " + String(pressReleaseReading));
  Serial.println("X_Positive: " + String(xPositiveReading));
  Serial.println("X_Negative: " + String(xNegativeReading));
  Serial.println("Y_Positive: " + String(yPositiveReading));
  Serial.println("Y_Negative: " + String(yNegativeReading));
  Serial.println("SLOW: " + String(slowMovementReading));
  Serial.println();

  // if (pressReleaseReading) {
  //   if (!Mouse.isPressed()) {
  //     Mouse.press();
  //   }
  // } else if (Mouse.isPressed()) {
  //   Mouse.release();
  // }

  if ((xPositiveReading ^ xNegativeReading) || (yPositiveReading ^ yNegativeReading)) {
    int x = 0;
    int y = 0;
    int unit = slowMovement ? slowUnit : fastUnit;
    if (xPositiveReading ^ xNegativeReading) {
      x = xPositiveReading ? -unit : unit;
    }
    if (yPositiveReading ^ yNegativeReading) {
      y = yPositiveReading ? -unit : unit;
    }
    // Mouse.move(x, y, 0);
  }
  
  delay(sensorDelay);
}

int touchBinary(int index) {
  long reading = sensors[index].capacitiveSensor(30);
  return reading > sensorThreshold;
}

