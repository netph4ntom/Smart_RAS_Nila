# Smart RAS Nila

Sistem monitoring kualitas air kolam budidaya ikan nila berbasis IoT dengan koneksi real-time ke Blynk Dashboard dan display informasi di LCD.

## Deskripsi Project

Smart RAS (Recirculating Aquaculture System) Nila adalah solusi monitoring otomatis untuk budidaya ikan nila yang menggunakan sensor-sensor untuk memantau kualitas air secara real-time. Sistem ini terintegrasi dengan aplikasi Blynk melalui WiFi, memungkinkan pemantauan jarak jauh dan notifikasi otomatis jika parameter air tidak optimal.

## Fitur Utama

- **Multi-Sensor Monitoring** - Turbidity, Suhu, dan pH sensor untuk monitoring komprehensif
- **Real-time Dashboard** - Monitoring via aplikasi Blynk di smartphone
- **WiFi Connectivity** - Data upload otomatis ke cloud
- **LCD Display** - Informasi status langsung di lokasi kolam
- **Alert System** - Notifikasi otomatis jika kualitas air buruk
- **Data Logging** - History data untuk analisis trend

## Komponen Hardware

### Mikrokontroler & Connectivity
- **ESP32** - Main controller dengan WiFi built-in
- **I2C LCD Module** - LCD 16x2 dengan modul I2C
- **Step-Down Module** - Regulator tegangan 5V

### Sensor
- **Turbidity Sensor** - Mengukur tingkat kekeruhan air
- **Temperature Sensor** (DS18B20/DHT) - Monitoring suhu air
- **pH Sensor** - Mengukur tingkat keasaman air

### Power Supply
- **Adaptor 12V** - Sumber daya utama
- **Step-Down Converter** - Konversi 12V ke 5V untuk ESP32

### Komponen Pendukung
- Kabel jumper
- Breadboard/PCB
- Casing
- Kabel sensor
  
## Parameter Monitoring

### 1. Turbidity (Kekeruhan)
- **Range**: 0 - 1000 NTU
- **Optimal untuk Nila**: < 25 NTU
- **Status**:
  - Jernih: 0-25 NTU
  - Keruh: 25-100 NTU
  - Sangat Keruh: > 100 NTU

### 2. Suhu Air
- **Range**: 0 - 50°C
- **Optimal untuk Nila**: 25-30°C
- **Status**:
  - Dingin: < 20°C
  - Optimal: 20-32°C
  - Panas: > 32°C

### 3. pH (Keasaman)
- **Range**: 0 - 14 pH
- **Optimal untuk Nila**: 6.5-8.5 pH
- **Status**:
  - Asam: < 6.5
  - Optimal: 6.5-8.5
  - Basa: > 8.5

## Cara Kerja Sistem

### Flow Monitoring
1. **Sensor Reading**: 
   - Sensor turbidity, suhu, dan pH membaca kondisi air
   - Data dikonversi dari analog ke digital (ADC)
   
2. **Data Processing**:
   - ESP32 memproses data dari semua sensor
   - Kalibrasi dan konversi ke satuan yang sesuai
   - Evaluasi status berdasarkan threshold

3. **Display Update**:
   - Data ditampilkan di LCD 16x2
   - Update setiap 2 detik
   - Scroll display untuk semua parameter

4. **Cloud Upload**:
   - Data dikirim ke Blynk Cloud via WiFi
   - Update setiap 5 detik
   - Virtual pins untuk setiap parameter

5. **Alert System**:
   - Jika parameter diluar range optimal
   - Notifikasi push ke aplikasi Blynk
   - Indikator visual di dashboard

## Blynk Dashboard Setup

### Virtual Pins Configuration
| Virtual Pin | Parameter | Widget Type |
|-------------|-----------|-------------|
| V0 | Turbidity (NTU) | Value Display + Gauge |
| V1 | Temperature (°C) | Value Display + Gauge |
| V2 | pH Level | Value Display + Gauge |
| V3 | Water Status | LED Indicator |
| V4 | System Online | LED Indicator |

### Notification Settings
- **Turbidity Alert**: V0 > 100 NTU
- **Temperature Alert**: V1 < 20°C atau > 32°C
- **pH Alert**: V2 < 6.5 atau > 8.5

##  Instalasi & Setup

### Requirements
- Arduino IDE
- ESP32 Board Support
- Library yang dibutuhkan:
  - `WiFi` (built-in ESP32)
  - `BlynkSimpleEsp32`
  - `LiquidCrystal_I2C`
  - `OneWire` (untuk DS18B20)
  - `DallasTemperature` (untuk DS18B20)

### Langkah Instalasi

1. **Clone Repository**
   ```bash
   git clone https://github.com/username/Smart_RAS_Nila.git
   cd Smart_RAS_Nila
   ```

2. **Install ESP32 Board**
   - Buka Arduino IDE
   - File → Preferences
   - Additional Board URLs: 
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Tools → Board → Board Manager
   - Search "ESP32" dan install

3. **Install Libraries**
   - Sketch → Include Library → Manage Libraries
   - Install semua library yang dibutuhkan

4. **Setup Blynk Account**
   - Download aplikasi Blynk di smartphone
   - Buat akun baru
   - Create New Project → pilih "ESP32"
   - Salin Auth Token yang diberikan

5. **Konfigurasi Code**
   - Buka `Software/source_code.ino`
   - Edit credentials WiFi:
     ```cpp
     char ssid[] = "NAMA_WIFI_KAMU";
     char pass[] = "PASSWORD_WIFI";
     ```
   - Edit Blynk Auth Token:
     ```cpp
     char auth[] = "TOKEN_DARI_BLYNK";
     ```

6. **Upload Code**
   - Pilih board: `ESP32 Dev Module`
   - Pilih port yang sesuai
   - Upload code ke ESP32

7. **Setup Hardware**
   - Pasang semua sensor sesuai diagram pin
   - Sambungkan LCD via I2C
   - Hubungkan power supply
   - Celupkan sensor ke kolam air

8. **Testing**
   - Buka Serial Monitor (115200 baud)
   - Cek koneksi WiFi dan Blynk
   - Verifikasi pembacaan sensor
   - Cek tampilan LCD dan Blynk app

## Kalibrasi Sensor

### Kalibrasi pH Sensor
1. Siapkan larutan buffer pH 4.0 dan pH 7.0
2. Celupkan sensor di pH 4.0, catat nilai ADC
3. Celupkan sensor di pH 7.0, catat nilai ADC
4. Hitung slope dan offset
5. Update nilai kalibrasi di code

### Kalibrasi Turbidity Sensor
1. Gunakan air bersih (0 NTU) sebagai referensi
2. Catat nilai ADC maksimal
3. Gunakan air keruh standar (100-500 NTU)
4. Update mapping di code

### Kalibrasi Temperature Sensor
- DS18B20 sudah terkalibrasi dari pabrik
- Bisa cross-check dengan termometer standar

## Struktur Folder

```
Smart_RAS_Nila/
├── Hardware/
│   ├── rangkaian.png        
│   └── alat_jadi.jpg        
├── Software/
│   └── source_code.ino     
└── README.md
```

## Troubleshooting

| Masalah | Solusi |
|---------|--------|
| ESP32 tidak connect WiFi | Cek SSID/password, pastikan sinyal kuat |
| LCD tidak tampil | Cek alamat I2C (scan dengan I2C scanner) |
| Blynk offline | Cek koneksi internet, verifikasi auth token |
| Sensor pH tidak akurat | Lakukan kalibrasi ulang dengan buffer solution |
| Turbidity tidak stabil | Pastikan sensor bersih, tidak ada gelembung udara |
| Suhu tidak terbaca | Cek koneksi DS18B20, pastikan pull-up resistor 4.7kΩ |
| Serial monitor crash | Turunkan baud rate, cek power supply |
