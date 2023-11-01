//
// Created by tefan on 10/31/23.
//
#include <fstream>
#include <math.h>
#include <iostream>
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

void compute(int** in_matrix, int** kernel, int i_n, int i_m, int k_n, int k_m){
    int** tempMatrix = new int*[i_n];

    for(int i = 0; i < i_n; ++i){
        tempMatrix[i] = new int[i_m];
        for(int j = 0; j < i_m; ++j){
            tempMatrix[i][j] = computeConv(in_matrix, kernel, i_n, i_m, k_n, k_m, i, j);
        }
    }

    for(int i = 0; i < i_n; ++i){
        for(int j = 0; j < i_m; ++j){
            in_matrix[i][j] = tempMatrix[i][j];
        }
    }
}

int main(int argc, char** argv){

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);


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

    compute(in_matrix, kernel, i_n, i_m, k_n, k_m);

    for (int i = 0; i < i_n; ++i) {
        for (int j = 0; j < i_m; ++j) {
            fout << in_matrix[i][j] << ' ';
        }
        fout<<'\n';
    }

    return 0;
}
