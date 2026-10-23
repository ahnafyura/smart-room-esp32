# Smart Room ESP32

Proyek praktik **Pertemuan 4 — Integrasi Software & Hardware** (Ekstrakurikuler Pemrograman SMAN 19 Surabaya).

ESP32 membaca suhu & kelembapan dari sensor DHT22, mengirimnya lewat internet (MQTT) ke **web app**, dan LED bisa dinyalakan/dimatikan dari web app. Semua dijalankan di simulator **[Wokwi](https://wokwi.com)**, tanpa alat fisik.

📄 **Modul lengkap (langkah bergambar, penjelasan konsep, soal tantangan, checklist): [materi_pertemuan4.pdf](materi_pertemuan4.pdf)**

```
[DHT22] -> [ESP32] --WiFi--> [broker.emqx.io] --internet--> [app.html]
               ^                                                |
               +---------- perintah "ON"/"OFF" lampu -----------+
```

## Isi repo

| File | Fungsi |
|---|---|
| `diagram.json` | Rangkaian Wokwi: ESP32 + DHT22 + LED + resistor 220Ω |
| `libraries.txt` | Daftar library Wokwi (DHT sensor library for ESPx, PubSubClient) |
| `modul1_sensor.ino` | Modul 1 — baca suhu, LED menyala kalau suhu > 30°C |
| `modul2_kirim_ke_app.ino` | Modul 2 — kirim suhu & kelembapan ke web app (ESP32 → App) |
| `modul3_kontrol_dari_app.ino` | Modul 3 — dua arah: kirim suhu + terima perintah lampu (App → ESP32) |
| `app.html` | Web app "Smart Room": tampilkan suhu, tombol lampu, log pesan |
| `materi_pertemuan4.pdf` | Modul praktik lengkap (9 halaman) dengan screenshot dan soal tantangan |

## Cara pakai

### 1. Clone / download

```bash
git clone https://github.com/ahnafyura/smart-room-esp32.git
```

Tidak punya git? Klik tombol hijau **Code → Download ZIP** di halaman repo ini, lalu ekstrak.

### 2. Siapkan proyek di Wokwi

1. Buka [wokwi.com](https://wokwi.com) → **ESP32** → template **ESP32** (Arduino).
2. Tab `diagram.json` → hapus semua isinya → paste isi `diagram.json` dari repo ini.
3. Tab **Library Manager** → **+** → tambahkan `DHT sensor library for ESPx` dan `PubSubClient`.
4. Tab `sketch.ino` → paste isi file modul yang sedang dikerjakan (`modul1_...`, `modul2_...`, atau `modul3_...`).
5. **Modul 2 & 3:** ganti `kelompok1` di baris `TOPIK_...` dengan nama kelompok kalian yang unik, misalnya `kel3-ayu-budi`.
6. Klik ▶ (hijau). Tunggu Serial Monitor menampilkan `MQTT tersambung!`.

### 3. Buka web app

1. Klik 2× file `app.html` (terbuka di Chrome/Edge; bisa juga dikirim ke HP).
2. Isi **Nama kelompok** persis sama dengan di kode ESP32 → **Sambungkan**.
3. Suhu & kelembapan muncul tiap 2 detik. Di Modul 3, tombol **Nyalakan/Matikan** mengendalikan LED di Wokwi.

> Klik sensor DHT22 di Wokwi lalu geser slider suhunya — angka di app ikut berubah!

## Rangkaian

| Kaki komponen | Ke pin ESP32 |
|---|---|
| DHT22 VCC | 3V3 |
| DHT22 SDA | 21 |
| DHT22 GND | GND |
| LED A (+, kaki panjang) | resistor 220Ω → 14 |
| LED C (−, kaki pendek) | GND |

## Topik MQTT

Broker: `broker.emqx.io` — ESP32 lewat port `1883`, browser lewat WebSocket `wss://broker.emqx.io:8084/mqtt`.

| Topik | Arah | Isi |
|---|---|---|
| `sman19/<kelompok>/suhu` | ESP32 → App | suhu °C, mis. `27.5` |
| `sman19/<kelompok>/lembap` | ESP32 → App | kelembapan %, mis. `65` |
| `sman19/<kelompok>/lampu` | App → ESP32 | `ON` / `OFF` |

> Broker ini publik dan gratis — **jangan kirim data pribadi**, dan pakai nama kelompok yang unik supaya tidak tercampur dengan kelompok lain.

## Troubleshooting

| Gejala | Solusi |
|---|---|
| Lama di "Waiting in queue" | Server build Wokwi antre; tunggu, atau login Wokwi. |
| `Menyambung WiFi.....` tidak selesai | Nama WiFi harus `Wokwi-GUEST`, password `""`. Arahkan mouse ke ikon WiFi di Wokwi untuk melihat error, lalu restart. |
| `PubSubClient.h: No such file` | Library belum ditambahkan di Library Manager. |
| App tersambung tapi angka `--` | Nama kelompok di app ≠ di kode ESP32. |
| Tombol app tidak berpengaruh | Pakai `modul3_kontrol_dari_app.ino`, bukan modul 2. |
| Suhu `nan` | Kabel SDA DHT22 harus ke pin 21. |
