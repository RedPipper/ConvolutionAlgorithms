//
// Created by tefan on 10/21/23.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <cstring>

#define TEST_MODE

using namespace std;

void computeConv(int **in_matrix, int **kernel, int **out_matrix, int i_n, int i_m, int k_n, int k_m, int x, int y) {
    int sX = x - k_n / 2;
    int sY = y - k_m / 2;
    int eX = x + k_n / 2;
    int eY = y + k_m / 2;

    for (int i = sX, Ki = 0; i <= eX && Ki < k_n; ++i, ++Ki) {
        for (int j = sY, Kj = 0; j <= eY && Kj < k_m; ++j, ++Kj) {
            int definite_i = max(0, min(i, i_n - 1));
            int definite_j = max(0, min(j, i_m - 1));
            out_matrix[x][y] += in_matrix[definite_i][definite_j] * kernel[Ki][Kj];
        }
    }
}

void computeLinedConvThread(int **in_matrix, int **kernel, int **out_matrix, int i_n, int i_m, int k_n, int k_m,
                            int startLine, int endLine) {
    for (int i = startLine; i < endLine; ++i) {
        for (int j = 0; j < i_m; ++j) {
            computeConv(in_matrix, kernel, out_matrix, i_n, i_m, k_n, k_m, i, j);
        }
    }
}

void computeLinedConv(int p, int **in_matrix, int **kernel, int **out_matrix, int i_n, int i_m, int k_n, int k_m) {
    int lines_per_thread = i_n / p;
    int remainder = i_n % p;
    int flag = 0;


    int startLine = 0;
    thread threads[p];
    for (int i = 0; i < p; ++i) {
        flag = 0;
        if (remainder > 0) {
            flag = 1;
            --remainder;
        }
#ifndef TEST_MODE
        printf("Thread %d started between the lines %d and %d\n", i, startLine, startLine + lines_per_thread + flag);
#endif
        //start thread
        threads[i] = thread(&computeLinedConvThread, in_matrix, kernel, out_matrix, i_n, i_m, k_n, k_m, startLine,
                            startLine + lines_per_thread + flag);
        startLine = startLine + lines_per_thread + flag;
    }

    for (int i = 0; i < p; ++i) {
        threads[i].join();
    }
}

void computeColumnedConvThread(int **in_matrix, int **kernel, int **out_matrix, int i_n, int i_m, int k_n, int k_m,
                               int startCol, int endCol) {
    for (int j = startCol; j < endCol; ++j) {
        for (int i = 0; i < i_n; ++i) {
            computeConv(in_matrix, kernel, out_matrix, i_n, i_m, k_n, k_m, i, j);
        }
    }
}

void computeColumnConv(int p, int **in_matrix, int **kernel, int **out_matrix, int i_n, int i_m, int k_n, int k_m) {
    int columns_per_thread = i_m / p;
    int remainder = i_m % p;
    int flag = 0;


    int startColumn = 0;
    thread threads[p];
    for (int i = 0; i < p; ++i) {
        flag = 0;
        if (remainder > 0) {
            flag = 1;
            --remainder;
        }
#ifndef TEST_MODE
        printf("Thread %d started between the columns %d and %d\n", i, startColumn, startColumn + columns_per_thread + flag);
#endif
        //start thread
        threads[i] = thread(&computeColumnedConvThread, in_matrix, kernel, out_matrix, i_n, i_m, k_n, k_m, startColumn,
                            startColumn + columns_per_thread + flag);
        startColumn = startColumn + columns_per_thread + flag;
    }

    for (int i = 0; i < p; ++i) {
        threads[i].join();
    }
}

void
computeIndexedConvThread(int **in_matrix, int **kernel, int **out_matrix, int i_n, int i_m, int k_n, int k_m, int x,
                         int y, int kLine, int nrOfLines) {
    for (int j = 0; j < nrOfLines; j++) {
        if (kLine == k_n) {
            kLine = 0;
            int temp = (y + 1);
            y = temp % i_m;
            x += temp / i_m;
        }
        int compLine = x - (k_m / 2 - kLine);
        int startY = y - k_m / 2;
        int sum = 0;

        for (int i = 0; i < k_m; ++i) {
            sum += in_matrix[max(0, min(compLine, i_n - 1))][max(0, min(startY + i, i_m - 1))] * kernel[kLine][i];
        }

        out_matrix[x][y] += sum;
        kLine++;
    }
    int a = 1;
}

void computeIndexedConv(int p, int **in_matrix, int **kernel, int **out_matrix, int i_n, int i_m, int k_n, int k_m) {
    int nr_of_lines = i_n * i_m * k_n;
    int lines_per_thread = nr_of_lines / p;
    int remaining = nr_of_lines % p;
    int flag;

    int startX = 0, startY = 0, kLine = 0;
    thread threads[p];
    for (int i = 0; i < p; ++i) {
        flag = 0;
        if (remaining > 0) {
            remaining--;
            flag = 1;
        }
#ifndef TEST_MODE
        printf("Thread %d starts at %d,%d, kLine %d -- %d lines\n", i, startX, startY, kLine, lines_per_thread + flag);
#endif
        threads[i] = thread(&computeIndexedConvThread, in_matrix, kernel, out_matrix, i_n, i_m, k_n, k_m, startX,
                            startY, kLine, lines_per_thread + flag);

        int addP = (lines_per_thread + flag) / k_n;
        int addK = (lines_per_thread + flag) % k_n;

        int temp = startY + addP + (kLine + addK) / k_n;
        kLine = (kLine + addK) % k_n;

        startY = temp % i_m;
        startX += temp / i_m;
    }

    for (int i = 0; i < p; ++i) {
        threads[i].join();
    }
}

int main(int argc, char **argv) {

    ifstream fin(
            argv[3]);

    int i_n, i_m;
    fin >> i_n >> i_m;

    int **in_matrix = new int *[i_n];
    int **out_matrix = new int *[i_n];
    for (int i = 0; i < i_n; ++i) {
        in_matrix[i] = new int[i_m];
        out_matrix[i] = new int[i_n];
        for (int j = 0; j < i_m; ++j) {
            fin >> in_matrix[i][j];
            out_matrix[i][j] = 0;
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

#ifndef TEST_MODE
    cout << "------------------Read check-----------------\n";
    for (int i = 0; i < k_n; ++i) {
        for (int j = 0; j < k_m; ++j) {
            printf("%d ", kernel[i][j]);
        }
        cout << '\n';
    }
    cout << "------------------Compute start--------------\n";
#endif

    chrono::time_point<chrono::high_resolution_clock> start_timer;
    chrono::time_point<chrono::high_resolution_clock> end_timer;

    if(strcmp(argv[2], "indexed") == 0){
        start_timer = chrono::high_resolution_clock::now();
        computeIndexedConv(p, in_matrix, kernel, out_matrix, i_n, i_m, k_n, k_m);
        end_timer = chrono::high_resolution_clock::now();
    }else if(strcmp(argv[2], "lined") == 0){
        start_timer = chrono::high_resolution_clock::now();
        computeLinedConv(p, in_matrix, kernel, out_matrix, i_n, i_m, k_n, k_m);
        end_timer = chrono::high_resolution_clock::now();
    }else if(strcmp(argv[2], "columned") == 0){
        start_timer = chrono::high_resolution_clock::now();
        computeColumnConv(p, in_matrix, kernel, out_matrix, i_n, i_m, k_n, k_m);
        end_timer = chrono::high_resolution_clock::now();
    }else{
    	cout<<"ERROR, NO ALGORITHM FOUND\n";
	return 1;
    }



    auto duration = chrono::duration_cast<chrono::microseconds>(end_timer - start_timer);

#ifndef TEST_MODE
    cout << "------------------Compute end----------------\n";

    cout << '\n';


    for (int i = 0; i < i_n; ++i) {
        for (int j = 0; j < i_m; ++j) {
            printf("%d ", out_matrix[i][j]);
        }
        cout << '\n';
    }
#endif
    cout<<duration.count()<<'\n';
    return 0;
}
