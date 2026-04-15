#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "ui.h"

// --- Configuration ---
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// NTP Server Settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;           // Update with your GMT offset in seconds
const int   daylightOffset_sec = 3600;   // Update to 3600 if observing DST, otherwise 0

// Open-Meteo API Settings (Free, NO API KEY required!)
// Go to https://open-meteo.com/ to find coordinates for your exact city
const String latitude = "51.5085";  // Default: London
const String longitude = "-0.1257"; // Default: London

// Hardware Configuration
static const uint16_t screenWidth  = 172; // Adjusted for your 1.47" screen
static const uint16_t screenHeight = 320; // Adjusted for your 1.47" screen

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); // Initialize TFT instance with correct dimensions

/* LVGL display flush callback */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}

// Global state variables
unsigned long lastTimeUpdate = 0;
unsigned long lastWeatherUpdate = 0;
const unsigned long weatherUpdateInterval = 10 * 60 * 1000; // 10 minutes

void setupWiFi() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected.");
    } else {
        Serial.println("\nWiFi connection failed.");
    }
}

void setupTime() {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        Serial.println("System time synchronized.");
    }
}

void updateTimeUI() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return;
    }

    char hourStr[3];
    char minStr[3];
    char secStr[3];
    char dateStr[20];

    // Format hour, minute, second
    sprintf(hourStr, "%02d", timeinfo.tm_hour);
    sprintf(minStr, "%02d", timeinfo.tm_min);
    sprintf(secStr, "%02d", timeinfo.tm_sec);
    
    // Format full date (e.g., "Mon, Oct 23")
    strftime(dateStr, sizeof(dateStr), "%a, %b %d", &timeinfo);

    // Update UI components (if they exist in SquareLine generated code)
    if(ui_timehour1) lv_label_set_text(ui_timehour1, hourStr);
    if(ui_timeminute) lv_label_set_text(ui_timeminute, minStr);
    if(ui_timesecond) lv_label_set_text(ui_timesecond, secStr);
    if(ui_date) lv_label_set_text(ui_date, dateStr);
}

void updateWeatherUI() {
    if (WiFi.status() != WL_CONNECTED) return;

    HTTPClient http;
    // Construct Open-Meteo URL to get current temp and today's min/max
    String serverPath = "http://api.open-meteo.com/v1/forecast?latitude=" + latitude + "&longitude=" + longitude + "&current_weather=true&daily=temperature_2m_max,temperature_2m_min&timezone=auto";
    
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
        String payload = http.getString();
        
        // Open-Meteo payloads can be a bit larger, use DynamicJsonDocument
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error) {
            // Parse Open-Meteo JSON structure
            float temp = doc["current_weather"]["temperature"];
            float tempMin = doc["daily"]["temperature_2m_min"][0];
            float tempMax = doc["daily"]["temperature_2m_max"][0];
            // Optional: int weathercode = doc["current_weather"]["weathercode"];
            
            char tempStr[10];
            char tempMinStr[15];
            char tempMaxStr[15];
            
            sprintf(tempStr, "%.0f°", temp);
            sprintf(tempMinStr, "Min %.0f°", tempMin);
            sprintf(tempMaxStr, "Max %.0f°", tempMax);
            
            if(ui_Temp) lv_label_set_text(ui_Temp, tempStr);
            if(ui_Tempmin) lv_label_set_text(ui_Tempmin, tempMinStr);
            if(ui_Tempmax) lv_label_set_text(ui_Tempmax, tempMaxStr);
            
            Serial.println("Weather updated successfully from Open-Meteo.");
        } else {
            Serial.print("JSON Error: ");
            Serial.println(error.c_str());
        }
    } else {
        Serial.print("Weather HTTP Error code: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}

void setup() {
    Serial.begin(115200);
    
    // 1. Init TFT Display
    tft.begin();
    tft.setRotation(0); // Adjust according to your smartwatch configuration
    tft.fillScreen(TFT_BLACK);
    
    // 2. Init LVGL
    lv_init();
    
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[screenWidth * 10];
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);
    
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    
    // 3. Init UI generated by SquareLine Studio
    ui_init();
    
    // 4. Connect to WiFi & Time Setup
    setupWiFi();
    setupTime();
    
    // 5. Initial Weather Fetch
    updateWeatherUI();
    lastWeatherUpdate = millis();
}

void loop() {
    lv_timer_handler();
    delay(5);
    
    unsigned long currentMillis = millis();

    // Reconnect to WiFi if connection is lost
    if (WiFi.status() != WL_CONNECTED && (currentMillis % 30000 == 0)) {
        Serial.println("WiFi connection lost. Reconnecting...");
        WiFi.disconnect();
        WiFi.begin(ssid, password);
    }
    
    // Update time every second
    // The ESP32 automatically uses its internal RTC to keep time ticking
    // even when WiFi is disconnected!
    if (currentMillis - lastTimeUpdate >= 1000) {
        updateTimeUI();
        lastTimeUpdate = currentMillis;
    }
    
    // Update weather every 10 minutes
    if (currentMillis - lastWeatherUpdate >= weatherUpdateInterval) {
        updateWeatherUI();
        lastWeatherUpdate = currentMillis;
    }
}
