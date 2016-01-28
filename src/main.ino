#include <NewPing.h>
#include <SoftwareSerial.h>

#define MAX_DISTANCE   200

#define TRIGGER_PIN1    10
#define ECHO_PIN1       11
#define TRIGGER_PIN2     8
#define ECHO_PIN2        9
#define ESP_TX           3
#define ESP_RX           2

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
SoftwareSerial esp_serial(ESP_RX, ESP_TX); // RX, TX

void setup() {
  Serial.begin(115200);
  esp_serial.begin(9600);
  Serial.println("Iniciado...");
};

String get_json() {
    String ret = "";
    int d1, d2;
    delay(100);
    d1 = sonar1.ping_cm();
    delay(100);
    d2 = sonar2.ping_cm();
    ret += "{\"sonar1\": ";
    ret += d1;
    ret += ", \"sonar2\": ";
    ret += d2;
    ret += "}";
    return ret;
}


void loop() {
    String input = "";
    while (esp_serial.available() > 0) {
        input += (char) esp_serial.read();
        // delay(5); // Nao sei se e necessario
    }

    if (input == "GET\n") {
        Serial.println("Recebeu GET... enviando");
        String json = get_json();
        esp_serial.println(json);
        Serial.println(json);
    }

    if (input == "\n") {
        Serial.println("Recebeu newline");
    }
};
