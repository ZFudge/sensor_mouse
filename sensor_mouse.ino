#include <Mouse.h>

const int xPositive = A0;
const int xNegative = A1;
const int yPositive = A2;
const int yNegative = A3;
const int pressRelease = A4;

const int sensorDelay = 1000;

int sensors[] = { 
  xPositive,
  xNegative,
  yPositive,
  yNegative,
  pressRelease
};

void setup() {
  Serial.begin(9600);
  pinMode(xPositive, INPUT);
  pinMode(xNegative, INPUT);
  pinMode(yPositive, INPUT);
  pinMode(yNegative, INPUT);
  pinMode(pressRelease, INPUT);
  Mouse.begin();
}

void loop() {
  int xPositiveReading = touchBinary(0);
  int xNegativeReading = touchBinary(1);
  int yPositiveReading = touchBinary(2);
  int yNegativeReading = touchBinary(3);
  int pressReleaseReading = touchBinary(4);
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
    if (xPositiveReading ^ xNegativeReading) {
      x = xPositiveReading ? -3 : 3;
    }
    if (yPositiveReading ^ yNegativeReading) {
      y = yPositiveReading ? -3 : 3;
    }
    Mouse.move(x, y, 0);
  }
  Serial.println("X_Positive" + String(xPositiveReading));
  Serial.println("X_Negative" + String(xNegativeReading));
  Serial.println("Y_Positive" + String(yPositiveReading));
  Serial.println("Y_Negative" + String(yNegativeReading));
  Serial.println("PRESS" + String(pressReleaseReading));
  Serial.println();
  delay(sensorDelay);
}

int touchBinary(int sensorIndex) {
  int sensorReading = analogRead(sensors[sensorIndex]);
  return sensorReading < 1000;
}

