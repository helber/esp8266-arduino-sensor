#include <NewPing.h>
#include <SoftwareSerial.h>

#define MAX_DISTANCE   300

#define TRIGGER_PIN1    10
#define ECHO_PIN1       11
#define TRIGGER_PIN2     8
#define ECHO_PIN2        9
#define ESP_TX           6
#define ESP_RX           7

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
SoftwareSerial esp_serial(ESP_RX, ESP_TX); // RX, TX

void setup() {
  Serial.begin(115200);
  esp_serial.begin(115200);
  Serial.println("Iniciado...");
};

char * get_json() {
    char ret[100];
    int d1 = -1, d2 = -1;
    delay(200);
    d1 = sonar1.ping_cm();
    Serial.println(d1);
    delay(200);
    d2 = sonar2.ping_cm();
    Serial.println(d2);
    snprintf(ret, 100, "{\"sonar1\": %d, \"sonar2\": %d}", d1, d2);
    Serial.println(ret);
    return ret;
}

void loop() {
    String input = "";
    char * json = "";
    while (esp_serial.available() > 0) {
        input += (char) esp_serial.read();
        // delay(10); // Nao sei se e necessario
    };

    if (input == "SENSOR") {
        Serial.println("Recebeu SENSOR... enviando:");
        json = get_json();
        esp_serial.print(json);
        Serial.print(json);
    };
    if (input == "GET /") {
        Serial.println("Caregou /");
    };
    if (input != ""){
        Serial.print(" > ");
        Serial.println(input);
    };
};
