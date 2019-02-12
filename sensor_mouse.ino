#include <Mouse.h>
#include <CapacitiveSensor.h>

CapacitiveSensor yPositive = CapacitiveSensor(0, 1);
CapacitiveSensor pressReleaseRight = CapacitiveSensor(2, 3);
CapacitiveSensor yNegative = CapacitiveSensor(4, 5);
CapacitiveSensor slowMovement = CapacitiveSensor(6, 7);
CapacitiveSensor xPositive= CapacitiveSensor(8, 9);
CapacitiveSensor pressReleaseLeft = CapacitiveSensor(10, 11);
CapacitiveSensor xNegative = CapacitiveSensor(12, 13);

const int sensorThreshold = 250;
const int samples = 8;
const int speed_increment = 4;
const int long_delay = 200;
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
  int pressReleaseLeftReading = !raw_reading ? touchBinary(pressReleaseLeft.capacitiveSensor(samples)) : pressReleaseLeft.capacitiveSensor(samples);
  int pressReleaseRightReading = !raw_reading ? touchBinary(pressReleaseRight.capacitiveSensor(samples)) : pressReleaseRight.capacitiveSensor(samples);
  int xPositiveReading = !raw_reading ? touchBinary(xPositive.capacitiveSensor(samples)) : xPositive.capacitiveSensor(samples);
  int xNegativeReading = !raw_reading ? touchBinary(xNegative.capacitiveSensor(samples)) : xNegative.capacitiveSensor(samples);
  int yPositiveReading = !raw_reading ? touchBinary(yPositive.capacitiveSensor(samples)) : yPositive.capacitiveSensor(samples);
  int yNegativeReading = !raw_reading ? touchBinary(yNegative.capacitiveSensor(samples)) : yNegative.capacitiveSensor(samples);
  int slowMovementReading = !raw_reading ? touchBinary(slowMovement.capacitiveSensor(samples)) : slowMovement.capacitiveSensor(samples);

  if (!raw_reading) {
    mousePress(pressReleaseLeftReading, pressReleaseRightReading);
    mouseMovement(xPositiveReading, xNegativeReading, yPositiveReading, yNegativeReading, slowMovementReading);
  }
  
  int active = (pressReleaseLeftReading || pressReleaseRightReading || xPositiveReading || xNegativeReading || yPositiveReading || yNegativeReading || slowMovementReading);
  sensorDelay_Check_Set(active);
  
  if (print_readings) {
    int readings[7] = { pressReleaseLeftReading, pressReleaseRightReading, xPositiveReading, xNegativeReading, yPositiveReading, yNegativeReading, slowMovementReading };
    long performance = millis() - start;
    printReadings(performance, readings);
  }
  
  delay(sensorDelay);
}

void mouseMovement(int xPos, int xNeg, int yPos, int yNeg, int slow) {
  if ((xPos ^ xNeg) || (yPos ^ yNeg)) {
    int x = 0;
    int y = 0;
    int unit = slow ? speed_increment : speed_increment * 1.65;
    if (xPos ^ xNeg) {
      x = xNeg ? -unit : unit;
    }
    if (yPos ^ yNeg) {
      y = yPos ? -unit : unit;
    }
    Mouse.move(x, y, 0);
  }
  
}

void mousePress(int left, int right) {
  if (left && !Mouse.isPressed(MOUSE_LEFT)) {
    Mouse.press(MOUSE_LEFT);
  } else if (!left && Mouse.isPressed(MOUSE_LEFT)) {
    Mouse.release(MOUSE_LEFT);
  }
  if (right && !Mouse.isPressed(MOUSE_RIGHT)) {
    Mouse.press(MOUSE_RIGHT);
  } else if (!right && Mouse.isPressed(MOUSE_RIGHT)) {
    Mouse.release(MOUSE_RIGHT);
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

void printReadings(long performance, int readings[7]) {
  Serial.println(performance);  // check on performance in ms
  Serial.println("LEFT_PRESS:      " + String(readings[0]));
  Serial.println("LEFT_RIGHT:      " + String(readings[1]));
  Serial.println("X_Positive: " + String(readings[2]));
  Serial.println("X_Negative: " + String(readings[3]));
  Serial.println("Y_Positive: " + String(readings[4]));
  Serial.println("Y_Negative: " + String(readings[5]));
  Serial.println("SLOW:       " + String(readings[6]));
  Serial.println();
}

