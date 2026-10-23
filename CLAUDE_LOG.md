# Log Kerja Claude — Pertemuan 4 (Smart Room ESP32)

Ringkasan pembuatan materi & kode ini bersama Claude Code, 23 Oktober 2026.

## Permintaan

Materi Pertemuan 4 "Integrasi Software & Hardware" yang langsung hands-on, simpel tapi seru: simulasi ESP32 + aplikasi buatan sendiri, PDF berdesain dengan screenshot asli, lalu kode di-upload ke GitHub publik supaya siswa tinggal clone.

## Pilihan yang diputuskan pengajar

| Pilihan | Diputuskan | Alternatif yang ditolak |
|---|---|---|
| Simulator + app | **Wokwi (ESP32) + MQTT + web app HTML** | Wokwi + Blynk (perlu akun/token per siswa), TinkerCad (tidak ada WiFi) |
| Bentuk PDF | **A4 portrait** gaya panduan (header navy, aksen kuning/biru) | Landscape 16:9 ala slide |
| Isi repo | **Kode Pertemuan 4 + README + PDF** | Semua pertemuan, atau kode saja |
| Nama repo | **smart-room-esp32** | ekskul-pemrograman-sman19 |

## Yang dibuat

- `modul1_sensor.ino` — DHT22 → Serial Monitor, LED menyala kalau suhu > 30°C.
- `modul2_kirim_ke_app.ino` — WiFi + publish suhu/kelembapan ke MQTT.
- `modul3_kontrol_dari_app.ino` — dua arah: publish suhu + subscribe perintah `ON`/`OFF` lewat callback, `millis()` menggantikan `delay()`.
- `diagram.json`, `libraries.txt` — rangkaian & library Wokwi.
- `app.html` — web app "Smart Room" (mqtt.js via WebSocket).
- `materi_pertemuan4.pdf` — modul 9 halaman (dibuat dari HTML lewat headless Chrome).

## Keputusan teknis (dan alasannya)

- **Broker `broker.emqx.io`**, bukan HiveMQ: saat diuji, HiveMQ port 1883 timeout dari jaringan pengujian; EMQX berhasil di port 1883 (ESP32) dan 8084/WSS (browser).
- **Pin LED 14, DHT22 pin 21** (bukan 2/15): supaya kabel di rangkaian tidak bersilangan dan screenshot rapi.
- **`WiFi.begin(..., "", 6)`**: channel 6 dianjurkan dokumentasi Wokwi, melewati scan WiFi (±4 detik lebih cepat). Percobaan pertama tanpa channel sempat gagal dengan pesan "IoT Gateway ... unexpectedly closed the connection"; setelah restart + channel 6 berhasil.
- **Topik unik per kelompok** (`sman19/<kelompok>/...`): broker publik dipakai banyak orang.

## Hasil verifikasi (dijalankan sungguhan di Wokwi + browser)

| Modul | Hasil |
|---|---|
| 1 | Slider DHT22 digeser ke 43,7°C → LED menyala, Serial Monitor mencatat. ✅ |
| 2 | Serial: `tersambung!` → `MQTT tersambung!` → `Terkirim -> suhu 43.70 C`. Subscriber di terminal menerima `suhu 43.7`, `lembap 65`; web app menampilkan angka yang sama. ✅ |
| 3 | Tombol **Nyalakan** di app → LED Wokwi menyala, Serial: `Perintah dari app: ON`. ✅ Tombol **Matikan** ditekan tetapi hasilnya tidak dicek ulang. |

Semua screenshot di PDF diambil dari run di atas (bukan mockup).

## Catatan untuk mengajar

- **Antrean build Wokwi** sempat sampai posisi #21. Satu kelas menekan ▶ bersamaan bisa antre lebih lama; login Wokwi memberi 1× skip gratis per hari.
- **Kecepatan simulasi** saat pengujian hanya ±3% (WiFi butuh ±1 menit real-time). Kemungkinan karena browser sedang dikendalikan otomatis; belum dipastikan. Coba sekali di laptop kelas sebelum hari-H.
- Karena simulasi lambat, ESP32 sempat **reconnect MQTT** sekali (keepalive terlewati) — tidak mengganggu, perintah lampu tetap diterima.

## Ide lanjutan (belum dikerjakan)

- Tautan PDF Pertemuan 1–3 / repo gabungan semua pertemuan.
- Hosting `app.html` (mis. GitHub Pages) supaya siswa bisa buka dari link tanpa download file.
