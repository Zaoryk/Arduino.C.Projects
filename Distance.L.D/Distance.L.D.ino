#include <Adafruit_NeoPixel.h>
#include <TM1637Display.h>

// === LED Ring Setup ===
#define LED_PIN 6
#define LED_COUNT 12
Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// === Ultrasonic Setup ===
#define TRIG_PIN 9
#define ECHO_PIN 10

// === TM1637 Setup ===
#define CLK 3
#define DIO 2
TM1637Display display(CLK, DIO);

void setup() {
  Serial.begin(9600);

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // LED ring
  ring.begin();
  ring.show();  // All off
  ring.setBrightness(40);

  // TM1637 Display
  display.setBrightness(0x0f); // max brightness
}

void loop() {
  // --- Measure Distance ---
  long duration;
  long distance;

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration/59; // distance in cm

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println();
  delay(500);

  // --- To show in Display ---
  if (distance > 9999) {
    display.showNumberDec(9999);
  } else {
    display.showNumberDec(distance, false, 4, 0);
  }

  // --- LED Distance  ---
  ring.clear();

  if (distance >= 0 && distance <= 10) {
    // Green
    for (int i = 0; i < LED_COUNT; i++) {
      ring.setPixelColor(i, ring.Color(0, 150, 0));
    }
  } 
  else if (distance > 10 && distance <= 20) {
    // Blue
    for (int i = 0; i < LED_COUNT; i++) {
      ring.setPixelColor(i, ring.Color(0, 0, 150));
    }
  } 
  else {
    // Red
    for (int i = 0; i < LED_COUNT; i++) {
      ring.setPixelColor(i, ring.Color(150, 0, 0));
    }
  }

  ring.show();
  delay(200);
}
