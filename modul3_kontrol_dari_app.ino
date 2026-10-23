// Modul 3 - Pertemuan 4
// Dua arah: ESP32 MENGIRIM suhu ke app, dan MENERIMA perintah "ON"/"OFF"
// dari tombol lampu di app untuk menyalakan/mematikan LED.
//
// Rangkaian: sama dengan Modul 1.
// Library (tab Library Manager): DHT sensor library for ESPx, PubSubClient
// GANTI "kelompok1" dengan nama kelompok kalian (harus SAMA dengan di app).

#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

#define DHT_PIN 21
#define LED_PIN 14

const char* WIFI_NAMA = "Wokwi-GUEST";
const char* BROKER = "broker.emqx.io";
const char* TOPIK_SUHU = "sman19/kelompok1/suhu";
const char* TOPIK_LEMBAP = "sman19/kelompok1/lembap";
const char* TOPIK_LAMPU = "sman19/kelompok1/lampu";   // perintah dari app

WiFiClient wifi;
PubSubClient mqtt(wifi);
DHTesp dht;
unsigned long waktuKirim = 0;

// Dipanggil OTOMATIS setiap ada pesan masuk dari app
void pesanMasuk(char* topik, byte* isi, unsigned int panjang) {
    String pesan = "";
    for (int i = 0; i < panjang; i++) {
        pesan += (char)isi[i];
    }
    Serial.println("Perintah dari app: " + pesan);

    if (pesan == "ON") {
        digitalWrite(LED_PIN, HIGH);
    } else if (pesan == "OFF") {
        digitalWrite(LED_PIN, LOW);
    }
}

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
    mqtt.setCallback(pesanMasuk);    // daftarkan fungsi penerima pesan
}

void loop() {
    if (!mqtt.connected()) {
        String id = "esp32-sman19-" + String(random(100000));
        if (mqtt.connect(id.c_str())) {
            mqtt.subscribe(TOPIK_LAMPU); // "langganan" perintah lampu
            Serial.println("MQTT tersambung!");
        } else {
            delay(1000);
            return;
        }
    }
    mqtt.loop();                     // cek pesan masuk (jangan diblok delay!)

    // kirim data tiap 2 detik tanpa delay(), supaya tombol app tetap responsif
    if (millis() - waktuKirim > 2000) {
        waktuKirim = millis();
        float suhu = dht.getTemperature();
        float lembap = dht.getHumidity();
        mqtt.publish(TOPIK_SUHU, String(suhu, 1).c_str());
        mqtt.publish(TOPIK_LEMBAP, String(lembap, 0).c_str());
    }
}
