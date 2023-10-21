//
// Created by tefan on 10/21/23.
//
#include <iostream>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <math.h>
#define INPUT_N     10000
#define INPUT_M     10000
#define KERNEL_N    1000
#define KERNEL_M    1000

int input_matrix[INPUT_N][INPUT_M];
int i_n, i_m;
int kernel[KERNEL_N][KERNEL_M];
int k_n, k_m;
int out_matrix[INPUT_N][INPUT_N];

using namespace std;

void computeConv(int x, int y){
    int sX = x - k_n/2;
    int sY = y - k_m/2;
    int eX = x + k_n/2;
    int eY = y + k_m/2;

    for ( int i = sX, Ki = 0; i <= eX && Ki < k_n; ++i, ++Ki){
        for( int j = sY, Kj = 0; j <= eY && Kj < k_m; ++j, ++Kj){
            int definite_i = max( 0 , min(i, i_n-1) );
            int definite_j = max( 0 , min(j, i_m-1) );
            out_matrix[x][y] += input_matrix[  definite_i ][ definite_j ] * kernel[Ki][Kj];
        }
    }
}

void computeIndexedConvThread(int x, int y, int kLine, int nrOfLines){
    for (int j = 0; j < nrOfLines; j++){
        if(kLine == k_n){
            kLine = 0;
            int temp = (y+1);
            y = temp % i_m;
            x += temp / i_m;
        }
        int compLine = x - (k_m/2 - kLine);
        int startY = y - k_m/2;
        int sum = 0;

        for( int i = 0; i < k_m; ++i){
            sum += input_matrix[ max(0, min(compLine, i_n - 1))] [ max(0, min(startY+i , i_m - 1)) ] * kernel[kLine][i];
        }

        out_matrix[x][y] += sum;
        kLine++;
    }
    int a = 1;
}

void computeIndexedConv(int p){
    int nr_of_lines = i_n * i_m * k_n;
    int lines_per_thread = nr_of_lines / p;
    int remaining = nr_of_lines % p;
    int flag;

    int startX = 0, startY = 0, kLine = 0;
    thread threads[p];
    for ( int i = 0; i < p; ++i){
        flag = 0;
        if(remaining > 0){
            remaining--;
            flag = 1;
        }
        printf("Thread %d starts at %d,%d, kLine %d -- %d lines\n", i, startX, startY, kLine, lines_per_thread + flag);
        threads[i] = thread(&computeIndexedConvThread, startX, startY, kLine, lines_per_thread + flag );

        int addP = (lines_per_thread + flag ) / k_n;
        int addK = (lines_per_thread + flag ) % k_n;

        int temp = startY + addP + (kLine + addK ) / k_n;
        kLine = (kLine + addK ) % k_n;

        startY = temp%i_m;
        startX += temp/i_m;



    }

    for (int i = 0; i < p; ++i) {
        threads[i].join();
    }
}

void computeLinedConvThread(int startLine, int endLine){
    for(int i = startLine; i < endLine; ++i){
        for (int j = 0; j < i_m; ++j) {
            computeConv(i,j);
        }
    }
}

void computeColumnedConvThread(int startCol, int endCol){
    for(int j = startCol; j < endCol; ++j){
        for(int i = 0; i < i_n;++i){
            computeConv(i,j);
        }
    }
}

void computeLinedConv(int p){
    int lines_per_thread = i_n / p;
    int remainder = i_n % p;
    int flag = 0;


    int startLine = 0;
    thread threads[p];
    for (int i = 0; i < p; ++i) {
        flag = 0;
        if ( remainder > 0 ){
            flag = 1;
            --remainder;
        }
        printf("Thread %d started between the lines %d and %d\n", i, startLine, startLine + lines_per_thread + flag);
        //start thread
        threads[i] = thread(&computeLinedConvThread, startLine, startLine + lines_per_thread + flag);
        startLine = startLine + lines_per_thread + flag;
    }

    for (int i = 0; i < p; ++i) {
        threads[i].join();
    }
}

void computeColumnConv(int p){
    int columns_per_thread = i_m / p;
    int remainder = i_m%p;
    int flag = 0;


    int startColumn = 0;
    thread threads[p];
    for (int i = 0; i < p; ++i) {
        flag = 0;
        if ( remainder > 0 ){
            flag = 1;
            --remainder;
        }
        printf("Thread %d started between the columns %d and %d\n", i, startColumn, startColumn + columns_per_thread + flag);
        //start thread
        threads[i] = thread(&computeColumnedConvThread, startColumn,  startColumn + columns_per_thread + flag);
        startColumn = startColumn + columns_per_thread + flag;
    }

    for (int i = 0; i < p; ++i) {
        threads[i].join();
    }
}

int main(int argc, char** argv){

    ifstream fin("/home/tefan/Facultate/Programare Paralela si Distribuita/Homework/ConvolutionAlgorithms/conv_algs_cpp/static_allocation/data.txt");

    fin>>i_n>>i_m;
    for (int i = 0; i < i_n; ++i) {
        for (int j = 0; j < i_m; ++j) {
            fin>>input_matrix[i][j];
        }
    }

    fin>>k_n>>k_m;
    int p;
    for (int i = 0; i < k_n; ++i) {
        for (int j = 0; j < k_m; ++j) {
            fin>>kernel[i][j];
        }
    }
    fin>>p;
    fin.close();
    cout<<"------------------Read check-----------------\n";
    for (int i = 0; i < k_n; ++i) {
        for (int j = 0; j < k_m; ++j) {
            printf("%d ", kernel[i][j]);
        }
        cout<<'\n';
    }
    cout<<"------------------Compute start--------------\n";
//    computeIndexedConv(p);
//    computeLinedConv(p);
    computeColumnConv(p);
    cout<<'\n';
    for (int i = 0; i < i_n; ++i) {
        for (int j = 0; j < i_m; ++j) {
            printf("%d ", out_matrix[i][j]);
        }
        cout<<'\n';
    }
    return 0;
}