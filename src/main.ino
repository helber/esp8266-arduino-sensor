#include <NewPing.h>

#define MAX_DISTANCE   200

#define TRIGGER_PIN1    10
#define ECHO_PIN1       11
#define TRIGGER_PIN2     8
#define ECHO_PIN2        9


NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciado...");
};

void loop() {
  int d1, d2;
  delay(100);
  d1 = sonar1.ping_cm();
  delay(100);
  d2 = sonar2.ping_cm();
  Serial.print("{\"sonar1\": ");
  Serial.print(d1);
  Serial.print(", \"sonar2\": ");
  Serial.print(d2);
  Serial.print("}\r\n");
};
