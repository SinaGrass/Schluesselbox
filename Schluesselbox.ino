// Schlüsselbox – Arduino Nano ESP32 + Micro Servo 9g (MS18)
// Wird im Seriellen Monitor die Taste "K" gesendet, dreht der Servo etwa eine halbe Umdrehung (WINKEL_START -> WINKEL_ZIEL).
// Beim nächsten "K" dreht er wieder zurück.
//
// Benötigt die Bibliothek "ESP32Servo" (Bibliotheksverwalter -> "ESP32Servo" von Kevin Harrington)

#include <ESP32Servo.h>

// Anschluss Servo -> Nano ESP32
// Braun/Schwarz = GND, Rot = 5V (VBUS), Orange/Gelb = Signal
const int SERVO_PIN = D2;

// Pulsbreiten für 0° und 180° in Mikrosekunden
// (falls der Servo die vollen 180° nicht erreicht oder am Anschlag brummt, hier anpassen)
const int PULS_MIN_US = 500;
const int PULS_MAX_US = 2400;

// Endwinkel leicht unter 0°/180°, damit der Servo nicht gegen Anschlag/Deckel drückt
// (falls der Deckel nicht ganz aufgeht, WINKEL_ZIEL schrittweise erhöhen)
const int WINKEL_START = 5;
const int WINKEL_ZIEL = 170;

// Pause pro Grad in Millisekunden (größer = langsamer und sanfter)
const int PAUSE_PRO_GRAD_MS = 5;

// Wartezeit nach der Bewegung, bevor das Signal abgeschaltet wird
const int HALTEZEIT_MS = 300;

Servo servo;
int aktuellerWinkel = WINKEL_START;

// Servo komplett stromlos schalten: PWM abhängen und Pin fest auf LOW ziehen,
// damit garantiert keine Pulse mehr ankommen und der Servo nicht nachregelt
void servoAus() {
  servo.detach();
  pinMode(SERVO_PIN, OUTPUT);
  digitalWrite(SERVO_PIN, LOW);
  Serial.println("Servo-Signal aus.");
}

void fahreZu(int zielWinkel) {
  servo.attach(SERVO_PIN, PULS_MIN_US, PULS_MAX_US);
  servo.write(aktuellerWinkel);  // erst die aktuelle Position halten, dann losfahren
  int richtung = (zielWinkel > aktuellerWinkel) ? 1 : -1;
  while (aktuellerWinkel != zielWinkel) {
    aktuellerWinkel += richtung;
    servo.write(aktuellerWinkel);
    delay(PAUSE_PRO_GRAD_MS);
  }
  delay(HALTEZEIT_MS);
  servoAus();
}

void setup() {
  Serial.begin(115200);
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, PULS_MIN_US, PULS_MAX_US);
  servo.write(aktuellerWinkel);
  delay(HALTEZEIT_MS);
  servoAus();
  Serial.println("Bereit. Taste K senden fuer eine halbe Umdrehung.");
}

void loop() {
  if (Serial.available() > 0) {
    char taste = Serial.read();
    if (taste == 'k' || taste == 'K') {
      int ziel = (aktuellerWinkel == WINKEL_START) ? WINKEL_ZIEL : WINKEL_START;
      Serial.print("Fahre auf ");
      Serial.print(ziel);
      Serial.println(" Grad...");
      fahreZu(ziel);
      Serial.println("Fertig.");
    }
  }
}
