import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import static java.lang.Math.max;
import static java.lang.Math.min;

public class lineThread extends Thread{

    int[][] in_matrix, kernel;
    int startLine, endLine;
    CyclicBarrier barrier;

    int computeConv(int x, int y){

        int sX = x - kernel.length/2;
        int sY = y - kernel[0].length/2;
        int eX = x + kernel.length/2;
        int eY = y + kernel[0].length/2;
        int sum = 0;
        for (int i = sX, Ki=0; i <= eX && Ki < kernel.length; i++, Ki++) {
            for (int j = sY, Kj = 0; j <= eY && Kj < kernel[0].length; j++, Kj++) {
                sum += in_matrix[max(0, min(i, in_matrix.length-1))][max(0, min(j, in_matrix[0].length-1))] * kernel[Ki][Kj];
            }
        }

        return sum;
    }

    public lineThread(int[][] in_matrix, int[][] kernel, int startLine, int endLine, CyclicBarrier barrier){
        this.in_matrix = in_matrix;
        this.kernel = kernel;
        this.startLine = startLine;
        this.endLine = endLine;
        this.barrier = barrier;
    }


    @Override
    public void run(){

        int[] buffer = new int[in_matrix[0].length * (endLine - startLine)];

        for(int i = startLine; i < endLine; ++i){
            for(int j = 0; j < in_matrix[0].length; ++j){
                buffer[(i-startLine)*in_matrix[0].length + j] = computeConv(i,j);
            }
        }
        try {
            barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            throw new RuntimeException(e);
        }

        for(int i = startLine; i < endLine; ++i){
            for(int j = 0; j < in_matrix[0].length; ++j){
                in_matrix[i][j] = buffer[(i - startLine)*in_matrix[0].length + j];
            }
        }

    }
}
