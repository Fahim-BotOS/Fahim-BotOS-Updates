#include <HTTPUpdate.h>
#include <Arduino.h>

// ওটিএ লিঙ্ক
const char* firmware_url = "https://github.com/Fahim-BotOS/Fahim-BotOS-Updates/releases/latest/download/firmware.bin";

// --- ফাংশনগুলোকে আগে থেকে চিনিয়ে দেওয়া (Forward Declaration) ---
void handleSave();
void startCaptivePortal();
void fetchSmartWeather();
void handleNormal();
void handleDizzy();
void handleAngry();
void handleSleep();
void playTone(int frequency, int duration);
void playBootSound();
void playSadSound();
void playLoveSound();
void playAngrySound();
void playDizzySound();
void playSleepSound();
void playScaredSound();
void playMoveSound();
void drawMood(int mood);
void expressionScared();
void expressionDizzy();
void expressionSleepy();
void expressionSuperAngry();
void expressionVebchi();
void drawEye(int x, int y, int w, int h, int r);
void expressionNormal();
void expressionSad();
void expressionLove();
void showWeatherPage();
void showTimePage();
void drawLoading();
void drawAutoExpressions();
void expressionPhysicsBall();


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <ESPmDNS.h>

// --- ওলেড ডিসপ্লে কনফিগারেশন ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MPU6050 mpu;
WebServer server(80);
DNSServer dnsServer;
Preferences preferences;

// --- পিক্সেল রোবট লোগো (৬৪x৬৪ পিক্সেল) ---
const unsigned char PROGMEM pixel_logo [] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0f, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x43, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x63, 0xff, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x71, 0xff, 0x13, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x78, 0xfe, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7c, 0xfc, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc2, 0x7c, 0x78, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x86, 0x7e, 0x71, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0e, 0x7e, 0x71, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1e, 0x7e, 0x23, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3e, 0x7f, 0x07, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7e, 0x7f, 0x0f, 0xf2, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7c, 0x7f, 0x0f, 0xf2, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0x7f, 0x1f, 0xf3, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0x7e, 0x3f, 0xf3, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xf0, 0x3e, 0x3f, 0xf3, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xf2, 0x3c, 0x7f, 0xf3, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xe2, 0x3c, 0xff, 0xf3, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xe2, 0x38, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xe6, 0x39, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xe6, 0x31, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xe6, 0x33, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xe6, 0x23, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xe6, 0x27, 0xc0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xe2, 0x07, 0x80, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xf2, 0x0f, 0x0f, 0xff, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xf0, 0x0f, 0x1f, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf0, 0x1f, 0x3f, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf8, 0x1f, 0x3f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7c, 0x1f, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7e, 0x3f, 0x1f, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0x0f, 0x8f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x81, 0x83, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xe0, 0x03, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xf8, 0x0f, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// --- নেটওয়ার্ক ও এপিআই সেটিংস ---
const byte DNS_PORT = 53;
#define OWKEY   "4692129c8c2ad099884d033e0066701f"
const char* apSSID = "Pixel_Robot_Setup";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 21600); // GMT+6

// --- হার্ডওয়্যার পিন কনফিগারেশন ---
const int TOUCH_PIN = 2;
const int BUZZER_PIN = 3;

// --- গ্লোবাল ভ্যারিয়েবল ---
String stSSID = "";
String stPass = "";
int currentPage = 0;
String currentLocation = "Unknown";
String weatherTemp = "--";
String weatherDesc = "Clear";
String humidity = "--";
String windSpeed = "--";

unsigned long lastWeatherUpdate = 0;
unsigned long lastPageChange = 0;

int pupilX = 0;
int pupilY = 0;
unsigned long lastLookTime = 0;
unsigned long lastBlinkTime = 0;
bool isBlinking = false;

unsigned long touchStartTime = 0;
bool isTouching = false;
unsigned long lastSleepCheck = 0;
unsigned long moodEndTime = 0;
int currentMood = 0;
bool isMoving = false;
int touchCount = 0; // টাচ গণনার জন্য
unsigned long lastTouchTime = 0; // কতক্ষণ আগে টাচ করা হয়েছে

// ==========================================
// --- ১. কাস্টম ক্যাপটিভ পোর্টাল হ্যান্ডলার ---
// ==========================================

String getSetupHTML() {
    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; background: #1a1a2e; color: white; padding: 30px; }";
    html += ".box { background: #16213e; padding: 25px; border-radius: 15px; box-shadow: 0 4px 15px rgba(0,0,0,0.3); }";
    html += "input { width: 90%; padding: 12px; margin: 10px 0; border: none; border-radius: 8px; font-size: 16px; }";
    html += "button { background: #e94560; color: white; padding: 15px; width: 95%; border: none; border-radius: 8px; font-size: 18px; cursor: pointer; transition: 0.3s; }";
    html += "button:hover { background: #ff4d6d; } h2 { margin-bottom: 20px; }</style></head><body>";
    html += "<div class='box'><h2>PIXEL ROBOT</h2><p>Connect to your Home WiFi</p>";
    html += "<form action='/save' method='POST'>";
    html += "<input type='text' name='ssid' placeholder='WiFi Name (SSID)' required><br>";
    html += "<input type='password' name='pass' placeholder='Password' required><br>";
    html += "<button type='submit'>SAVE & CONNECT</button></form></div></body></html>";
    return html;
}

void handleSave() {
    stSSID = server.arg("ssid");
    stPass = server.arg("pass");

    // আগে সেভ করুন
    preferences.begin("wifi-data", false);
    preferences.putString("ssid", stSSID);
    preferences.putString("pass", stPass);
    preferences.end();

    // তারপর response পাঠান
    server.send(200, "text/html", "<h2>Setup Saved!</h2><p>Pixel is restarting...</p>");

    delay(1000);
    ESP.restart();
}


void startCaptivePortal() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apSSID); // পাসওয়ার্ড ছাড়া ওপেন নেটওয়ার্ক
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

    server.on("/", []() {
        server.send(200, "text/html", getSetupHTML());
    });
    server.on("/save", HTTP_POST, handleSave);
    server.onNotFound([]() {
        server.send(200, "text/html", getSetupHTML());
    });
    server.begin();

    while(true) {
        dnsServer.processNextRequest();
        server.handleClient();

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(25, 10);
        display.print("SETUP MODE");
        display.drawLine(0, 20, 128, 20, WHITE);
        display.setCursor(5, 35);
        display.print("Connect WiFi: ");
        display.setCursor(5, 50);
        display.print(apSSID); // Pixel_Robot_Setup দেখাবে
        display.display();
        delay(10);
    }
}



// ==========================================
// --- ২. স্মার্ট লোকেশন ও আবহাওয়া ---
// ==========================================

void fetchSmartWeather() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://ip-api.com/json/");
        if (http.GET() > 0) {
            StaticJsonDocument<512> locDoc;
            deserializeJson(locDoc, http.getString());
            currentLocation = locDoc["city"].as<String>();
        }
        http.end();

        String url = "http://api.openweathermap.org/data/2.5/weather?q=" + currentLocation + "&appid=" + String(OWKEY) + "&units=metric";
        http.begin(url);
        if (http.GET() > 0) {
            StaticJsonDocument<1024> doc;
            deserializeJson(doc, http.getString());
            weatherTemp = String((int)doc["main"]["temp"]);
            humidity = String((int)doc["main"]["humidity"]);
            windSpeed = String((float)doc["wind"]["speed"]);
            weatherDesc = doc["weather"][0]["main"].as<String>();
        }
        http.end();
    }
}

// --- অ্যাপ কন্ট্রোল হ্যান্ডলার ---

void handleNormal() {
    currentMood = 0;
    moodEndTime = 0;
    server.send(200, "text/plain", "Normal Mode Active");
}

void handleDizzy() {
    currentMood = 4;
    moodEndTime = millis() + 5000;
    playDizzySound();
    server.send(200, "text/plain", "Dizzy Mode Active");
}

void handleAngry() {
    currentMood = 7;
    moodEndTime = millis() + 5000;
    playAngrySound();
    server.send(200, "text/plain", "Angry Mode Active");
}

void handleSleep() {
    currentMood = 5;
    lastSleepCheck = millis();
    playSleepSound();
    server.send(200, "text/plain", "Sleep Mode Active");
}

// ==========================================
// --- ৩. সেটআপ ফাংশন ---
// ==========================================

void setup() {
    Serial.begin(115200);
    pinMode(TOUCH_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        for(;;);
    }
    mpu.begin();
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    display.clearDisplay();
    display.setTextColor(WHITE);

    // --- বুট লোগো প্রদর্শন শুরু ---
    display.clearDisplay();

    // লোগোটি পুরো স্ক্রিন জুড়ে (০, ০) পজিশন থেকে দেখাবে
    display.drawBitmap(0, 0, pixel_logo, 128, 64, WHITE);
    display.display();

    playBootSound(); // লোগো আসার সময় শব্দ হবে
    delay(3000);     // ৩ সেকেন্ড লোগোটি স্থায়ী হবে
    // --- বুট লোগো প্রদর্শন শেষ ---


    playBootSound(); // লোগো আসার সময় শব্দ হবে
    delay(3000);     // ৩ সেকেন্ড লোগোটি স্থায়ী হবে

    // ওয়াইফাই ডেটা রিট্রাইভ করা
    // ওয়াইফাই ডেটা রিট্রাইভ করা
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(500);

    preferences.begin("wifi-data", true);
    stSSID = preferences.getString("ssid", "");
    stPass = preferences.getString("pass", "");
    preferences.end();

    Serial.println("SSID: [" + stSSID + "]");
    Serial.println("PASS: [" + stPass + "]");

    if (stSSID == "") {
        startCaptivePortal();
    } else {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("SSID:");
        display.setCursor(0, 15);
        display.print(stSSID);
        display.display();
        delay(2000);

        WiFi.mode(WIFI_STA);
        delay(200);
        WiFi.begin(stSSID.c_str(), stPass.c_str());

        int retry = 0;
        while (WiFi.status() != WL_CONNECTED && retry < 40) {
            delay(500);
            drawLoading();
            retry++;
        }

        if (WiFi.status() != WL_CONNECTED) {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("WIFI FAILED!");
            display.setCursor(0, 15);
            display.print(stSSID);
            display.setCursor(0, 30);
            display.print(stPass);
            display.display();
            delay(3000);
            preferences.begin("wifi-data", false);
            preferences.clear();
            preferences.end();
            startCaptivePortal();
        } else {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("CONNECTED!");
            display.display();
            delay(2000);
        }
    }


    if (MDNS.begin("pixel")) { // এখানে "pixel" হলো আপনার রোবটের নাম
        Serial.println("MDNS responder started");
        // ব্রাউজারে এখন আপনি http://pixel.local লিখে ঢুকতে পারবেন
    }

    server.on("/normal", handleNormal);
    server.on("/dizzy", handleDizzy);
    server.on("/angry", handleAngry);
    server.on("/sleep", handleSleep);
    server.begin();

    timeClient.begin();
    fetchSmartWeather();
    lastSleepCheck = millis();
    playBootSound();
}

// ==========================================
// --- ৪. মেইন লুপ ---
// ==========================================

void loop() {
    server.handleClient();
    unsigned long currentMillis = millis();
    timeClient.update();

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // --- ১. টাচ সেন্সর লজিক ---
    // --- উন্নত টাচ লজিক ---
    int touchState = digitalRead(TOUCH_PIN);

    if (touchState == HIGH) {
        if (!isTouching) {
            touchStartTime = millis(); // টাচ শুরু হওয়ার সময়
            isTouching = true;
            touchCount++;
            lastTouchTime = millis();
        }

        unsigned long duration = millis() - touchStartTime;

        // শর্ত ১: একটানা ৩ সেকেন্ডের বেশি হাত বুলালে (Relax Mode)
        if (duration > 3000 && duration < 10000) {
            currentMood = 10; // আমরা ১০ নম্বর মুড হিসেবে Relax সেট করছি
            moodEndTime = millis() + 500; // হাত বুলানো অবস্থায় মুড থাকবে
        }
        // শর্ত ২: একটানা ১০ সেকেন্ড টাচ করে রাখলে (Sleep Mode)
        else if (duration >= 10000) {
            currentMood = 5; // Sleep Mode
            moodEndTime = millis() + 10000;
        }
    } else {
        if (isTouching) {
            unsigned long finalDuration = millis() - touchStartTime;

            // শর্ত ৩: খুব হালকা টাচ (১০০ ms এর কম) করলে Sad মুড
            if (finalDuration < 100) {
                currentMood = 1; // Sad Mode
                moodEndTime = millis() + 2000;
            }
            isTouching = false;
        }
    }

// ৩ বার দ্রুত টাচ করলে ভেংচি (আগের লজিক)
    if (touchCount == 3 && (millis() - lastTouchTime < 1500)) {
        currentMood = 9;
        moodEndTime = millis() + 4000;
        touchCount = 0;
    }

// --- ২. মুভমেন্ট ও তালি শনাক্তকরণ লজিক ---
    //detectGesture();


    // তালি (Clap) বা ইমপ্যাক্ট শনাক্তকরণ
    float zDifference = abs(a.acceleration.z - 9.81);
    if (zDifference > 3.8) {
        if (currentMood == 5) {
            currentMood = 0;
            playTone(1200, 100);
            lastSleepCheck = currentMillis;
        }
        else {
            currentMood = 8;
            playScaredSound();
            moodEndTime = currentMillis + 3500;
            lastSleepCheck = currentMillis;
        }
    }

    if (abs(g.gyro.x) > 0.5 || abs(g.gyro.y) > 0.5 || abs(g.gyro.z) > 0.5) {
        if (!isMoving) playMoveSound();
        isMoving = true;
        lastLookTime = 0;
        lastSleepCheck = currentMillis;
    } else {
        isMoving = false;
    }

    // --- ৩. ঝাকুনি বা শেক ডিটেকশন লজিক ---
    // নতুন করে sensors_event_t লিখার দরকার নেই, শুধু ডাটা আপডেট করলেই হবে
    mpu.getEvent(&a, &g, &temp); 

    // X, Y, Z অক্ষের সম্মিলিত শক্তি বা G-Force মাপা
    float shakeForce = sqrt(a.acceleration.x * a.acceleration.x + 
                            a.acceleration.y * a.acceleration.y + 
                            a.acceleration.z * a.acceleration.z);

    // যদি রোবটকে যথেষ্ট জোরে ঝাকানো হয় (১৮.০ এর উপরে)
    if (shakeForce > 18.0) {    
        if (currentMood != 11) { // যদি অলরেডি এই মুডে না থাকে
            currentMood = 11;    
            moodEndTime = millis() + 5000; // ৫ সেকেন্ডের জন্য এনিমেশন চলবে
        }
    }



    // --- ৪. পেজ ও মুড টাইমিং ---
    if (currentMood == 0 || currentMillis > moodEndTime) {
        if (currentMood != 0 && currentMood != 5) currentMood = 0;
        if (!isMoving && !isTouching) {
            unsigned long timeInPage = currentMillis - lastPageChange;
            if (currentPage == 0 && timeInPage > 6000) {
                currentPage = 2;
                lastPageChange = currentMillis;
            }
            else if (currentPage == 2 && timeInPage > 3000) {
                currentPage = 1;
                lastPageChange = currentMillis;
            }
            else if (currentPage == 1 && timeInPage > 6000) {
                currentPage = 0;
                lastPageChange = currentMillis;
            }
        } else {
            currentPage = 0;
            lastPageChange = currentMillis;
        }
    }

    // --- ৫. স্লিপ ও আপডেট লজিক ---
    if (currentMillis - lastSleepCheck > 600000) {
        if (currentMood != 5) playSleepSound();
        currentMood = 5;
    }
    if (currentMillis - lastWeatherUpdate > 600000) {
        fetchSmartWeather();
        lastWeatherUpdate = currentMillis;
    }

    if (currentMillis - lastBlinkTime > random(3000, 6000)) {
        isBlinking = true;
        lastBlinkTime = currentMillis;
    }
    if (isBlinking && currentMillis - lastBlinkTime > 150) isBlinking = false;

    // --- ৬. রেন্ডারিং ---
    display.clearDisplay();
    if (currentMillis < moodEndTime || currentMood == 5) {
        drawMood(currentMood);
    }
    else {
        if (currentPage == 0) drawAutoExpressions();
        else if (currentPage == 1) showWeatherPage();
        else if (currentPage == 2) showTimePage();
    }
    display.display();
    delay(30);
}

// --- সাউন্ড ও গ্রাফিক্স ফাংশনসমূহ (বিস্তারিত) ---

void playTone(int frequency, int duration) {
    long delayValue = 1000000 / frequency / 2;
    long numCycles = frequency * duration / 1000;
    for (long i = 0; i < numCycles; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(delayValue);
        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(delayValue);
    }
}

void playBootSound() {
    playTone(1000, 100);
    delay(50);
    playTone(1500, 200);
}
void playSadSound() {
    for (int hz = 600; hz > 200; hz -= 10) playTone(hz, 25);
    delay(100);
    playTone(180, 600);
}
void playLoveSound() {
    playTone(1200, 150);
    delay(50);
    playTone(1500, 150);
    delay(50);
    playTone(2000, 300);
}
void playAngrySound() {
    for(int i=0; i<5; i++) {
        playTone(200, 100);
        delay(50);
    }
}
void playDizzySound() {
    for (int hz = 400; hz < 1200; hz += 40) playTone(hz, 10);
    for (int hz = 1200; hz > 400; hz -= 40) playTone(hz, 10);
}
void playSleepSound() {
    for (int hz = 150; hz < 350; hz += 10) playTone(hz, 25);
    delay(100);
    for (int hz = 300; hz > 120; hz -= 8) playTone(hz, 40);
}
void playScaredSound() {
    for(int i=0; i<8; i++) {
        playTone(random(800, 1800), 40);
        delay(20);
    }
}
void playMoveSound() {
    for (int hz = 1200; hz < 1800; hz += 100) playTone(hz, 5);
    for (int hz = 1800; hz > 1200; hz -= 100) playTone(hz, 5);
}

void drawMood(int mood) {
    switch(mood) {
    case 1:
        expressionSad();
        break;
    case 2:
        expressionLove();
        break;
    case 4:
        expressionDizzy();
        break;
    case 5:
        expressionSleepy();
        break;
    case 7:
        expressionSuperAngry();
        break;
    case 8:
        expressionScared();
        break;
    case 9:
        expressionVebchi();
        break;
    case 10:
        expressionRelax();
        break;
    case 11:
    expressionPhysicsBall();
    break;
    }
}



void expressionScared() {
    display.clearDisplay(); // স্ক্রিন ক্লিয়ার করবে

    // ১. কাঁপুনির এনিমেশন (Shaking/Wiggle Animation Logic)
    // wiggleX এবং wiggleY প্রতি লুপে দ্রুত পজিশন বদলাবে
    int wiggleX = (millis() % 80 > 40) ? 2 : -2; 
    int wiggleY = (millis() % 60 > 30) ? 1 : -1;

    // ২. প্রফেশনাল ভীত চোখ (আকার বড় করা হয়েছে)
    // চোখগুলো স্ক্রিনের ডানদিকের উপরের কোণায় থাকবে
    int eyeSize = 25;            // ভয় পাওয়ার কারণে চোখ বড় (আগে ১০ ছিল)
    int cornerX = 75 + wiggleX;  // চোখের আকার বড় হওয়ায় পজিশন অ্যাডজাস্ট করা হয়েছে (আগে ১০০ ছিল)
    int cornerY = 10 + wiggleY;  // মূল পজিশন ১০, সাথে কাঁপুনির অফসেট

    // বাম চোখ (কোণায়)
    display.fillRoundRect(cornerX, cornerY, eyeSize, eyeSize, 5, WHITE); // কর্নার রেডিয়াস ৫ করা হয়েছে
    display.fillCircle(cornerX + (eyeSize/2), cornerY + (eyeSize/2), 4, BLACK); // মণি (ভরাট)

    // ডান চোখ (কোণায় এবং বাম চোখের পাশেই)
    int eyeGap = 5; // দুই চোখের মাঝের দূরত্ব
    display.fillRoundRect(cornerX + eyeSize + eyeGap, cornerY, eyeSize, eyeSize, 5, WHITE);
    display.fillCircle(cornerX + eyeSize + eyeGap + (eyeSize/2), cornerY + (eyeSize/2), 4, BLACK);

    // ৩. আতঙ্কের শব্দ (Panic Buzzer Sound - ২ নম্বর পিন)
    static unsigned long lastPanicSound = 0;
    if (millis() - lastPanicSound > 80) { // প্রতি ৮০ মিলি-সেকেন্ড অন্তর
        playTone(random(600, 1500), 20); 
        lastPanicSound = millis();
    }

    // ৪. কিউট ঘামের ফোঁটা (Sweat Drop Animation)
    static float sweatY = 10;
    if ((millis() / 700) % 2 == 0) {
        display.fillCircle(cornerX - 8, (int)sweatY, 3, WHITE); // ঘামের ফোঁটা একটু বড় করা হয়েছে
        display.drawLine(cornerX - 8, (int)sweatY - 4, cornerX - 8, (int)sweatY - 8, WHITE);
        sweatY += 2.0; // ঘাম দ্রুত নিচে নামবে
        if (sweatY > 55) sweatY = 10; 
    }

    // ৫. "SCARED!" টেক্সট মুছে ফেলা হয়েছে (এই অংশটি খালি)

    display.display();
}



void expressionDizzy() {
    display.clearDisplay();

    // ১. কিউট ঘোরানো চোখ (Spiral Eyes)
    float angle = millis() * 0.008; // ঘোরার গতি একটু কমিয়ে স্মুথ করা হয়েছে

    for (int i = 0; i < 2; i++) {
        int centerX = (i == 0) ? 40 : 88; // দুই চোখের সেন্টার

        // চোখের বাইরের মণি (আগের চেয়ে কিউট শেপ)
        display.drawRoundRect(centerX - 15, 20, 30, 30, 10, WHITE);

        // ভেতরের স্পাইরাল এনিমেশন
        for (int r = 3; r < 12; r += 3) {
            int xOffset = (r-1) * cos(angle + r);
            int yOffset = (r-1) * sin(angle + r);
            display.drawPixel(centerX + xOffset, 35 + yOffset, WHITE);
            display.drawCircle(centerX + xOffset, 35 + yOffset, 1, WHITE);
        }
    }

    // ২. মাথার ওপর ঘোরানো তারা (Dizzy Stars) - এটি প্রফেশনাল লুক দেবে
    int starX = 64 + 20 * cos(millis() * 0.005);
    int starY = 10 + 5 * sin(millis() * 0.005);
    display.setCursor(starX, starY);
    display.print("*");
    display.setCursor(128 - starX, starY + 2);
    display.print(".");

    // ৩. মুখ (ছোট্ট গোল হাঁ করা মুখ - যা দেখতে কিউট)
    display.drawCircle(64, 56, 4, WHITE);

    display.display();
}


void expressionSleepy() {
    display.clearDisplay(); // স্ক্রিন একদম পরিষ্কার রাখবে, কোনো X থাকবে না

    // ১. বন্ধ চোখ (নিচের দিকে শান্তভাবে বন্ধ)
    display.fillRoundRect(25, 40, 30, 6, 3, WHITE);
    display.fillRoundRect(73, 40, 30, 6, 3, WHITE);

    // ২. "Z Z Z" এনিমেশন লজিক
    int zPos = (millis() / 800) % 3;
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    if(zPos >= 0) { display.setCursor(100, 20); display.print("z"); }
    if(zPos >= 1) { display.setCursor(110, 12); display.print("Z"); }
    if(zPos >= 2) { display.setCursor(120, 4);  display.print("z"); }

    // ৩. বাস্তবসম্মত স্নোরিং ইফেক্ট (Gap সহ)
    // লুপটি এখন ৪ সেকেন্ডের (৪০০০ মিলি-সেকেন্ড)
    unsigned long sleepMillis = millis() % 4000; 
    
    if (sleepMillis < 1200) { 
        // ১.২ সেকেন্ড ধরে নাক ডাকার শব্দ (ইনহেল/নাক ডাকা)
        int freq = 150 + (sleepMillis / 12); 
        tone(3, freq, 30); 
    } 
    else if (sleepMillis >= 1200 && sleepMillis < 2200) {
        // ১ সেকেন্ড বিরতি (শ্বাস ছাড়ার সময় বা গ্যাপ)
        noTone(3);
    }
    else if (sleepMillis >= 2200 && sleepMillis < 3000) {
        // পরবর্তী ছোট একটি দীর্ঘশ্বাস (এক্সহেল)
        int freq = 200 - ((sleepMillis - 2200) / 15);
        if (freq > 50) tone(3, freq, 20);
    }
    else {
        // লুপের বাকি সময় একদম শান্ত
        noTone(3);
    }

    // ৪. ছোট্ট কিউট হাসি (মাঝখানে কোনো X থাকবে না)
    display.drawCircleHelper(64, 48, 5, 2, WHITE);
    display.drawCircleHelper(70, 48, 5, 1, WHITE);

    display.display();
}


void drawEye(int x, int y, int w, int h, int r) {
    if (isBlinking) {
        display.fillRect(x, y + (h/2), w, 4, WHITE);
    }
    else {
        display.fillRoundRect(x, y, w, h, r, WHITE);
        display.fillCircle(x + (w/2) + pupilX, y + (h/2) + pupilY, 4, BLACK);
    }
}

void expressionNormal() {
    if (millis() - lastLookTime > (isMoving ? 150 : random(2000, 5000))) {
        pupilX = random(-7, 8);
        pupilY = random(-6, 7);
        lastLookTime = millis();
    }
    drawEye(25, 20, 30, 30, 8);
    drawEye(73, 20, 30, 30, 8);
}

void expressionSuperAngry() {
    display.clearDisplay();

    // ১. রাগের কারণে পুরো স্ক্রিন বা চোখ কাঁপানো (Shake effect)
    int shake = (millis() % 50 > 25) ? 2 : -2;

    // ২. ভয়ংকর রাগী ভ্রু (V-Shape)
    display.drawLine(20 + shake, 15, 50 + shake, 30, WHITE); // বাম ভ্রু
    display.drawLine(108 + shake, 15, 78 + shake, 30, WHITE); // ডান ভ্রু

    // ৩. রাগী চোখ (তীর্যক বা নিচের দিকে ভরাট)
    // বাম চোখ
    display.fillRoundRect(25 + shake, 32, 30, 15, 4, WHITE);
    display.fillCircle(40 + shake, 40, 4, BLACK); // মণি

    // ডান চোখ
    display.fillRoundRect(73 + shake, 32, 30, 15, 4, WHITE);
    display.fillCircle(88 + shake, 40, 4, BLACK);

    // ৪. রাগী মুখ (নিচের দিকে বাঁকানো বড় মুখ)
    display.drawCircleHelper(64 + shake, 65, 15, 1, WHITE);
    display.drawCircleHelper(64 + shake, 65, 15, 2, WHITE);
    
    // ৫. ৩ নম্বর পিনে রাগী সাইরেন বা কর্কশ শব্দ
    if (millis() % 200 < 100) {
        tone(3, random(100, 300), 50); // লো-ফ্রিকোয়েন্সি কর্কশ শব্দ
    } else {
        noTone(3);
    }

    display.display();
}


void expressionSad() {
    display.clearDisplay();

    // ১. কিউট স্যাড ভ্রু (অল্প বাঁকানো, ভয়ংকর নয়)
    display.drawCircleHelper(40, 20, 12, 1, WHITE); // বাম ভ্রু
    display.drawCircleHelper(88, 20, 12, 2, WHITE); // ডান ভ্রু

    // ২. প্রফেশনাল কিউট চোখ (একটু লম্বাটে এবং ভরাট)
    // বাম চোখ
    display.fillRoundRect(25, 25, 30, 28, 10, WHITE); 
    // ডান চোখ
    display.fillRoundRect(73, 25, 30, 28, 10, WHITE); 

    // ৩. বড় মণি (নিচের দিকে তাকিয়ে থাকা, কিন্তু কিউট)
    int eyeLookY = 40 + (int)(sin(millis() * 0.002) * 2); // হালকা নড়াচড়া করবে
    display.fillCircle(40, eyeLookY, 6, BLACK);
    display.fillCircle(88, eyeLookY, 6, BLACK);
    
    // মণির ভেতরে ছোট সাদা বিন্দু (Sparkle) - এটি কিউটনেস বহুগুণ বাড়িয়ে দেয়
    display.fillCircle(42, eyeLookY - 2, 2, WHITE);
    display.fillCircle(90, eyeLookY - 2, 2, WHITE);

    // ৪. প্রফেশনাল চোখের জল (Tear Drop Animation)
    static float tearY = 40;
    static bool tearLeft = true; 
    
    if (tearLeft) {
        display.fillCircle(30, (int)tearY, 2, WHITE); // বাম চোখের জল
        display.fillTriangle(28, (int)tearY, 32, (int)tearY, 30, (int)tearY - 4, WHITE);
    } else {
        display.fillCircle(98, (int)tearY, 2, WHITE); // ডান চোখের জল
        display.fillTriangle(96, (int)tearY, 100, (int)tearY, 98, (int)tearY - 4, WHITE);
    }

    tearY += 0.5; // খুব স্মুথলি জল পড়বে
    if (tearY > 58) {
        tearY = 40;
        tearLeft = !tearLeft; // একবার বাম চোখে, একবার ডান চোখে জল পড়বে
    }

    // ৫. ছোট কিউট মুখ (উল্টো বাঁকানো)
    display.drawCircleHelper(64, 60, 6, 1, WHITE);
    display.drawCircleHelper(64, 60, 6, 2, WHITE);

    // ৬. ৩ নম্বর পিনে হালকা কান্নার শব্দ (Whimpering Sound)
    if ((int)tearY % 10 == 0) {
        tone(3, 400 + (int)(tearY * 2), 10);
    } else {
        noTone(3);
    }

    display.display();
}


void expressionLove() {
    display.clearDisplay();

    // ১. হার্টবিট বা পালসিং লজিক (Smooth Sine Wave)
    float pulse = sin(millis() * 0.006) * 2.5;
    int s = (int)pulse;

    // ২. প্রফেশনাল চোখ (বড় এবং কিউট)
    // বাম চোখ
    display.fillRoundRect(25, 20, 32, 32, 10, WHITE);
    // ডান চোখ
    display.fillRoundRect(71, 20, 32, 32, 10, WHITE);

    // ৩. চোখের মণি হিসেবে হার্ট (কালো রঙের হার্ট চোখের ভেতর)
    // বাম চোখের হার্ট
    int lx = 41, ly = 32; // বাম মণির সেন্টার
    display.fillCircle(lx - 4, ly - 2, 4 + s/2, BLACK);
    display.fillCircle(lx + 4, ly - 2, 4 + s/2, BLACK);
    display.fillTriangle(lx - 8 - s/2, ly, lx + 8 + s/2, ly, lx, ly + 10 + s, BLACK);

    // ডান চোখের হার্ট
    int rx = 87, ry = 32; // ডান মণির সেন্টার
    display.fillCircle(rx - 4, ry - 2, 4 + s/2, BLACK);
    display.fillCircle(rx + 4, ry - 2, 4 + s/2, BLACK);
    display.fillTriangle(rx - 8 - s/2, ry, rx + 8 + s/2, ry, rx, ry + 10 + s, BLACK);

    // ৪. কিউট ব্লাশ (গালের লাল আভা বা ডট এনিমেশন)
    if ((millis() / 500) % 2 == 0) {
        display.drawPixel(20, 45, WHITE); display.drawPixel(22, 47, WHITE);
        display.drawPixel(106, 45, WHITE); display.drawPixel(104, 47, WHITE);
    }

    // ৫. প্রফেশনাল কিউট মুখ (স্মাইল)
    display.drawCircleHelper(64, 45, 7, 2, WHITE);
    display.drawCircleHelper(70, 45, 7, 1, WHITE);

    // ৬. ৩ নম্বর পিনে কিউট "লিটল হার্ট" সাউন্ড (পপিং সাউন্ড)
    if (abs(s) > 2) { 
        tone(3, 1200 + (s * 20), 10); // হার্ট যখন বড় হবে তখন শব্দ হবে
    } else {
        noTone(3);
    }

    display.display();
}


void showWeatherPage() {
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(currentLocation);
    display.drawLine(0, 10, 128, 10, WHITE);
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(weatherTemp + " C ");
    display.setTextSize(1);
    display.print(weatherDesc);
    display.drawLine(0, 42, 128, 42, WHITE);
    display.setCursor(0, 48);
    display.print("Hum: " + humidity + "%");
    display.setCursor(65, 48);
    display.print("Wind: " + windSpeed + "m/s");
}

void showTimePage() {
    display.clearDisplay(); 

    // ১. সময় দেখানো (HH:MM)
    display.setTextSize(3);
    display.setCursor(20, 15);
    display.print(timeClient.getFormattedTime().substring(0, 5));

    // ২. বর্তমান বার (Day of the week) বের করা
    String days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    String currentDay = days[timeClient.getDay()]; 

    // ৩. তারিখ এবং মাস বের করার বিকল্প পদ্ধতি (Epoch Time থেকে)
    time_t rawtime = timeClient.getEpochTime();
    struct tm * ti;
    ti = localtime (&rawtime);

    // তারিখ এবং মাসের ভ্যারিয়েবল
    int day = ti->tm_mday;
    int month = ti->tm_mon + 1; // tm_mon ০ থেকে শুরু হয় (জানুয়ারি = ০)

    // ৪. ডিসপ্লেতে বার এবং তারিখ/মাস দেখানো
    display.setTextSize(1);
    display.drawLine(10, 42, 118, 42, WHITE); // স্টাইলিশ ডিভাইডার

    // বার (বামে)
    display.setCursor(15, 50);
    display.print(currentDay); 
    
    // তারিখ/মাস (ডানে)
    display.setCursor(80, 50);
    if(day < 10) display.print("0"); // ০ যোগ করা (সুন্দর দেখানোর জন্য)
    display.print(day);
    display.print("/");
    if(month < 10) display.print("0");
    display.print(month);

    display.display();
}


void drawLoading() {
    display.clearDisplay();
    display.setCursor(20, 30);
    display.print("Connecting WiFi...");
    display.display();
}

void drawAutoExpressions() {
    expressionNormal();
}

// এটি কোডের একদম শেষে অন্য ফ���ংশনগুলোর সাথে থাকবে
void detectGesture() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    float force = sqrt(a.acceleration.x * a.acceleration.x +
                       a.acceleration.y * a.acceleration.y +
                       a.acceleration.z * a.acceleration.z);

    if (force > 25.0) {
        currentMood = 7;
        playAngrySound();
        moodEndTime = millis() + 4000;
    }
    else if (force > 11.5 && force < 14.5) {
        static unsigned long moveStart = 0;
        if (moveStart == 0) moveStart = millis();
        if (millis() - moveStart > 2000) {
            currentMood = 2;
            playLoveSound();
            moodEndTime = millis() + 4000;
            moveStart = 0;
        }
    }
}

void expressionVebchi() {
    display.clearDisplay();

    // ১. কান/এন্টেনা (চঞ্চল লুক)
    display.fillTriangle(20, 10, 35, 20, 10, 25, WHITE); 
    display.fillTriangle(108, 10, 93, 20, 118, 25, WHITE); 

    // ২. চোখ ও ভ্রু
    display.fillRoundRect(25, 25, 30, 20, 5, WHITE); 
    display.fillRoundRect(73, 25, 30, 20, 5, WHITE); 
    display.fillCircle(40, 35, 4, BLACK);
    display.fillCircle(88, 35, 4, BLACK);
    display.drawLine(25, 18, 45, 25, WHITE); 
    display.drawLine(103, 18, 83, 25, WHITE); 

    // ৩. জিবের এনিমেশন ও সাউন্ড (৩ নম্বর পিন)
    float wave = sin(millis() * 0.02);
    int tongueH = 15 + (int)(abs(wave) * 5); 
    display.fillRoundRect(56, 52, 16, tongueH, 8, WHITE); 
    display.drawLine(64, 54, 64, 50 + tongueH, BLACK);

    // বাজার সাউন্ড (জিব নড়লে শব্দ হবে)
    if (abs(wave) > 0.3) {
        tone(3, 1200 + (int)(abs(wave) * 500), 20);
    } else {
        noTone(3);
    }

    display.display();
}

void expressionRelax() {
    display.clearDisplay();

    // ১. প্রফেশনাল রিলাক্স চোখ (অর্ধেক বন্ধ এবং কিউট শেপ)
    // বাম চোখ
    display.fillRoundRect(25, 30, 30, 15, 8, WHITE); // চোখের ওপরের অংশ
    display.fillCircle(40, 45, 4, BLACK);           // নিচের দিকে মণি

    // ডান চোখ
    display.fillRoundRect(73, 30, 30, 15, 8, WHITE);
    display.fillCircle(88, 45, 4, BLACK);

    // ২. প্রশান্তির ভ্রু (একটু উপরে উঠানো)
    display.drawCircleHelper(40, 25, 10, 1, WHITE); 
    display.drawCircleHelper(88, 25, 10, 2, WHITE); 

    // ৩. শ্বাস নেওয়ার মতো এনিমেশন ( Breathing / Pulsing )
    // এটি করলে মনে হবে রোবটটি জ্যান্ত এবং নিঃশ্বাস নিচ্ছে
    int breatheShift = (int)(sin(millis() * 0.003) * 3);
    
    // ৪. ছোট্ট কিউট হাসিমুখ (যা নিঃশ্বাসের সাথে হালকা নড়বে)
    display.drawCircleHelper(64, 52 + breatheShift, 6, 2, WHITE); 
    display.drawCircleHelper(70, 52 + breatheShift, 6, 1, WHITE); 

    // ৫. আরামদায়ক শব্দ (Purring Sound - ৩ নম্বর পিন)
    // এটি একদম কম ফ্রিকোয়েন্সির শব্দ হবে যা শুনতে ভালো লাগে
    if (millis() % 2000 < 1000) { // ২ সেকেন্ড অন্তর অন্তর হালকা গুঞ্জন
        int purrFreq = 150 + (int)(sin(millis() * 0.01) * 50);
        tone(3, purrFreq, 15);
    } else {
        noTone(3);
    }

    // ৬. চারপাশের শান্ত পরিবেশের চিহ্ন (Floating '~' or Music note)
    if ((millis() / 800) % 2 == 0) {
        display.setCursor(110, 15 + breatheShift); display.print("~");
        display.setCursor(10, 25 - breatheShift); display.print("~");
    }

    display.display();
}


void expressionPhysicsBall() {
    display.clearDisplay();

    // ১. সেন্সর ডাটা আপডেট
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // রোবটটি কোন দিকে নাড়ানো হচ্ছে তা মাপার জন্য (Y অক্ষ ব্যবহার করছি)
    float moveForce = a.acceleration.y;

    // ২. ফিজিক্স ভ্যারিয়েবল (static রাখা হয়েছে যেন লুপের বাইরেও ডাটা মনে থাকে)
    static float ballY = 32.0;    
    static float velocity = 0.0;   
    const float gravity = 0.9;     // অভিকর্ষজ টান (বল নিচে পড়ার জন্য)
    const float bounce = -0.75;    // ড্রপ খাওয়ার শক্তি (বউন্স)
    const int topLimit = 22;       // চোখের বক্সের উপরের সীমা
    const int bottomLimit = 52;    // চোখের বক্সের নিচের সীমা

    // ৩. ঝাঁকুনি শনাক্ত করা (Shake Detection)
    // ঝাঁকুনি দিলে বলের বেগে নতুন গতি যোগ হবে
    if (abs(moveForce) > 16.0) {   
        velocity += (moveForce * 0.8); // ঝাঁকুনির জোর অনুযায়ী বেগে পরিবর্তন
    }

    // ৪. গতির আপডেট (Physics Logic)
    velocity += gravity;           // মাধ্যাকর্ষণ যোগ হচ্ছে
    ballY += velocity;             // সেই অনুযায়ী মণি পজিশন পরিবর্তন করছে

    // ৫. কলাইশন বা বর্ডারে বাড়ি খাওয়া (Collision Detection)
    // নিচে বাড়ি খেলে ড্রপ খাবে
    if (ballY >= bottomLimit) {
        ballY = bottomLimit;
        velocity *= bounce;        // গতি উল্টে গিয়ে ড্রপ খাবে
        
        // বাড়ি খাওয়ার শব্দ (৩ নম্বর পিন)
        if (abs(velocity) > 2.0) {
            tone(3, 900 + (int)abs(velocity * 10), 15);
        }
    }
    // উপরে বাড়ি খেলেও ড্রপ খাবে
    else if (ballY <= topLimit) {
        ballY = topLimit;
        velocity *= bounce;
    }

    // ৬. ড্রইং - চোখের ফ্রেম (বক্স)
    display.drawRoundRect(20, 18, 36, 38, 8, WHITE); // বাম বক্স
    display.drawRoundRect(72, 18, 36, 38, 8, WHITE); // ডান বক্স

    // ৭. ড্রইং - মণি (মার্বেল বলের মতো ড্রপ খাচ্ছে)
    int finalY = (int)ballY;
    
    // বাম মণি
    display.fillCircle(38, finalY, 7, WHITE);
    display.fillCircle(38, finalY, 2, BLACK); // মণির গভীরতা

    // ডান মণি
    display.fillCircle(90, finalY, 7, WHITE);
    display.fillCircle(90, finalY, 2, BLACK);

    display.display();
}
    
