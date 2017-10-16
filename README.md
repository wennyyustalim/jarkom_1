# Tugas Besar 01 - IF3130 Jaringan Komputer

# Sliding Window

Simulasi proses pengiriman data yang terjadi dari sebuah program ke program lainnya menggunakan bahasa pemrograman C++ dan Socket UDP.

## Kelompok

K2-17 HabisUTS

* 13515002 Wenny Yustalim
* 13515071 Daniel Pintara
* 13515086 Muhammad Iqbal Al Khowarizmi

## Petunjuk Penggunaan Program

## Cara Kerja Sliding Window Kami

### Fungsi x

Deskripsi fungsi x

### Fungsi y

Deskripsi fungsi y

## Answers to Questions

### Question: Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani​ ​ hal​ ​ tersebut?

### Answer:
Advertised window yang bernilai 0 menandakan bahwa penerima sedang tidak dapat menerima informasi dari pengirim sehingga koneksi TCP dihentikan untuk sementara waktu hingga penerima dapat memroses informasi yang ada di dalam buffer-nya.

Advertised window adalah besar informasi yang dapat diterima suatu mesin dalam satu sesi koneksi TCP. Ketika sebuah mesin menginisiasi koneksi dengan server, ia akan memberi tahu server, melalui advertised window, berapa besar informasi yang dapat ia terima.

Ketika koneksi TCP dilakukan dan server mulai mengirim penerima data, penerima akan mengurangi ukuran advertised window-nya bersamaan dengan terisinya buffer penerima. Di saat yang bersamaan, penerima juga memroses data yang ada di dalam buffernya dan mengosongkannya, membuat ruang untuk data selanjutnya. Penerima memberi tahu server berapa banyak data yang masih bisa ia tampung di dalam buffernya melalui informasi advertised window yang ia kirim bersama dengan ACK. Jika advertised window yang dikirim bernilai 0, itu berarti buffer penerima sedang penuh dan tidak dapat menerima informasi lebih banyak sehingga server perlu menghentikan pengiriman informasi untuk sementara waktu, hingga penerima dapat membuat ruang di buffer-nya. Hal ini terjadi ketika laju informasi yang diterima klien dari server lebih tinggi dari kemampuan klien memroses informasi. 

Untuk mengatasinya, server dapat mengirimkan satu byte segmen TCP secara periodik kepada penerima untuk memancing respons dari penerima. Ketika buffer penerima sudah siap untuk menerima data lagi, ia akan mengirimkan ACK dari segmen TCP yang dikirim, bersamaan dengan informasi ukuran advertised window yang baru.

### Question: Sebutkan field data yang terdapat TCP Header serta ukurannya, ilustrasikan, dan jelaskan​ ​ kegunaan​ ​ dari​ ​ masing-masing​ ​ field​ ​ data​ ​ tersebut!

### Answer:

jwbn soal 2
