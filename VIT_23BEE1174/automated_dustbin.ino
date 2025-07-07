#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 6;

Servo lidServo;

const int openDistance = 20;      // Distance threshold in cm
const int openAngle = 90;
const int closeAngle = 0;

bool isLidOpen = false;
int currentAngle = closeAngle;
unsigned long lastDetectionTime = 0;
const unsigned long lidOpenDuration = 3000; // Stay open for 5 seconds

void setup() {
  Serial.begin(9600);
  lidServo.attach(servoPin);
  lidServo.write(closeAngle);
  currentAngle = closeAngle;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.print("Initial Lid Angle: ");
  Serial.println(currentAngle);
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure distance
  long duration = pulseIn(echoPin, HIGH, 30000);  // 30 ms timeout
  int distance = duration * 0.034 / 2;

  unsigned long currentTime = millis();

  // If object is within range
  if (distance > 0 && distance < openDistance) {
    lastDetectionTime = currentTime;
    if (!isLidOpen) {
      lidServo.write(openAngle);
      currentAngle = openAngle;
      isLidOpen = true;
      Serial.print("Lid opened. Angle: ");
      Serial.println(currentAngle);
    }
  }

  // If enough time has passed since last detection, close the lid
  if (isLidOpen && (currentTime - lastDetectionTime > lidOpenDuration)) {
    lidServo.write(closeAngle);
    currentAngle = closeAngle;
    isLidOpen = false;
    Serial.print("Lid closed. Angle: ");
    Serial.println(currentAngle);
  }

  delay(100);
}




