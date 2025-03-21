#include <iostream>
#include <time.h>
#include <cstdlib>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    const int size = 100000000; // Increased size to observe performance differences
    int *v1 = new int[size];
    int *v2 = new int[size];
    int *v3 = new int[size];

    randomVector(v1, size);
    randomVector(v2, size);

    int chunk_sizes[] = {10, 100, 1000, 10000};
    for (int chunk : chunk_sizes) {
        auto start = high_resolution_clock::now();
        #pragma omp parallel for schedule(static, chunk) default(none) shared(size, v1, v2, v3, chunk)
        for (int i = 0; i < size; i++) {
            v3[i] = v1[i] + v2[i];
        }
        auto end = high_resolution_clock::now();
        cout << "Static scheduling (chunk = " << chunk << ") time: " << duration<double>(end - start).count() << " seconds" << endl;
    }
    
    for (int chunk : chunk_sizes) {
        auto start = high_resolution_clock::now();
        #pragma omp parallel for schedule(dynamic, chunk) default(none) shared(size, v1, v2, v3, chunk)
        for (int i = 0; i < size; i++) {
            v3[i] = v1[i] + v2[i];
        }
        auto end = high_resolution_clock::now();
        cout << "Dynamic scheduling (chunk = " << chunk << ") time: " << duration<double>(end - start).count() << " seconds" << endl;
    }
    
    for (int chunk : chunk_sizes) {
        auto start = high_resolution_clock::now();
        #pragma omp parallel for schedule(guided, chunk) default(none) shared(size, v1, v2, v3, chunk)
        for (int i = 0; i < size; i++) {
            v3[i] = v1[i] + v2[i];
        }
        auto end = high_resolution_clock::now();
        cout << "Guided scheduling (chunk = " << chunk << ") time: " << duration<double>(end - start).count() << " seconds" << endl;
    }
    
    auto start = high_resolution_clock::now();
    #pragma omp parallel for schedule(auto) default(none) shared(size, v1, v2, v3)
    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i];
    }
    auto end = high_resolution_clock::now();
    cout << "Auto scheduling time: " << duration<double>(end - start).count() << " seconds" << endl;
    
    delete[] v1;
    delete[] v2;
    delete[] v3;

    return 0;
}
