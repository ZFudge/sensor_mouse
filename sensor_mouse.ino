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
const int long_delay = 500;
const int short_delay = 20;
int sensorDelay = long_delay;

int print_readings = 0;
int raw_reading = 0;

void setup() {
  Serial.begin(9600);
  Mouse.begin();
}

void loop() {
  long start = millis();
  int pressReleaseReading = !raw_reading ? touchBinary(pressRelease.capacitiveSensor(samples)) : pressRelease.capacitiveSensor(samples);
  int xPositiveReading = !raw_reading ? touchBinary(xPositive.capacitiveSensor(samples)) : xPositive.capacitiveSensor(samples);
  int xNegativeReading = !raw_reading ? touchBinary(xNegative.capacitiveSensor(samples)) : xNegative.capacitiveSensor(samples);
  int yPositiveReading = !raw_reading ? touchBinary(yPositive.capacitiveSensor(samples)) : yPositive.capacitiveSensor(samples);
  int yNegativeReading = !raw_reading ? touchBinary(yNegative.capacitiveSensor(samples)) : yNegative.capacitiveSensor(samples);
  int slowMovementReading = !raw_reading ? touchBinary(slowMovement.capacitiveSensor(samples)) : slowMovement.capacitiveSensor(samples);
  
  mousePress(pressReleaseReading);
  mouseMovement(xPositiveReading, xNegativeReading, yPositiveReading, yNegativeReading, slowMovementReading);

  int active = (pressReleaseReading || xPositiveReading || xNegativeReading || yPositiveReading || yNegativeReading || slowMovementReading);
  sensorDelay_Check_Set(active);
  
  if (print_readings) {
    int readings[6] = { pressReleaseReading, xPositiveReading, xNegativeReading, yPositiveReading, yNegativeReading, slowMovementReading };
    long performance = millis() - start;
    printReadings(performance, readings);
  }
  
  delay(sensorDelay);
}

void mouseMovement(int xPos, int xNeg, int yPos, int yNeg, int slow) {
  if ((xPos ^ xNeg) || (yPos ^ yNeg)) {
    int x = 0;
    int y = 0;
    int unit = slow ? speed_increment : speed_increment * 1.5;
    if (xPos ^ xNeg) {
      x = xPos ? -unit : unit;
    }
    if (yPos ^ yNeg) {
      y = yPos ? -unit : unit;
    }
    Mouse.move(x, y, 0);
  }
  
}

void mousePress(int pressed) {
  if (pressed) {
    if (!Mouse.isPressed()) {
      Mouse.press();
    }
  } else if (Mouse.isPressed()) {
    Mouse.release();
  }
}

void sensorDelay_Check_Set(int active) {
    if (active && sensorDelay == long_delay) {
      sensorDelay = short_delay;
    } else if (!active && sensorDelay == short_delay) {
      sensorDelay = long_delay;
    }
}

int touchBinary(long reading) {
  return reading > sensorThreshold;
}

void printReadings(long performance, int readings[6]) {
  Serial.println(performance);  // check on performance in ms
  Serial.println("PRESS:      " + String(readings[0]));
  Serial.println("X_Positive: " + String(readings[1]));
  Serial.println("X_Negative: " + String(readings[2]));
  Serial.println("Y_Positive: " + String(readings[3]));
  Serial.println("Y_Negative: " + String(readings[4]));
  Serial.println("SLOW:       " + String(readings[5]));
  Serial.println();
}

