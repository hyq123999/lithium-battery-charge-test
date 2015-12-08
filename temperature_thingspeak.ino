#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid     = "...";
const char* password = "...";
const char* host     = "api.thingspeak.com";
const char* apiKey   = "...";
const int pin = 14;

OneWire oneWire(pin);
DallasTemperature sensors(&oneWire);
DeviceAddress thermometer;

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.printf("Chip id: %x", ESP.getChipId());
  Serial.printf("\nHeap: %u\n", ESP.getFreeHeap());

  sensors.begin();
  sensors.setResolution(TEMP_12_BIT);
  if (!sensors.getAddress(thermometer, 0)) {
    Serial.println(F("Unable to find address for Device 0"));
  }

  Serial.print(F("Thermometer 0 id: "));
  printAddress(thermometer);
  Serial.println();

  Serial.print(F("Connecting to WiFi: "));
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print(F("\nWiFi connected: "));
  Serial.println(WiFi.localIP());
}


void loop() {
  Serial.printf("\nHeap: %u\n", ESP.getFreeHeap());
  sensors.requestTemperatures();
  float temperature = sensors.getTempC(thermometer);

  Serial.print(F("Temperature: "));
  Serial.println(temperature);
  reportTemperature(temperature);

  delay(30000);
}

void reportTemperature(float temperature) {
  Serial.print(F("Connecting to "));
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println(F("connection failed"));
    return;
  }

  String url = "/update?api_key=";
  url += apiKey;
  url += "&field1=";
  url += temperature;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(10);

  while(client.available()) {
    String line = client.readStringUntil('\r');
    line.trim();
    if (line.startsWith("Status:") || line.startsWith("Date:")) {
      Serial.println(line);
    }
  }

  Serial.println();
  Serial.println(F("closing connection"));
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
