#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SDA_PIN 17
#define SCL_PIN 18

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Start I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Start display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display not found");
    while(true);
  }

  display.clearDisplay();

  // Test text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.print("HELLO");

  display.display();
}

void loop() {
}
