#include <NewPing.h>
#include <SoftwareSerial.h>

// 4800 9600 19200 38400 57600 115200
#define SERIAL_BAUD  115200

#define MAX_DISTANCE   200

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
  esp_serial.begin(SERIAL_BAUD);
  Serial.println("Iniciado...");
};

char * get_json() {
    char ret[100];
    int d1 = -1, d2 = -1;
    delay(200);
    d1 = sonar1.ping_cm();
    delay(200);
    d2 = sonar2.ping_cm();
    snprintf(ret, 100, "{\"sonar1\": %d, \"sonar2\": %d}", d1, d2);
    return ret;
}

void loop() {
    char input[500] = "";
    int pos = 0;
    char * json = "";
    while (esp_serial.available() > 0) {
        input[pos] = esp_serial.read();
        pos++;
    };
    if (strcmp(input, "") != 0){
        Serial.print("SERIAL > [");
        Serial.print(input);
        Serial.print("] ");
    };
    if (strcmp(input, "SENSOR") == 0) {
        Serial.print("GET /sensor");
        json = get_json();
        Serial.print(" SEND:[");
        Serial.print(json);
        Serial.println("]");
        esp_serial.write(json);
        // Serial.print(json);
    };
    if (strcmp(input, "GET /") == 0) {
        Serial.println("SERIAL > GET /");
    };
};
