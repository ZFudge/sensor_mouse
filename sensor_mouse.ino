#include <CapacitiveSensor.h>
#include <Mouse.h>

CapacitiveSensor pressRelease = CapacitiveSensor(2,3);
CapacitiveSensor xPositive = CapacitiveSensor(4,5);
CapacitiveSensor xNegative = CapacitiveSensor(6,7);
CapacitiveSensor yPositive = CapacitiveSensor(8,9);
CapacitiveSensor yNegative = CapacitiveSensor(10,11);
CapacitiveSensor slowMovement = CapacitiveSensor(12,13);

const int sensorDelay = 500;
const int sensorThreshold = 1000;
const int samples = 8;
const int slowUnit = 3;
const int fastUnit = 6;

void setup() {
  Serial.begin(9600);
//  pressRelease.set_CS_AutocaL_Millis(0xFFFFFFFF);
//  xPositive.set_CS_AutocaL_Millis(0xFFFFFFFF);
//  xNegative.set_CS_AutocaL_Millis(0xFFFFFFFF);
//  yPositive.set_CS_AutocaL_Millis(0xFFFFFFFF);
//  yNegative.set_CS_AutocaL_Millis(0xFFFFFFFF);
//  slowMovement.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Mouse.begin();
}

void loop() {
  long start = millis();

  long pressReleaseReading = pressRelease.capacitiveSensor(samples); // touchBinary(pressRelease.capacitiveSensor(30));
  long xPositiveReading = xPositive.capacitiveSensor(samples); // touchBinary(xPositive.capacitiveSensor(30));
  long xNegativeReading = xNegative.capacitiveSensor(samples); // touchBinary(xNegative.capacitiveSensor(30));
  long yPositiveReading = yPositive.capacitiveSensor(samples); // touchBinary(yPositive.capacitiveSensor(30));
  long yNegativeReading = yNegative.capacitiveSensor(samples); // touchBinary(yNegative.capacitiveSensor(30));
  long slowMovementReading = slowMovement.capacitiveSensor(samples); // touchBinary(slowMovement.capacitiveSensor(30));

  Serial.println(millis() - start);        // check on performance in ms
  Serial.println("PRESS:      " + String(pressReleaseReading));
  Serial.println("X_Positive: " + String(xPositiveReading));
  Serial.println("X_Negative: " + String(xNegativeReading));
  Serial.println("Y_Positive: " + String(yPositiveReading));
  Serial.println("Y_Negative: " + String(yNegativeReading));
  Serial.println("SLOW:       " + String(slowMovementReading));
  Serial.println();

  if (pressReleaseReading) {
    if (!Mouse.isPressed()) {
      Mouse.press();
    }
  } else if (Mouse.isPressed()) {
    Mouse.release();
  }

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
    Mouse.move(x, y, 0);
  }
  
  delay(sensorDelay);
}

int touchBinary(long reading) {
  return reading > sensorThreshold;
}

