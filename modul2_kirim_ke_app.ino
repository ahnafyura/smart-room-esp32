// Modul 2 - Pertemuan 4
// ESP32 tersambung WiFi, lalu MENGIRIM suhu & kelembapan ke internet (MQTT)
// supaya bisa dibaca oleh web app (app.html).
//
// Rangkaian: sama dengan Modul 1.
// Library (tab Library Manager): DHT sensor library for ESPx, PubSubClient
// GANTI "kelompok1" dengan nama kelompok kalian (harus SAMA dengan di app).

#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

#define DHT_PIN 21
#define LED_PIN 14

const char* WIFI_NAMA = "Wokwi-GUEST";   // WiFi virtual milik Wokwi
const char* BROKER = "broker.emqx.io";   // "kantor pos" MQTT gratis
const char* TOPIK_SUHU = "sman19/kelompok1/suhu";
const char* TOPIK_LEMBAP = "sman19/kelompok1/lembap";

WiFiClient wifi;
PubSubClient mqtt(wifi);
DHTesp dht;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    dht.setup(DHT_PIN, DHTesp::DHT22);

    WiFi.begin(WIFI_NAMA, "", 6);       // 6 = channel WiFi Wokwi (lebih cepat)
    Serial.print("Menyambung WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
    }
    Serial.println(" tersambung!");

    mqtt.setServer(BROKER, 1883);
}

void loop() {
    if (!mqtt.connected()) {
        String id = "esp32-sman19-" + String(random(100000));
        if (mqtt.connect(id.c_str())) {
            Serial.println("MQTT tersambung!");
        } else {
            delay(1000);
            return;                  // coba lagi di putaran loop berikutnya
        }
    }
    mqtt.loop();

    float suhu = dht.getTemperature();
    float lembap = dht.getHumidity();
    mqtt.publish(TOPIK_SUHU, String(suhu, 1).c_str());
    mqtt.publish(TOPIK_LEMBAP, String(lembap, 0).c_str());

    Serial.print("Terkirim -> suhu ");
    Serial.print(suhu);
    Serial.print(" C, lembap ");
    Serial.print(lembap);
    Serial.println(" %");
    delay(2000);
}
