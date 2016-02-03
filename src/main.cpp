#include <NewPing.h>
#include <SoftwareSerial.h>

// 4800 9600 19200 38400 57600 115200
#define SERIAL_BAUD  115200

#define MAX_DISTANCE   300

#define TRIGGER_PIN1    10
#define ECHO_PIN1       11
#define TRIGGER_PIN2     8
#define ECHO_PIN2        9
#define ESP_TX           6
#define ESP_RX           7

unsigned long d1, d2;
char ret[100];

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
SoftwareSerial esp_serial(ESP_RX, ESP_TX); // RX, TX

void setup() {
  Serial.begin(115200);
  esp_serial.begin(SERIAL_BAUD);
  Serial.flush();
  Serial.println("Iniciado...");
  Serial.flush();
  esp_serial.flush();
};

char * get_json() {
    delay(100);
    d1 = sonar1.ping_cm();
    delay(100);
    d2 = sonar2.ping_cm();
    snprintf(ret, 100, "{\"sonar1\": %lu, \"sonar2\": %lu}", d1, d2);
    return ret;
}

void loop() {
    char input[500] = "";
    int pos = 0;
    while (esp_serial.available() > 0) {
        input[pos] = esp_serial.read();
        pos++;
    };
    delay(100);
    d1 = sonar1.ping_cm();
    delay(100);
    d2 = sonar2.ping_cm();
    if (strcmp(input, "") != 0){
        Serial.print("SERIAL > [");
        Serial.print(input);
        Serial.print("] ");
    };
    if (strcmp(input, "SENSOR") == 0) {
        Serial.print("GET /sensor");
        Serial.print(" SEND:[");
        Serial.print(d1);
        Serial.print(", ");
        Serial.print(d2);
        Serial.println("]");
        esp_serial.print("{\"sonar1\": ");
        esp_serial.print(d1);
        esp_serial.print(", \"sonar2\": ");
        esp_serial.print(d2);
        esp_serial.print("}");
    };
    if (strcmp(input, "GET /") == 0) {
        Serial.println("SERIAL > GET /");
    };
    esp_serial.flush();
    Serial.flush();
};
