#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <driver/i2s.h>

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SDA_PIN 17
#define SCL_PIN 18

// ================= LED =================
int leds[] = {4,5,6,7};
int ledCount = 4;

// ================= I2S MIC =================
#define I2S_WS 9
#define I2S_SCK 10
#define I2S_SD 11
#define I2S_PORT I2S_NUM_0

#define bufferLen 64
int16_t sBuffer[bufferLen];

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  for(int i=0;i<ledCount;i++){
    pinMode(leds[i], OUTPUT);
  }

  // I2S config
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = bufferLen,
    .use_apll = false
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

// ================= LOOP =================
void loop() {

  // 🎤 Read sound
  size_t bytesIn = 0;
  i2s_read(I2S_PORT, &sBuffer, sizeof(sBuffer), &bytesIn, portMAX_DELAY);

  int samples = bytesIn / 2;
  long sum = 0;

  for(int i=0;i<samples;i++){
    sum += abs(sBuffer[i]);
  }

  int amplitude = sum / samples;

  // 🎚️ Map sound to LED count
  int level = map(amplitude, 0, 1500, 0, ledCount);

  // 💡 LED reaction
  for(int i=0;i<ledCount;i++){
    if(i < level){
      digitalWrite(leds[i], HIGH);
    } else {
      digitalWrite(leds[i], LOW);
    }
  }

  // 📺 OLED display (2 lines)
  display.clearDisplay();

  display.setTextColor(WHITE);

  // Line 1
  display.setTextSize(2);
  display.setCursor(30, 10);
  display.print("  Eid");

  // Line 2
  display.setTextSize(2);
  display.setCursor(5, 35);
  display.print("  Mubarak");

  // Optional: simple sound bar.
  int bar = map(amplitude, 0, 1500, 0, 120);
  display.fillRect(4, 60, bar, 3, WHITE);

  display.display();

  delay(10);
}