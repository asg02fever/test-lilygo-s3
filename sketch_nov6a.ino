// LilyGo T-Display S3 AMOLED V2 Demo
// This demo shows basic display functionality and button handling

#include <Arduino.h>
#include "rm67162.h"
#include <TFT_eSPI.h>

// Pin Definitions
#define PIN_BUTTON_1          0   // Bottom button when USB on right
#define PIN_BUTTON_2          21  // Top button when USB on right
#define PIN_DISPLAY_POWER     38  // Controls display power on V2
#define DISPLAY_WIDTH         536
#define DISPLAY_HEIGHT        240

// Display settings
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

// Global variables
bool displayOn = true;
uint8_t currentDemo = 0;
const uint8_t totalDemos = 4;

// Colors
#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_CYAN      0x07FF
#define COLOR_MAGENTA   0xF81F
#define COLOR_YELLOW    0xFFE0

void setupDisplay() {
    // Create sprite with full screen size
    sprite.createSprite(DISPLAY_HEIGHT, DISPLAY_WIDTH);
    sprite.setSwapBytes(true);
    
    // Initialize the display
    rm67162_init();
    lcd_setRotation(1); // Rotate 90 degrees (USB on right)
    
    // Turn on display
    digitalWrite(PIN_DISPLAY_POWER, HIGH);
    lcd_brightness(200); // Set initial brightness (0-255)
}

void drawColorBoxes() {
    int boxWidth = DISPLAY_HEIGHT / 4;
    int boxHeight = DISPLAY_WIDTH / 2;
    
    uint16_t colors[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW,
                        COLOR_CYAN, COLOR_MAGENTA, COLOR_WHITE, COLOR_BLACK};
    
    sprite.fillSprite(COLOR_BLACK);
    
    for(int i = 0; i < 8; i++) {
        int x = (i % 4) * boxWidth;
        int y = (i / 4) * boxHeight;
        sprite.fillRect(x, y, boxWidth, boxHeight, colors[i]);
    }
    
    lcd_PushColors(0, 0, DISPLAY_HEIGHT, DISPLAY_WIDTH, (uint16_t *)sprite.getPointer());
}

void drawCircles() {
    sprite.fillSprite(COLOR_BLACK);
    
    for(int i = 0; i < 10; i++) {
        int x = random(DISPLAY_HEIGHT);
        int y = random(DISPLAY_WIDTH);
        int radius = random(10, 50);
        uint16_t color = random(0xFFFF);
        sprite.fillCircle(x, y, radius, color);
    }
    
    lcd_PushColors(0, 0, DISPLAY_HEIGHT, DISPLAY_WIDTH, (uint16_t *)sprite.getPointer());
}

void drawText() {
    sprite.fillSprite(COLOR_BLACK);
    sprite.setTextColor(COLOR_WHITE, COLOR_BLACK);
    
    // Draw different size text
    sprite.setTextSize(1);
    sprite.drawString("LilyGo T-Display", 10, 10);
    
    sprite.setTextSize(2);
    sprite.drawString("S3 AMOLED V2", 10, 30);
    
    sprite.setTextSize(3);
    sprite.drawString("Demo", 10, 60);
    
    sprite.setTextColor(COLOR_GREEN, COLOR_BLACK);
    sprite.setTextSize(1);
    sprite.drawString("Press top button to change demo", 10, 100);
    sprite.drawString("Press bottom button to toggle display", 10, 120);
    
    lcd_PushColors(0, 0, DISPLAY_HEIGHT, DISPLAY_WIDTH, (uint16_t *)sprite.getPointer());
}

void drawPattern() {
    sprite.fillSprite(COLOR_BLACK);
    
    for(int x = 0; x < DISPLAY_HEIGHT; x += 10) {
        for(int y = 0; y < DISPLAY_WIDTH; y += 10) {
            uint16_t color = ((x + y) % 20 == 0) ? COLOR_WHITE : COLOR_BLUE;
            sprite.fillRect(x, y, 8, 8, color);
        }
    }
    
    lcd_PushColors(0, 0, DISPLAY_HEIGHT, DISPLAY_WIDTH, (uint16_t *)sprite.getPointer());
}

void handleButtons() {
    // Top button - change demo
    if(digitalRead(PIN_BUTTON_2) == 0) {
        delay(50); // Simple debounce
        while(digitalRead(PIN_BUTTON_2) == 0); // Wait for release
        currentDemo = (currentDemo + 1) % totalDemos;
    }
    
    // Bottom button - toggle display
    if(digitalRead(PIN_BUTTON_1) == 0) {
        delay(50); // Simple debounce
        while(digitalRead(PIN_BUTTON_1) == 0); // Wait for release
        displayOn = !displayOn;
        digitalWrite(PIN_DISPLAY_POWER, displayOn);
    }
}

void updateDisplay() {
    if (!displayOn) return;
    
    switch(currentDemo) {
        case 0:
            drawColorBoxes();
            break;
        case 1:
            drawCircles();
            break;
        case 2:
            drawText();
            break;
        case 3:
            drawPattern();
            break;
    }
}

void setup() {
    Serial.begin(115200);
    
    // Setup buttons
    pinMode(PIN_BUTTON_1, INPUT_PULLUP);
    pinMode(PIN_BUTTON_2, INPUT_PULLUP);
    pinMode(PIN_DISPLAY_POWER, OUTPUT);
    
    // Initialize display
    setupDisplay();
    
    // Show initial demo
    updateDisplay();
}

void loop() {
    handleButtons();
    updateDisplay();
    delay(50);
}