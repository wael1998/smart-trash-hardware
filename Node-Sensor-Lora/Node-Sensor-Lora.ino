#include <SPI.h>
#include <LoRa.h> 


int pot = A0;
int Gas_analog = A4;    // used for ESP32
const int trigPin = 4;
const int echoPin = 3;
long duration;
int distance;


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  pinMode(pot,INPUT);
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  while (!Serial);  
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of yout module
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void loop() {
  Serial.println("LoRa sending");
  int gassensorAnalog = analogRead(Gas_analog);
  Serial.println(gassensorAnalog);

    int distance=DistanceCal();
  
    Serial.print("Distance: ");
    Serial.println(distance);
   
    Serial.print("Gas: ");
    Serial.println(gassensorAnalog);
  
    LoRa.beginPacket();  
    LoRa.print(gassensorAnalog);
    LoRa.print(distance);
    LoRa.endPacket();
    delay(1000);
    
 
}



int DistanceCal()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  return distance;
  }
