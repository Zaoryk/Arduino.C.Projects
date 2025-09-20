#include <SPI.h>
#include <MFRC522.h>
const int RST_PIN = 9;
const int SS_PIN = 10;
MFRC522 mfrc522 (SS_PIN, RST_PIN);

byte validKey1[4] = { 0x86, 0xB7, 0xB4, 0xF8 };

bool isEqualArray(byte* arrayA, byte* arrayB, int length) {
  for (int index  = 0; index < length; index++) {
    if (arrayA[index] != arrayB[index]) return false;
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  // it literally just shows the bytes of your card
  if (mfrc522.PICC_IsNewCardPresent ()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      if (isEqualArray(mfrc522.uid.uidByte, validKey1, 4)) {
        Serial.println("Tarjeta valida");
      }else {
        Serial.println("Tarjeta invalida");
      }
      //uwu
      mfrc522.PICC_HaltA();
    }
  }
  delay(250);
}
