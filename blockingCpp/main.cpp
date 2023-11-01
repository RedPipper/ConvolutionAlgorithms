//
// Created by tefan on 10/21/23.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <cstring>
#include <barrier>
#define TEST_MODE

using namespace std;

int computeConv(int **in_matrix, int **kernel, int i_n, int i_m, int k_n, int k_m, int x, int y) {
    int sX = x - k_n / 2;
    int sY = y - k_m / 2;
    int eX = x + k_n / 2;
    int eY = y + k_m / 2;
    int sum = 0;
    for (int i = sX, Ki = 0; i <= eX && Ki < k_n; ++i, ++Ki) {
        for (int j = sY, Kj = 0; j <= eY && Kj < k_m; ++j, ++Kj) {
            int definite_i = max(0, min(i, i_n - 1));
            int definite_j = max(0, min(j, i_m - 1));
            sum += in_matrix[definite_i][definite_j] * kernel[Ki][Kj];
        }
    }
    return sum;
}

void
computeLinedConvThread(int **in_matrix, int **kernel, int i_n, int i_m, int k_n, int k_m, int startLine, int endLine,
                       barrier<>* thread_barr) {

    int* buffer = new int[(endLine - startLine) * i_m];
    for (int i = startLine; i < endLine; ++i) {
        for (int j = 0; j < i_m; ++j) {
            buffer[(i-startLine) * i_n + j] = computeConv(in_matrix, kernel, i_n, i_m, k_n, k_m, i, j);
        }
    }

    thread_barr->arrive_and_wait();
    for (int i = startLine; i < endLine; ++i) {
        for (int j = 0; j < i_m; ++j) {
            in_matrix[i][j] = buffer[(i-startLine) * i_n + j] ;
        }
    }

}

void computeLinedConv(int p, int **in_matrix, int **kernel, int i_n, int i_m, int k_n, int k_m) {
    int lines_per_thread = i_n / p;
    int remainder = i_n % p;
    int flag = 0;


    int startLine = 0;
    thread threads[p];
    barrier barr(p);
    for (int i = 0; i < p; ++i) {
        flag = 0;
        if (remainder > 0) {
            flag = 1;
            --remainder;
        }

        //start thread
        threads[i] = thread(&computeLinedConvThread, in_matrix, kernel, i_n, i_m, k_n, k_m, startLine,
                            startLine + lines_per_thread + flag, &barr);
        startLine = startLine + lines_per_thread + flag;
    }

    for (int i = 0; i < p; ++i) {
        threads[i].join();
    }
}


int main(int argc, char **argv) {

    ifstream fin(
            argv[2]);


    int i_n, i_m;
    fin >> i_n >> i_m;

    int **in_matrix = new int *[i_n];
    for (int i = 0; i < i_n; ++i) {
        in_matrix[i] = new int[i_m];
        for (int j = 0; j < i_m; ++j) {
            fin >> in_matrix[i][j];
        }
    }

    int k_n, k_m;
    fin >> k_n >> k_m;
    int **kernel = new int *[k_n];
    for (int i = 0; i < k_n; ++i) {
        kernel[i] = new int[k_m];
        for (int j = 0; j < k_m; ++j) {
            fin >> kernel[i][j];
        }
    }
    fin.close();
    int p;
    p = (int) strtol(argv[1], nullptr, 10);

    chrono::time_point<chrono::high_resolution_clock> start_timer;
    chrono::time_point<chrono::high_resolution_clock> end_timer;


    start_timer = chrono::high_resolution_clock::now();
    computeLinedConv(p, in_matrix, kernel, i_n, i_m, k_n, k_m);
    end_timer = chrono::high_resolution_clock::now();


    auto duration = chrono::duration_cast<chrono::microseconds>(end_timer - start_timer);
    if ( argc == 4){
        ofstream  fout(argv[3]);
        for (int i = 0; i < i_n; ++i) {
            for (int j = 0; j < i_m; ++j) {
                fout << in_matrix[i][j] << ' ';
            }
            fout<<'\n';
        }
	fout.close();
    }

    cout<<duration.count()<<'\n';
    return 0;
}
