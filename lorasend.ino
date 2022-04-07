#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

int pot = A0;
int Gas_analog = A4;    // used for ESP32
const int trigPin = 3;
const int echoPin = 2;
// defines variables
long duration;
int distance;
void setup() {
  Serial.begin(9600);
  pinMode(pot, INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of yout module
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.println("LoRa sending");
  int gassensorAnalog = analogRead(Gas_analog);
  Serial.println(gassensorAnalog);


  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor


  //char dis[1000];
  //char gaz[1000];
  //sprintf(dis, "%d", distance);
  //sprintf(gaz, "%d", gassensorAnalog);


  StaticJsonDocument<200> doc;
  doc["gaz"] = gassensorAnalog;
  doc["distance"] = distance;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client


  LoRa.beginPacket();
  LoRa.print(jsonBuffer);
  LoRa.endPacket();
  delay(1000);


}
