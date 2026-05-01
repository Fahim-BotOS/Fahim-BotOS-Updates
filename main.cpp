#include <HTTPUpdate.h>
#include <Arduino.h>
#include <HTTPUpdate.h>

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
void playRelaxSound();
void drawMood(int mood);
void expressionSkeptical();
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
  
  server.on("/", []() { server.send(200, "text/html", getSetupHTML()); });
  server.on("/save", HTTP_POST, handleSave);
  server.onNotFound([]() { server.send(200, "text/html", getSetupHTML()); });
  server.begin();
  
  while(true) {
    dnsServer.processNextRequest();
    server.handleClient();
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(25, 10); display.print("SETUP MODE");
    display.drawLine(0, 20, 128, 20, WHITE);
    display.setCursor(5, 35); display.print("Connect WiFi: ");
    display.setCursor(5, 50); display.print(apSSID); // Pixel_Robot_Setup দেখাবে
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
  currentMood = 0; moodEndTime = 0; 
  server.send(200, "text/plain", "Normal Mode Active");
}

void handleDizzy() {
  currentMood = 4; moodEndTime = millis() + 5000; 
  playDizzySound(); server.send(200, "text/plain", "Dizzy Mode Active");
}

void handleAngry() {
  currentMood = 7; moodEndTime = millis() + 5000; 
  playAngrySound(); server.send(200, "text/plain", "Angry Mode Active");
}

void handleSleep() {
  currentMood = 5; lastSleepCheck = millis(); 
  playSleepSound(); server.send(200, "text/plain", "Sleep Mode Active");
}

// ==========================================
// --- ৩. সেটআপ ফাংশন ---
// ==========================================

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { for(;;); }
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
    // --- চূড়ান্ত টাচ লজিক ---
  int touchState = digitalRead(TOUCH_PIN);

  if (touchState == HIGH) {
    if (!isTouching) { 
      touchStartTime = currentMillis; 
      isTouching = true;
      touchCount++;
      lastTouchTime = currentMillis;
    }
    
    unsigned long duration = currentMillis - touchStartTime;

    // ১. ১০ সেকেন্ড ধরে রাখলে ঘুম (Sleep)
    if (duration >= 10000) {
      currentMood = 5; 
      moodEndTime = currentMillis + 10000;
    }
    // ২. ৩ থেকে ১০ সেকেন্ড ধরে রাখলে আরাম (Relax)
    else if (duration > 3000) {
      currentMood = 10; 
      moodEndTime = currentMillis + 1000; 
    }
    lastSleepCheck = currentMillis;
  } else {
    if (isTouching) {
      unsigned long finalDuration = currentMillis - touchStartTime;
      
      // ৩. ১০০ms এর কম টাচ করলে মন খারাপ (Sad)
      if (finalDuration < 100) {
        currentMood = 1; 
        moodEndTime = currentMillis + 3000;
        playTone(300, 200); // মন খারাপের শব্দ
      }
      isTouching = false;
    }
  }

  // ৪. দ্রুত ৩ বার টাচ করলে ভেংচি (Vebchi)
  if (touchCount == 3) {
    if (currentMillis - lastTouchTime < 1500) {
      currentMood = 9; 
      moodEndTime = currentMillis + 5000;
      playTone(800, 100); playTone(1200, 100); // ভেংচি কাটার শব্দ
      touchCount = 0;
    }
  }
  
  // কাউন্টার রিসেট
  if (currentMillis - lastTouchTime > 2000) touchCount = 0;

// --- ২. মুভমেন্ট ও তালি শনাক্তকরণ লজিক ---
detectGesture(); 


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
    isMoving = true; lastLookTime = 0; lastSleepCheck = currentMillis; 
  } else { isMoving = false; }

  // --- ৩. পেজ ও মুড টাইমিং ---
  if (currentMood == 0 || currentMillis > moodEndTime) {
    if (currentMood != 0 && currentMood != 5) currentMood = 0;
    if (!isMoving && !isTouching) {
      unsigned long timeInPage = currentMillis - lastPageChange;
      if (currentPage == 0 && timeInPage > 6000) { currentPage = 2; lastPageChange = currentMillis; } 
      else if (currentPage == 2 && timeInPage > 3000) { currentPage = 1; lastPageChange = currentMillis; } 
      else if (currentPage == 1 && timeInPage > 6000) { currentPage = 0; lastPageChange = currentMillis; }
    } else { currentPage = 0; lastPageChange = currentMillis; }
  }

  // --- ৪. স্লিপ ও আপডেট লজিক ---
  if (currentMillis - lastSleepCheck > 600000) { if (currentMood != 5) playSleepSound(); currentMood = 5; }
  if (currentMillis - lastWeatherUpdate > 600000) { fetchSmartWeather(); lastWeatherUpdate = currentMillis; }

  if (currentMillis - lastBlinkTime > random(3000, 6000)) { isBlinking = true; lastBlinkTime = currentMillis; }
  if (isBlinking && currentMillis - lastBlinkTime > 150) isBlinking = false;

  // --- ৫. রেন্ডারিং ---
  display.clearDisplay();
  if (currentMillis < moodEndTime || currentMood == 5) { drawMood(currentMood); } 
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
    digitalWrite(BUZZER_PIN, HIGH); delayMicroseconds(delayValue);
    digitalWrite(BUZZER_PIN, LOW); delayMicroseconds(delayValue);
  }
}

void playBootSound() { playTone(1000, 100); delay(50); playTone(1500, 200); }
void playSadSound() { for (int hz = 600; hz > 200; hz -= 10) playTone(hz, 25); delay(100); playTone(180, 600); }
void playLoveSound() { playTone(1200, 150); delay(50); playTone(1500, 150); delay(50); playTone(2000, 300); }
void playAngrySound() { for(int i=0; i<5; i++) { playTone(200, 100); delay(50); } }
void playDizzySound() { for (int hz = 400; hz < 1200; hz += 40) playTone(hz, 10); for (int hz = 1200; hz > 400; hz -= 40) playTone(hz, 10); }
void playSleepSound() { for (int hz = 150; hz < 350; hz += 10) playTone(hz, 25); delay(100); for (int hz = 300; hz > 120; hz -= 8) playTone(hz, 40); }
void playScaredSound() { for(int i=0; i<8; i++) { playTone(random(800, 1800), 40); delay(20); } }
void playMoveSound() { for (int hz = 1200; hz < 1800; hz += 100) playTone(hz, 5); for (int hz = 1800; hz > 1200; hz -= 100) playTone(hz, 5); }
void playRelaxSound() {
  static unsigned long lastSoundTime = 0;
  if (millis() - lastSoundTime > 800) { 
    int freq = 200 + sin(millis() * 0.005) * 50; 
    playTone(freq, 150); 
    lastSoundTime = millis();
  }
}

void drawMood(int mood) {
  switch(mood) {
    case 1: expressionSad(); break;
    case 2: expressionLove(); break;
    case 4: expressionDizzy(); break;
    case 5: expressionSleepy(); break;
    case 7: expressionSuperAngry(); break;
    case 8: expressionSkeptical(); break; 
    case 9: expressionVebchi(); break;
    case 10: expressionRelax(); break;
  }
}



void expressionSkeptical() {
  display.clearDisplay();

  // ১. বাম চোখ (একটু বড় এবং সন্দেহজনকভাবে তাকিয়ে থাকা)
  display.fillRoundRect(25, 25, 30, 25, 8, WHITE);
  // মণি - যা ডানে এবং বামে হালকা নড়াচড়া করবে (সন্দেহ করার মতো)
  int lookOffset = (millis() / 1000) % 2 == 0 ? 2 : -2; 
  display.fillCircle(40 + lookOffset, 37, 5, BLACK); 

  // ২. ডান চোখ (সরু বা ছোট করা চোখ - এটিই স্কেপটিক্যাল লুক দেয়)
  // চোখটি একটু উপরে উঠানো থাকবে
  display.fillRoundRect(73, 20, 30, 15, 4, WHITE);
  display.fillCircle(88 + lookOffset, 27, 4, BLACK);

  // ৩. সন্দেহজনক মুখ (একটি বাঁকা লাইন যা কাঁপবে)
  int mouthWiggle = (millis() % 200 > 100) ? 1 : 0;
  display.drawLine(54, 52 + mouthWiggle, 74, 52 - mouthWiggle, WHITE);
  
  // ৪. একটি ছোট্ট ঘাম বা দুশ্চিন্তার বিন্দু (Sweat drop) - যা কিউটনেস বাড়াবে
  if ((millis() / 2000) % 2 == 0) {
    display.fillCircle(110, 15, 2, WHITE);
    display.drawLine(110, 13, 110, 10, WHITE);
  }

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
  display.fillRoundRect(25, 40, 30, 8, 4, WHITE); display.fillRoundRect(73, 40, 30, 8, 4, WHITE);
  int zPos = (millis() / 700) % 3; 
  display.setTextSize(1);
  if(zPos >= 0) { display.setCursor(100, 15); display.print("z"); }
  if(zPos >= 1) { display.setCursor(110, 10); display.print("z"); }
  if(zPos >= 2) { display.setCursor(118, 5); display.print("z"); }
}

void expressionSuperAngry() {
  display.drawLine(20, 15, 55, 30, WHITE); display.drawLine(108, 15, 73, 30, WHITE);
  drawEye(25, 25, 30, 20, 5); drawEye(73, 25, 30, 20, 5);
}

void drawEye(int x, int y, int w, int h, int r) {
  if (isBlinking) { display.fillRect(x, y + (h/2), w, 4, WHITE); } 
  else { display.fillRoundRect(x, y, w, h, r, WHITE); display.fillCircle(x + (w/2) + pupilX, y + (h/2) + pupilY, 4, BLACK); }
}

void expressionNormal() {
  if (millis() - lastLookTime > (isMoving ? 150 : random(2000, 5000))) {
    pupilX = random(-7, 8); pupilY = random(-6, 7); lastLookTime = millis();
  }
  drawEye(25, 20, 30, 30, 8); drawEye(73, 20, 30, 30, 8);
}

void expressionSad() {
  display.clearDisplay();
  
  // ১. দুঃখিত ভ্রু (Sad Eyebrows) - যা কিউটনেস বাড়াবে
  display.drawLine(25, 15, 45, 22, WHITE); // বাম ভ্রু
  display.drawLine(103, 15, 83, 22, WHITE); // ডান ভ্রু

  // ২. চোখ (মণি নিচের দিকে নামানো)
  // আপনার আগের স্থানাঙ্ক অনুযায়ী
  display.fillRoundRect(25, 25, 30, 25, 8, WHITE); // বাম চোখ
  display.fillRoundRect(73, 25, 30, 25, 8, WHITE); // ডান চোখ
  
  // মণি (নিচের দিকে তাকিয়ে থাকা)
  display.fillCircle(40, 42, 4, BLACK); 
  display.fillCircle(88, 42, 4, BLACK);

  // ৩. চোখের জল (Tear Drop) - এটি প্রফেশনাল লুক দেবে
  static int tearY = 45;
  display.fillTriangle(30, tearY, 34, tearY, 32, tearY - 4, WHITE);
  display.fillCircle(32, tearY + 2, 2, WHITE);
  
  tearY++; // চোখের জল নিচে পড়বে
  if (tearY > 60) tearY = 45;

  // ৪. মুখ (উল্টো বাঁকানো ছোট মুখ)
  display.drawCircleHelper(64, 62, 10, 1, WHITE); 
  display.drawCircleHelper(64, 62, 10, 2, WHITE); 

  display.display();
}


void expressionLove() {
  display.clearDisplay();
  
  // ১. স্মুথ পালসিং ক্যালকুলেশন (sine wave ব্যবহার করে)
  // এটি হার্টের স্পন্দনকে খুব স্মুথ এবং প্রফেশনাল করে তুলবে
  float pulse = sin(millis() * 0.005) * 3; 
  int s = (int)pulse;

  // ২. বাম পাশের হার্ট (বাম চোখ)
  display.fillCircle(32, 28, 8 + s, WHITE); 
  display.fillCircle(48, 28, 8 + s, WHITE);
  display.fillTriangle(24 - s, 32, 56 + s, 32, 40, 52 + s, WHITE);

  // ৩. ডান পাশের হার্ট (ডান চোখ)
  display.fillCircle(80, 28, 8 + s, WHITE); 
  display.fillCircle(96, 28, 8 + s, WHITE);
  display.fillTriangle(72 - s, 32, 104 + s, 32, 88, 52 + s, WHITE);

  // ৪. ফ্লোটিং হার্ট এনিমেশন (ছোট ছোট হার্ট উপরে উড়ে যাবে)
  static int heartY = 20;
  static int heartX = 64;
  display.drawPixel(heartX, heartY, WHITE);
  display.drawPixel(heartX-1, heartY-1, WHITE);
  display.drawPixel(heartX+1, heartY-1, WHITE);
  
  heartY--; // হার্ট উপরে উঠবে
  if (heartY < 0) {
    heartY = 30;
    heartX = random(40, 90);
  }

  // ৫. কিউট স্মাইল
  display.drawCircleHelper(64, 48, 8, 2, WHITE); 

  display.display();
}


void showWeatherPage() {
  display.setTextSize(1); display.setCursor(0, 0); display.print(currentLocation);
  display.drawLine(0, 10, 128, 10, WHITE);
  display.setTextSize(2); display.setCursor(0, 20); display.print(weatherTemp + " C ");
  display.setTextSize(1); display.print(weatherDesc);
  display.drawLine(0, 42, 128, 42, WHITE);
  display.setCursor(0, 48); display.print("Hum: " + humidity + "%");
  display.setCursor(65, 48); display.print("Wind: " + windSpeed + "m/s");
}

void showTimePage() {
  display.setTextSize(1); display.setCursor(45, 5); display.print("TIME");
  display.drawLine(0, 15, 128, 15, WHITE);
  display.setTextSize(3); display.setCursor(20, 30); display.print(timeClient.getFormattedTime().substring(0, 5));
}

void drawLoading() {
  display.clearDisplay(); display.setCursor(20, 30); display.print("Connecting WiFi..."); display.display();
}

void drawAutoExpressions() { expressionNormal();
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

void expressionRelax() {
  display.clearDisplay();
  
  // ভ্রু
  display.drawCircleHelper(40, 32, 18, 1, WHITE); 
  display.drawCircleHelper(88, 32, 18, 1, WHITE);

  // চোখ
  display.drawCircle(40, 42, 16, WHITE); 
  display.drawCircle(88, 42, 16, WHITE);

  // চোখের মোশন
  for (int i = 0; i < 12; i++) {
    int y = 44 + sin((millis() * 0.005) + i) * 3;
    display.drawPixel(34 + i, y, WHITE);
    display.drawPixel(82 + i, y, WHITE);
  }
  
  // মুখ
  display.fillCircleHelper(64, 52, 10, 2, 0, WHITE); 
  display.fillCircleHelper(64, 52, 10, 1, 0, WHITE); 
  
  if (millis() % 1000 < 500) {
     display.fillCircle(64, 58, 6, BLACK); 
  }

  // সাউন্ড কল করা
  playRelaxSound(); 

  display.display();
}

void expressionVebchi() {
  display.clearDisplay();
  
  // ১. চোখ (চিত্র ১৮৪২৮ অনুযায়ী - একটি খোলা ও একটি বন্ধ)
  display.fillCircle(88, 35, 12, WHITE); // ডান চোখ খোলা
  display.fillCircle(88, 35, 4, BLACK);  // মণি
  
  // বাম চোখ বন্ধ (উইঙ্ক করা)
  display.drawLine(25, 35, 45, 25, WHITE); 
  display.drawLine(25, 35, 45, 45, WHITE);

  // ২. ভ্রু
  display.drawLine(20, 15, 40, 20, WHITE);
  display.drawLine(108, 15, 88, 20, WHITE);

  // ৩. মুখ ও জিবের অ্যানিমেশন
  display.drawRoundRect(44, 50, 40, 10, 5, WHITE); // মুখ
  
  static float tongueOffset = 0;
  tongueOffset = sin(millis() * 0.01) * 4; // জিব ডানে-বামে নড়বে
  
  display.fillRoundRect(56 + tongueOffset, 55, 16, 12, 5, WHITE); // জিব
  display.drawLine(64 + tongueOffset, 55, 64 + tongueOffset, 62, BLACK); // জিবের রেখা

  display.display();
}

