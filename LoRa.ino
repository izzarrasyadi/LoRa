#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 15
#define dio0 4
// sck 18
// mosi 23
// miso 18
int i = 3020;
int incomingByte = 0;
void setup() {
  //initialize Serial Monitor
  pinMode(17, OUTPUT);
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("LoRa Receiver");
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  LoRa.setTxPower(20);  // Supported values are 2 to 20
  // LoRa.setSpreadingFactor(12);      // Supported values are between 6 and 12
  // LoRa.setSignalBandwidth(10.4E3);  // Supported values are 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3, and 500E3.
  // LoRa.setCodingRate4(8);                // Supported values are between 5 and 8, these correspond to coding rates of 4/5 and 4/8
  LoRa.setGain(6);  // Supported values are between 0 and 6
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    //Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      tone(17, 4700);  // Send sound signal...
      delay(100);      // ...for 1 sec
      noTone(17);      // Stop sound...
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData);
    }
  }

  //send packet from Serial monitor
  if (Serial.available() > 0) {
    //read serial
    String kirim = Serial.readString();
    Serial.print("kirim: ");
    Serial.println(kirim);
    //send packet
    LoRa.beginPacket();
    LoRa.print(kirim);
    LoRa.endPacket();
  }
}