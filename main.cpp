#include <HTTPUpdate.h>
const char* firmware_url = "https://github.com/Fahim-BotOS/Fahim-BotOS-Updates/releases/latest/download/firmware.bin";
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
  server.send(200, "text/html", "<h2>Setup Saved!</h2><p>Pixel is restarting to connect...</p>");
  delay(2000);
  preferences.begin("wifi-data", false);
  preferences.putString("ssid", stSSID);
  preferences.putString("pass", stPass);
  preferences.end();
  ESP.restart();
}

void startCaptivePortal() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  server.on("/", []() { server.send(200, "text/html", getSetupHTML()); });
  server.on("/save", HTTP_POST, handleSave);
  server.onNotFound([]() { server.send(200, "text/html", getSetupHTML()); });
  server.begin();
  
  while(true) {
    dnsServer.processNextRequest();
    server.handleClient();
    display.clearDisplay();
    display.setCursor(25, 10); display.print("SETUP MODE");
    display.drawLine(0, 20, 128, 20, WHITE);
    display.setCursor(5, 35); display.print("Connect: ");
    display.setCursor(5, 45); display.print(apSSID);
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
  preferences.begin("wifi-data", true);
  stSSID = preferences.getString("ssid", "");
  stPass = preferences.getString("pass", "");
  preferences.end();

  if (stSSID == "") {
    startCaptivePortal();
  } else {
    WiFi.begin(stSSID.c_str(), stPass.c_str());
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 30) {
      drawLoading();
      delay(500);
      retry++;
    }
    if (WiFi.status() != WL_CONNECTED) {
      startCaptivePortal();
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
  int touchState = digitalRead(TOUCH_PIN);
  if (touchState == HIGH) {
    lastSleepCheck = currentMillis; 
    if (!isTouching) { touchStartTime = currentMillis; isTouching = true; }
    unsigned long duration = currentMillis - touchStartTime;
    if (duration > 6000) { if (currentMood != 7) playAngrySound(); currentMood = 7; moodEndTime = currentMillis + 5000; } 
    else if (duration > 2000) { if (currentMood != 2) playLoveSound(); currentMood = 2; moodEndTime = currentMillis + 3000; }
  } 
  else {
    if (isTouching) { if (currentMillis - touchStartTime < 500) { currentMood = 1; playSadSound(); moodEndTime = currentMillis + 4000; } isTouching = false; }
  }

  // --- ২. মুভমেন্ট ও তালি শনাক্তকরণ লজিক ---
  float totalAccel = sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z);
  
  // ঝাঁকি দিলে Dizzy হওয়া
  if (totalAccel > 15.0) { if (currentMood != 4) playDizzySound(); currentMood = 4; moodEndTime = currentMillis + 5000; lastSleepCheck = currentMillis; }

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

void drawMood(int mood) {
  switch(mood) {
    case 1: expressionSad(); break;
    case 2: expressionLove(); break;
    case 4: expressionDizzy(); break;
    case 5: expressionSleepy(); break;
    case 7: expressionSuperAngry(); break;
    case 8: expressionSkeptical(); break; 
  }
}

void expressionSkeptical() {
  display.fillCircle(40, 30, 10, WHITE); display.fillCircle(40, 30, 4, BLACK);
  display.fillCircle(88, 25, 12, WHITE); display.fillCircle(88, 25, 5, BLACK);
  display.fillCircle(64, 50, 8, WHITE);  display.fillCircle(64, 50, 5, BLACK);
}

void expressionDizzy() {
  display.drawCircle(40, 35, 15, WHITE); display.drawCircle(88, 35, 15, WHITE);
  float angle = millis() * 0.01; 
  for (int i = 0; i < 2; i++) {
    int centerX = (i == 0) ? 40 : 88;
    for (int r = 2; r < 12; r += 4) {
      int xOffset = r * cos(angle + r); int yOffset = r * sin(angle + r);
      display.drawCircle(centerX + xOffset, 35 + yOffset, 2, WHITE);
    }
  }
  display.drawEllipse(64, 55, 10, 6, WHITE); 
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
  display.fillRoundRect(45, 20, 10, 22, 5, WHITE); display.fillRoundRect(73, 20, 10, 22, 5, WHITE);
  display.drawCircleHelper(64, 60, 15, 1, WHITE); display.drawCircleHelper(64, 60, 15, 2, WHITE); 
}

void expressionLove() {
  int s = ((millis() / 500) % 2) * 2; 
  display.fillCircle(32, 28, 7 + s, WHITE); display.fillCircle(47, 28, 7 + s, WHITE);
  display.fillTriangle(25 - s, 32, 54 + s, 32, 40, 50 + s, WHITE);
  display.fillCircle(80, 28, 7 + s, WHITE); display.fillCircle(95, 28, 7 + s, WHITE);
  display.fillTriangle(73 - s, 32, 102 + s, 32, 88, 50 + s, WHITE);
  display.drawCircleHelper(64, 45, 8, 2, WHITE); 
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

void drawAutoExpressions() { expressionNormal(); }
    
