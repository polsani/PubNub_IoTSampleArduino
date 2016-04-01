
#include <SPI.h>
#include <Ethernet.h>
#include <PubNub.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177);

char pubkey[] = "demo";
char subkey[] = "demo";
char channel[] = "ChannelArduino1";

String message = "";
char c;

PubSubClient *pclient;

void checkPubNubMessage()
{
  pclient = PubNub.subscribe(channel);
  if (!pclient) {
    Serial.println("subscription error");
    delay(1000);
    return;
  }
  message = "";
  while (pclient->wait_for_data()) {
    Serial.println("chegou aqui 4");
    c = pclient->read();
    message.concat(c);
  }
  Serial.println(message);
  pclient->stop();
}

void setup() {
  pinMode(7, OUTPUT);  
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }

  PubNub.begin(pubkey, subkey);
  
}

void loop() {
  
  checkPubNubMessage();

  message.replace("[","");
  message.replace("]","");

  if(message.length() >= 2){
    message.remove(0,1);
    message.remove(message.length() -1 , 1);
  }

  while(message == "blink") {
    digitalWrite(7, HIGH);
    delay(1000);
    digitalWrite(7, LOW);
    delay(1000);
    checkPubNubMessage();
  }
}
