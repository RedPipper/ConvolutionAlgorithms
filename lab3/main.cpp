#include <iostream>
#include <fstream>
#include <mpi.h>
#include <cstring>
#include <chrono>
using namespace std;

int main(int argc, char** argv) {

    chrono::time_point<chrono::high_resolution_clock> start_timer;
    chrono::time_point<chrono::high_resolution_clock> end_timer;

    int myid, defined_procs, numprocs, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    numprocs = (int) strtol(argv[1], nullptr, 10) ;

#ifdef TIME_T1
    long durations[numprocs-1];
#endif

    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &defined_procs);
    MPI_Get_processor_name(processor_name, &namelen);
    if(myid == 0){
#ifdef TIME_T2
        start_timer = chrono::high_resolution_clock::now();
#endif
        ifstream fin(argv[2]);
        int kSize[2];
        fin >> kSize[0] >> kSize[1];
        MPI_Bcast(kSize, 2, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < kSize[0]; ++i) {
            int aux[kSize[1]];
            for (int j = 0; j < kSize[1]; ++j) {
                fin >> aux[j];
            }
            MPI_Bcast(aux, kSize[1] , MPI_INT, 0, MPI_COMM_WORLD );
            MPI_Barrier(MPI_COMM_WORLD);
        }
        int i_n, i_m;
        fin >> i_n >> i_m;
        if( i_n % ( numprocs-1 ) == 0){
            int linesPerProcess = i_n / (numprocs - 1);
            int buffer[2] = {linesPerProcess, i_m};
            MPI_Bcast(buffer, 2 , MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);

            for(int p = 1; p < numprocs; ++p){
                int *auxBuffer =  new int[linesPerProcess * i_m];
                for (int i = 0; i < linesPerProcess; ++i) {
                    for (int j = 0; j < i_m; ++j) {
                        fin >> auxBuffer[i * i_m + j];
                    }
                }
                MPI_Send(auxBuffer, linesPerProcess * i_m, MPI_INT, p, 0, MPI_COMM_WORLD);
            }
            int numRecvs = 0;

            fstream fout(argv[3]);
            for(int i = 0; i < i_n; ++i){
                fout<<"\n";
            }
            fout.close();

            int outMatrix[i_n][i_m];
            while(numRecvs < numprocs-1){
                int *outBuffer =  new int[linesPerProcess * i_m];
                MPI_Status status;
                int recErr = MPI_Recv(outBuffer, linesPerProcess * i_m, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                                      &status);

#ifdef TIME_T1
                MPI_Recv(&durations[status.MPI_SOURCE-1], 1, MPI_LONG, status.MPI_SOURCE, 0, MPI_COMM_WORLD, nullptr);
#endif

                if(recErr == MPI_SUCCESS){
                    for(int i = 0; i < linesPerProcess; ++i){
                        for(int j = 0; j < i_m; ++j){
                            outMatrix[(status.MPI_SOURCE-1) * linesPerProcess + i][j] = outBuffer[i * i_m + j];
                        }
                    }
                    ++numRecvs;
                }else{
                    cout<<"ERROR\n";
                }

            }
            fstream outstream(argv[3] );
            for(int i = 0; i < i_n; ++i){
                for(int j = 0; j < i_m; ++j){
                    outstream << outMatrix[i][j]<< ' ';
                }
                outstream << '\n';
            }
            outstream.close();

        }else{
            int buffer[2] = {-1, -1};
            MPI_Bcast(buffer, 2 * sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
        }
        fin.close();

#ifdef TIME_T1
        long sumDur = 0;
            for(int i = 1; i < numprocs; ++i){
                sumDur += durations[i-1];
            }

            cout<<sumDur/(numprocs - 1)<<'\n';
#endif

#ifdef TIME_T2
        end_timer = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_timer - start_timer);
        cout<<duration.count()<<'\n';
#endif

    }else{
        int kSize[2] = {0, 0};
        MPI_Bcast(kSize, 2 , MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        int **kernel = new int*[kSize[0]];
        for(int i=0;i<kSize[0];++i){
            kernel[i] = new int[kSize[1]];
            MPI_Bcast(kernel[i], kSize[1], MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
        }

        int mSize[2];
        MPI_Bcast(mSize, 2 , MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        //error message, cant split matrix evenly
        if(mSize[0] != -1){
            int *base_matrix = new int[mSize[0] * mSize[1]];
            int *result_matrix = new int[mSize[0] * mSize[1]];
            MPI_Recv(base_matrix, mSize[0] * mSize[1] , MPI_INT, 0, 0, MPI_COMM_WORLD, nullptr);

            int *upper = new int[mSize[1]];
            int *lower = new int[mSize[1]];
            //recv and send to upper proc
            if(myid > 1){

                MPI_Recv(upper, mSize[1] , MPI_INT, myid - 1, 0, MPI_COMM_WORLD, nullptr);

                MPI_Send(base_matrix, mSize[1] , MPI_INT, myid - 1, 0, MPI_COMM_WORLD);

            }else{
                memcpy(upper, base_matrix, mSize[1] * sizeof(int));
            }

            if(myid < numprocs - 1 ){

                MPI_Send(base_matrix + (mSize[0]-1) * mSize[1], mSize[1], MPI_INT, myid + 1, 0, MPI_COMM_WORLD);

                MPI_Recv(lower, mSize[1] , MPI_INT, myid + 1, 0, MPI_COMM_WORLD, nullptr);

            }else{
                memcpy(lower, base_matrix + (int) ((mSize[0]-1) * mSize[1]), mSize[1] * sizeof(int));
            }

#ifdef TIME_T1
            start_timer = chrono::high_resolution_clock::now();
#endif
            for(int i = 0; i < mSize[0]; ++i){

                for(int j = 0; j < mSize[1]; ++j){
                    int si = i - kSize[0]/2;
                    int ei = i + kSize[0]/2;
                    int sj = j - kSize[1]/2;
                    int ej = j + kSize[1]/2;

                    int sum = 0;
                    for(int x = si, ik = 0; x <= ei && ik < kSize[0]; ++x, ++ik){
                        for(int y = sj, jk = 0; y <= ej && jk < kSize[1]; ++y, ++jk){
                            if(x == -1){
                                sum += upper[max(0, min(y, mSize[1] - 1))] * kernel[ik][jk];
                            }else if(x == mSize[0]){
                                sum += lower[max(0, min(y, mSize[1] - 1))] * kernel[ik][jk];
                            }else{
                                sum += base_matrix[x * mSize[1] + max(0, min(y, mSize[1] - 1))] * kernel[ik][jk];
                            }
                        }
                    }

                    result_matrix[i * mSize[1] + j] = sum;

                }
            }

#ifdef TIME_T1
            end_timer = chrono::high_resolution_clock::now();
#endif

            MPI_Send(result_matrix, mSize[0] * mSize[1], MPI_INT, 0, 0, MPI_COMM_WORLD);

#ifdef TIME_T1
            auto duration = chrono::duration_cast<chrono::microseconds>(end_timer - start_timer);
            long intDuration = duration.count();
            MPI_Send(&intDuration, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
#endif
        }
    }



    MPI_Finalize();
    return 0;
}
