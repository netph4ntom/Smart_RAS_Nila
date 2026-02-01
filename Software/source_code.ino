#define BLYNK_TEMPLATE_ID "TMPL6vOlwLdIh"
#define BLYNK_TEMPLATE_NAME "Monitoring Kualitas Air"
#define BLYNK_AUTH_TOKEN "XKD7xeGmtRNOvuTW-M8DknyY-QWbNJhA"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ==== WiFi & Blynk ====
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "yanto";
char pass[] = "Nadhifahh";

// ==== LCD I2C ====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==== DS18B20 ====
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ==== Sensor Analog ====
#define PH_PIN 34
#define TURBIDITY_PIN 35

// ==== Push Button Reset ====
#define RESET_BTN 32

// ==== Variabel Global ====
float phValue, turbidityNTU, tempC;
String globalStatus;
BlynkTimer timer;
bool wifiConnected = false;
bool sensorsOK = true;

// ==== Fungsi Baca Analog Rata-rata ====
const int SAMPLES = 10;
float readAnalogAverage(int pin) {
  long rawSum = 0;
  for (int i = 0; i < SAMPLES; i++) {
    rawSum += analogRead(pin);
    delay(5);
  }
  return rawSum / (float)SAMPLES;
}

// ==== Sensor ====
float readPH(int pin) {
  float voltage = (readAnalogAverage(pin) / 4095.0) * 3.3;
  float ph = -5.7 * voltage + 21.34;
  return constrain(ph, 0, 14);
}

float readTurbidity(int pin) {
  float rawAvg = readAnalogAverage(pin);
  int adcClear = 4095, adcMuddy = 2700;
  float ntu = (rawAvg - adcClear) * (50.0 / (adcMuddy - adcClear));
  return constrain(ntu, 0, 50);
}

float readTemperature() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  if (temp == DEVICE_DISCONNECTED_C || temp < -10 || temp > 60) {
    sensorsOK = false;
    return -999;
  }
  sensorsOK = true;
  return temp;
}

// ==== Evaluasi Status ====
String evaluateStatus(float ph, float temp, float turb) {
  if (ph < 6.0 || ph > 9.0 || temp < 20 || temp > 35 || turb > 50) return "DANGER";
  if (ph < 6.5 || ph > 8.5 || temp < 24 || temp > 30 || turb > 35) return "WARNING";
  return "OPTIMAL";
}

// ==== Monitoring Task ====
void monitorTask() {
  // cek reset button
  if (digitalRead(RESET_BTN) == LOW) {
    lcd.clear(); lcd.setCursor(0,0); lcd.print("RESET SYSTEM");
    delay(500); ESP.restart();
  }

  // baca sensor
  turbidityNTU = readTurbidity(TURBIDITY_PIN);
  tempC = readTemperature();

  // slide LCD
  static int displayMode = 0;
  lcd.clear();
  switch (displayMode % 3) {
    case 0: { // Sistem Info
        unsigned long uptimeSec = millis()/1000;
        lcd.setCursor(0,0);
        lcd.print("WiFi:"); lcd.print(WiFi.status() == WL_CONNECTED?"OK":"NO");
        lcd.setCursor(0,1);
        lcd.print("Uptime: ");
        lcd.print(uptimeSec/3600); lcd.print("h");
        lcd.print((uptimeSec%3600)/60); lcd.print("m");
      } break;

    case 1: { // Sensor Data (Dummy pH)
        float dummyPH = random(30, 150) / 10.0;
        lcd.setCursor(0,0);
        lcd.print("pH:"); lcd.print(dummyPH,1);
        lcd.print(" T:"); lcd.print(tempC,1); lcd.print("C");
        lcd.setCursor(0,1);
        lcd.print("Turb:"); lcd.print(turbidityNTU,1); lcd.print(" NTU");

        // kirim ke Blynk
        if (WiFi.status() == WL_CONNECTED && Blynk.connected()) {
          Blynk.virtualWrite(V1, dummyPH);
          Blynk.virtualWrite(V2, tempC);
          Blynk.virtualWrite(V3, turbidityNTU);
          Blynk.virtualWrite(V4, evaluateStatus(dummyPH, tempC, turbidityNTU));
        }
      } break;

    case 2: { // Status Keseluruhan
        float dummyPH = random(30, 150) / 10.0;
        lcd.setCursor(0,0);
        lcd.print("Status: "); lcd.print(evaluateStatus(dummyPH, tempC, turbidityNTU));
        lcd.setCursor(0,1);
        lcd.print("Sensors: "); lcd.print(sensorsOK?"OK":"NO");
      } break;
  }
  displayMode++;
}

// ==== Setup & Loop ====
void setup() {
  Serial.begin(115200);
  lcd.init(); lcd.backlight(); lcd.print("Starting System");
  sensors.begin(); sensors.setResolution(12);
  pinMode(RESET_BTN, INPUT_PULLUP);

  // WiFi
  WiFi.begin(ssid, pass);

  // set timer
  timer.setInterval(5000L, monitorTask); // update tiap 5 detik
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) Blynk.run();
  timer.run();
}
