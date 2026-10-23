// Modul 1 - Pertemuan 4
// ESP32 membaca suhu & kelembapan dari DHT22, lalu menampilkannya di Serial Monitor.
// LED di pin 14 menyala kalau suhu lebih dari 30 derajat.
//
// Rangkaian di Wokwi (lihat diagram.json):
//   DHT22 VCC -> 3V3, GND -> GND, SDA -> pin 21
//   LED kaki A (+) -> resistor 220 ohm -> pin 14, kaki C (-) -> GND
// Library (tab Library Manager): DHT sensor library for ESPx

#include "DHTesp.h"

#define DHT_PIN 21
#define LED_PIN 14

DHTesp dht;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    dht.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
    float suhu = dht.getTemperature();
    float lembap = dht.getHumidity();

    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" C | Kelembapan: ");
    Serial.print(lembap);
    Serial.println(" %");

    if (suhu > 30) {
        digitalWrite(LED_PIN, HIGH); // panas -> LED nyala
    } else {
        digitalWrite(LED_PIN, LOW);
    }
    delay(2000);                     // baca tiap 2 detik
}
