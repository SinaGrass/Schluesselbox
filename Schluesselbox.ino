#include <ESP32Servo.h>

const int SERVO_PIN = D2;
const int POS_A = 0;
const int POS_B = 180;
const int FAHRZEIT_MS = 2500;  // so lange darf er drehen, dann ist Schluss

Servo servo;
bool beiB = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Bereit. K senden.");
}

void loop() {
  if (Serial.available() > 0) {
    char taste = Serial.read();
    if (taste == 'k' || taste == 'K') {
      int ziel = beiB ? POS_A : POS_B;
      Serial.print("Fahre auf ");
      Serial.println(ziel);

      servo.setPeriodHertz(50);
      servo.attach(SERVO_PIN, 500, 2400);
      servo.write(ziel);
      delay(FAHRZEIT_MS);
      servo.detach();

      beiB = !beiB;
      Serial.println("Fertig, Servo aus.");
    }
  }
}
