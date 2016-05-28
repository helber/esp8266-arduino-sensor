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

unsigned long d1, d2, runid;
bool stringComplete = false;
String input = "";
// char ret[200];

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
SoftwareSerial esp_serial(ESP_RX, ESP_TX); // RX, TX

void setup() {
  Serial.begin(115200);
  while (!Serial) {;}
  esp_serial.begin(SERIAL_BAUD);
  Serial.flush();
  Serial.println("Iniciado...");
  Serial.flush();
  esp_serial.flush();
  input.reserve(200);
  runid = 0;
};

void update_dist() {
    d1 = sonar1.ping_cm();
    delay(80);
    d2 = sonar2.ping_cm();
}

String get_json() {
    String ret;
    int sec = millis() / 1000;
    ret = "{\"sonar1\":";
    ret += d1;
    ret += ",\"sonar2\":";
    ret += d2;
    ret += ",\"sec\":";
    ret += sec;
    ret += "}";
    // snprintf(ret, "{\"sonar1\": %lu, \"sonar2\": %lu, \"sec\": %d}", d1, d2, sec);
    // snprintf(ret, 200, "{\"sonar1\": %lu, \"sonar2\": %lu}", d1, d2);
    return ret;
}

void loop() {
    if ((runid / 50000) == 1) {
        update_dist();
        // Serial.println(get_json());
        runid = 0;
        delay(1);
        Serial.println(get_json());
    } else {
        runid++;
    };
    esp_serial.listen();
    while (esp_serial.available()) {
        char inChar = (char) esp_serial.read();
        // add it to the inputString:
        input += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        Serial.print((int)inChar);Serial.print(" ");
        if (inChar == 'R') {
          stringComplete = true;
          Serial.println(";");
        }
    }
    if (stringComplete) {
        int sec = millis() / 1000;
        // esp_serial.print(get_json());
        esp_serial.print("{\"sonar1\": ");
        esp_serial.print(d1);
        esp_serial.print(", \"sonar2\": ");
        esp_serial.print(d2);
        esp_serial.print(", \"sec\": ");
        esp_serial.print(sec);
        esp_serial.print("}");
        input = "";
        stringComplete = false;
    }
    /*
    if (strcmp(input, "") != 0){
        Serial.print("SERIAL > [");
        Serial.print(input);
        Serial.print("] ");
        Serial.print(get_json());
    };
    if (strcmp(input, "SENSOR") == 0) {
        Serial.print("GET /sensor");
        Serial.print(" SEND:[");
        Serial.print(get_json());
        Serial.println("]");
        esp_serial.print(get_json());
    };
    if (strcmp(input, "GET /") == 0) {
        Serial.println("SERIAL > GET /");
    };
    */
};
