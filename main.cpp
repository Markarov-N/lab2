#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>
#include <omp.h>
using namespace std;
using namespace std::chrono;

bool readMatrix(const string& filename, vector<vector<double>>& matrix, int& n) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return false;
    }
    fin >> n;
    matrix.assign(n, vector<double>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            fin >> matrix[i][j];
    fin.close();
    return true;
}

void writeMatrix(const string& filename, const vector<vector<double>>& matrix, int n) {
    ofstream fout(filename);
    fout << n << "\n";
    fout << fixed << setprecision(6);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fout << matrix[i][j];
            if (j < n - 1) fout << " ";
        }
        fout << "\n";
    }
    fout.close();
}

void multiplyMatrices(const vector<vector<double>>& A,
                      const vector<vector<double>>& B,
                      vector<vector<double>>& C, int n, int numThreads) {
    C.assign(n, vector<double>(n, 0.0));
    omp_set_num_threads(numThreads);

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            double a_ik = A[i][k];
            for (int j = 0; j < n; ++j) {
                C[i][j] += a_ik * B[k][j];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    string fileA = "matrix_a.txt";
    string fileB = "matrix_b.txt";
    string fileOut = "result_cpp.txt";
    int numThreads = 4;
    int benchMaxSize = 0;

    if (argc >= 4) { fileA = argv[1]; fileB = argv[2]; fileOut = argv[3]; }
    if (argc >= 5) { numThreads = stoi(argv[4]); }
    if (argc >= 6) { benchMaxSize = stoi(argv[5]); }

    // --- Режим 1: умножение матриц из файлов ---
    vector<vector<double>> A, B, C;
    int nA, nB;
    if (!readMatrix(fileA, A, nA)) return 1;
    if (!readMatrix(fileB, B, nB)) return 1;
    if (nA != nB) {
        cerr << "Ошибка: матрицы разного размера (" << nA << " vs " << nB << ")" << endl;
        return 1;
    }
    int n = nA;
    cout << "Размер матриц: " << n << " x " << n << endl;
    cout << "Потоков: " << numThreads << endl;

    auto start = high_resolution_clock::now();
    multiplyMatrices(A, B, C, n, numThreads);
    auto end = high_resolution_clock::now();
    double timeSec = duration_cast<microseconds>(end - start).count() / 1e6;

    long long operations = 2LL * n * n * n;
    double gflops = (operations / 1e9) / timeSec;
    double memoryMB = 3.0 * n * n * sizeof(double) / (1024.0 * 1024.0);

    writeMatrix(fileOut, C, n);
    cout << "Время: " << fixed << setprecision(6) << timeSec << " сек" << endl;
    cout << "Память: " << memoryMB << " МБ" << endl;
    cout << "GFLOPS: " << setprecision(2) << gflops << endl;
    cout << "Результат в: " << fileOut << endl;

    // --- Режим 2: бенчмарк ---
    if (benchMaxSize > 0) {
        cout << "\n=== БЕНЧМАРК ===" << endl;
        cout << "Size,Threads,Time_sec,GFLOPS" << endl;

        int sizes[] = {200, 400, 800, 1200, 1600, 2000};
        int threads[] = {1, 2, 4, 8};

        for (int sz : sizes) {
            if (sz > benchMaxSize) break;
            for (int nt : threads) {
                vector<vector<double>> tA(sz, vector<double>(sz, 1.5));
                vector<vector<double>> tB(sz, vector<double>(sz, 2.0));
                vector<vector<double>> tC;

                auto t1 = high_resolution_clock::now();
                multiplyMatrices(tA, tB, tC, sz, nt);
                auto t2 = high_resolution_clock::now();
                double tSec = duration_cast<microseconds>(t2 - t1).count() / 1e6;
                long long ops = 2LL * sz * sz * sz;
                double gf = (ops / 1e9) / tSec;

                cout << sz << "," << nt << "," << fixed << setprecision(6) << tSec
                     << "," << setprecision(2) << gf << endl;
            }
        }
    }
    return 0;
}