#include <Mouse.h>

const int xPositive = A0;
const int xNegative = A1;
const int yPositive = A2;
const int yNegative = A3;
const int pressRelease = A4;
const int slowMovement = A5;

const int sensorDelay = 1000;

int sensors[] = { 
  xPositive,
  xNegative,
  yPositive,
  yNegative,
  pressRelease,
  slowMovement
};

void setup() {
  Serial.begin(9600);
  pinMode(xPositive, INPUT);
  pinMode(xNegative, INPUT);
  pinMode(yPositive, INPUT);
  pinMode(yNegative, INPUT);
  pinMode(pressRelease, INPUT);
  pinMode(slowMovement, INPUT);
  Mouse.begin();
}

void loop() {
  int xPositiveReading =  analogRead(0); // touchBinary(0);
  int xNegativeReading =  analogRead(1); // touchBinary(1);
  int yPositiveReading =  analogRead(2); // touchBinary(2);
  int yNegativeReading =  analogRead(3); // touchBinary(3);
  int pressReleaseReading =  analogRead(4); // touchBinary(4);
  int slowMovementReading =  analogRead(5); // touchBinary(5);
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
    int unit = slowMovement ? 3 : 6;
    if (xPositiveReading ^ xNegativeReading) {
      x = xPositiveReading ? -unit : unit;
    }
    if (yPositiveReading ^ yNegativeReading) {
      y = yPositiveReading ? -unit : unit;
    }
    Mouse.move(x, y, 0);
  }
  Serial.println("X_Positive: " + String(xPositiveReading));
  Serial.println("X_Negative: " + String(xNegativeReading));
  Serial.println("Y_Positive: " + String(yPositiveReading));
  Serial.println("Y_Negative: " + String(yNegativeReading));
  Serial.println("PRESS:      " + String(pressReleaseReading));
  Serial.println("SLOW        " + String(slowMovementReading));
  Serial.println();
  delay(sensorDelay);
}

int touchBinary(int sensorIndex) {
  int sensorReading = analogRead(sensors[sensorIndex]);
  return sensorReading < 1000;
}

