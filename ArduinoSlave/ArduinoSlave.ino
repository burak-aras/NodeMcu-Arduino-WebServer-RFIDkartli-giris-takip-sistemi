int rs_pin = 9;
int ss_pin = 10;
int buzzer = 4;
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <MFRC522.h>
#include <SPI.h>
SoftwareSerial nodemcu(5, 6);
MFRC522 rfid{ ss_pin, rs_pin };

void setup() {

  SPI.begin();
  rfid.PCD_Init();
  Serial.begin(115200);
  nodemcu.begin(115200);
  Serial.println("Program started");
  pinMode(buzzer,OUTPUT);
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) {
    if (rfid.PICC_ReadCardSerial()) {

      String a = String(rfid.uid.uidByte[0]);
      String b = String(rfid.uid.uidByte[1]);
      String c = String(rfid.uid.uidByte[2]);
      String d = String(rfid.uid.uidByte[3]);
      Serial.println("");
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      String idNumarasi = a + b + c + d; 
      StaticJsonBuffer<1000> jsonBuffer;
      JsonObject& data = jsonBuffer.createObject();
      data["idNumarasi"] = idNumarasi;
      data.printTo(nodemcu);
      jsonBuffer.clear();
    }
    rfid.PICC_HaltA();
  }
}