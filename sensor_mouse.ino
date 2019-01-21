#include <Mouse.h>
#include <CapacitiveSensor.h>

CapacitiveSensor xPositive = CapacitiveSensor(2,3);
CapacitiveSensor xNegative = CapacitiveSensor(4,5);
CapacitiveSensor pressRelease = CapacitiveSensor(6,7);
CapacitiveSensor slowMovement = CapacitiveSensor(8,9);
CapacitiveSensor yNegative = CapacitiveSensor(10,11);
CapacitiveSensor yPositive = CapacitiveSensor(12,13);

const int sensorThreshold = 100;
const int samples = 8;
const int speed_increment = 4;
int sensorDelay = 25;

int raw_reading = 0;

void setup() {
  Serial.begin(9600);
  Mouse.begin();
}

void loop() {
  long start = millis();

  long pressReleaseReading = !raw_reading ? touchBinary(pressRelease.capacitiveSensor(samples)) : pressRelease.capacitiveSensor(samples);
  long xPositiveReading = !raw_reading ? touchBinary(xPositive.capacitiveSensor(samples)) : xPositive.capacitiveSensor(samples);
  long xNegativeReading = !raw_reading ? touchBinary(xNegative.capacitiveSensor(samples)) : xNegative.capacitiveSensor(samples);
  long yPositiveReading = !raw_reading ? touchBinary(yPositive.capacitiveSensor(samples)) : yPositive.capacitiveSensor(samples);
  long yNegativeReading = !raw_reading ? touchBinary(yNegative.capacitiveSensor(samples)) : yNegative.capacitiveSensor(samples);
  long slowMovementReading = !raw_reading ? touchBinary(slowMovement.capacitiveSensor(samples)) : slowMovement.capacitiveSensor(samples);

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
    int unit = slowMovementReading ? speed_increment : speed_increment * 1.5;
    if (xPositiveReading ^ xNegativeReading) {
      x = xPositiveReading ? -unit : unit;
    }
    if (yPositiveReading ^ yNegativeReading) {
      y = yPositiveReading ? -unit : unit;
    }
    Mouse.move(x, y, 0);
  }
  
  int active = (pressReleaseReading || xPositiveReading || xNegativeReading || yPositiveReading || yNegativeReading || slowMovementReading);
  sensorDelay_Check_Set(active);
  
  delay(sensorDelay);
}

void sensorDelay_Check_Set(int active) {
    if (active && sensorDelay == 750) {
      sensorDelay = 25;
    } else if (!active && sensorDelay == 25) {
      sensorDelay = 750;
    }
}

int touchBinary(long reading) {
  return reading > sensorThreshold;
}

