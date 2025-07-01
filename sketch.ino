#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4
#define BUTTON_PIN 27

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

bool waiting = false;
bool readyToPress = false;
unsigned long startTime;
unsigned long cueTime;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  tft.begin();
  tft.setRotation(1);  // ✅ Landscape طبيعي
  randomSeed(analogRead(0));
  startNewRound();
}

void startNewRound() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(30, 100);
  tft.println("Get Ready...");
  waiting = true;
  readyToPress = false;
  cueTime = millis() + random(2000, 6000);
}

void loop() {
  unsigned long now = millis();

  if (waiting && now >= cueTime) {
    tft.fillScreen(ILI9341_MAGENTA);
    delay(150);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(3);
    tft.setCursor(30, 100);
    tft.println("PRESS NOW!");
    startTime = millis();
    waiting = false;
    readyToPress = true;
  }

  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(10);
    if (digitalRead(BUTTON_PIN) == LOW) {
      if (readyToPress) {
        unsigned long reactionTime = millis() - startTime;
        tft.fillScreen(ILI9341_CYAN);
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(2);
        tft.setCursor(30, 100);
        tft.print("Reaction: ");
        tft.print(reactionTime);
        tft.println(" ms");
      } else {
        tft.fillScreen(ILI9341_RED);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.setCursor(30, 100);
        tft.println("Too Early!");
      }

      delay(2000);
      startNewRound();
    }
  }
}
