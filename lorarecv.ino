/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define ss 5
#define rst 14           //define the pins used by the transceiver module
#define dio0 2

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
String LoRaData;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Receiver");

  connectAWS();
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void(* resetFunc) (void) = 0;

void loop() {
  const char* message;
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();





      message = LoRaData.c_str();
      int questionCheck = 0;
      for (int i = 0; message[i] != '\0'; ++i)
      {
        if (message[i] == '⸮')
        {
          resetFunc();
          break;
        }
      }
      if(client.publish(AWS_IOT_PUBLISH_TOPIC, message)==0)
        resetFunc();

      client.loop();
      delay(5000);
    }
   
    // print RSSI of packet
 
    Serial.println(LoRa.packetRssi());
  }
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//Connect to AWS
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("Fixbox-78F637", "MTU2NGRi");

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  // Create a message handler
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");

}
