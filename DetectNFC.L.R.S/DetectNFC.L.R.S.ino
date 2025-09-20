// includes
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
//motor

Servo servo;
const int SERVO_PIN = A0;
const int SERVO_INICIAL = 90;
const int SERVO_CERRAR = 0;

//SCREEN

#define I2C_ADDR 0x27
LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);

//LED RING

#define LED_PIN 1
#define NUM_LEDS 12
Adafruit_NeoPixel ring(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

//NFC SCANNER

const int RST_PIN = 9;
const int SS_PIN = 10;
MFRC522 mfrc522(SS_PIN, RST_PIN);

//VALID UID
byte validKey1[4] = { 0x86, 0xB7, 0xB4, 0xF8 }; //this is MY nfc card, put your own there.

//LED COMPARISON
bool isEqualArray(byte* arrayA, byte* arrayB, int length) {
  for (int index  = 0; index < length; index++) {
    if (arrayA[index] != arrayB[index]) return false;
  }
  return true;
}
//SET
void setup() {
  Serial.begin(9600);
  
  // MOTOR
  servo.attach(SERVO_PIN);
  servo.write(SERVO_CERRAR);

  // SCREEN
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Preparandose");
  // RING
  ring.begin();
  ring.show();
  ring.setBrightness(40);
  // NFC SCANNER

  SPI.begin();
  mfrc522.PCD_Init();
  delay(1000);
}
// LED FUNCTION
void setRingColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    ring.setPixelColor(i, ring.Color(r, g, b));
  }
  ring.show();
}

void loop() {
  // NFC SCANNER
  // DETECT CARD
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    
    if (isEqualArray(mfrc522.uid.uidByte, validKey1, 4)) {
      // IF it aight :3
      Serial.println("Acceso autorizado");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Acceso");
      lcd.setCursor(0,1);
      lcd.print("Autorizado");
      //led gren
      setRingColor(0, 150, 0);
      // spinin motor
      servo.write(SERVO_INICIAL);
      delay(10000);
      servo.write(SERVO_CERRAR);
    } else {
      // if IT BAD.
      Serial.println("Acceso denegado");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Acceso");
      lcd.setCursor(0,1);
      lcd.print("Denegado");
      // red
      setRingColor(150, 0, 0);
      delay(2000);
    }
    mfrc522.PICC_HaltA();
  }
}
