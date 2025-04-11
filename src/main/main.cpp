#include <iostream>
#include <string>
#include <algorithm>
#include "../Compressor/Compressor.hpp"
#include "../utils/image_io.hpp"
#include <cmath>
using namespace std;

int main() {
    string inputPath, outputPath, gifPath;
    int method, minBlockSize;
    double threshold, targetCompressionRatio;

    cout << "=================================" << endl;
    cout << "=         TUCIL 2 STIMA         =" << endl;
    cout << "=  Quadtree Image Compression   =" << endl;
    cout << "=        By: Reza Ahmad S       =" << endl;
    cout << "=================================" << endl;

    cout << endl;

    cout << "Masukkan alamat absolut gambar input: ";
    getline(cin, inputPath);

    if (!isValidExtension(inputPath)) {
        cerr << "[ERROR] Format file tidak valid. Gunakan gambar dengan extensi jpg, jpeg, atau png" << endl;
        return 1;
    }

    cout << "Pilih metode perhitungan error (1: Variance, 2: MAD, 3: MaxDiff, 4: Entropy): ";
    cin >> method;

    if (method < 1 || method > 4) {
        cerr << "[ERROR] Metode tidak valid. Pilih antara 1 hingga 4." << endl;
        return 1;
    }

    switch (method) {
        case 1: cout << "[INFO] Threshold Variance: 0 - "<< pow(255,2) << "\n"; break;
        case 2: cout << "[INFO] Threshold MAD: 0 - 127.5\n"; break;
        case 3: cout << "[INFO] Threshold MaxDiff: 0 - 255\n"; break;
        case 4: cout << "[INFO] Threshold Entropy: 0 - 8\n"; break;
    }

    cout << "Masukkan nilai threshold: ";
    cin >> threshold;

    cin.ignore(); 

    bool thresholdValid = false;
    switch (method) {
        case 1: thresholdValid = (threshold >= 0 && threshold <= pow(255, 2)); break;
        case 2: thresholdValid = (threshold >= 0 && threshold <= 127.5); break;
        case 3: thresholdValid = (threshold >= 0 && threshold <= 255); break;
        case 4: thresholdValid = (threshold >= 0 && threshold <= 8); break;
    }
    
    if (!thresholdValid) {
        cerr << "[ERROR] Threshold di luar batas untuk metode ini.\n";
        return 1;
    }

    cout << "Masukkan ukuran blok minimum (1 - 64): ";
    cin >> minBlockSize;

    if (minBlockSize < 1 || minBlockSize > 64) {
        cerr << "[ERROR] Ukuran blok minimum tidak valid. Harus antara 1 hingga 64." << endl;
        return 1;
    }

    cout << "Masukkan target persentase kompresi (1.0 = 100%, 0 = nonaktif): ";
    cin >> targetCompressionRatio;
    if (targetCompressionRatio < 0.0 || targetCompressionRatio > 1.0) {
        cerr << "[ERROR] Target kompresi tidak valid. Harus antara 0 hingga 1." << endl;
        return 1;
    }

    cin.ignore(); 

    cout << "Masukkan alamat absolut gambar output: ";
    getline(cin, outputPath);
    if (!isValidExtension(outputPath)) {
        cerr << "[ERROR] Format file tidak valid. Gunakan gambar dengan extensi jpg, jpeg, atau png" << endl;
        return 1;
    }
    
    if (getFileExtension(outputPath) != getFileExtension(inputPath)) {
        cerr << "[ERROR] Format file output harus sama dengan format file input." << endl;
        return 1;
    }

    cout << "\n==========================" << endl;
    
    Compressor compressor(inputPath.c_str(), outputPath.c_str(), method, threshold, minBlockSize, targetCompressionRatio);

    cout << "[INFO] Jumlah channel gambar: " << compressor.getChannels() << endl;


    compressor.compressImage();

    return 0;
}
