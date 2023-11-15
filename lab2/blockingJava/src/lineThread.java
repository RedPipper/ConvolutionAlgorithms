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

    int computeNewConv(int[][] bufferLines, int[][] kernel, int pos){
        int sY = pos - kernel[0].length/2;
        int eY = pos + kernel[0].length/2;
        int sum = 0;
        for(int i=0; i < kernel.length; ++i){
            for(int j=0, bJ=sY; j < kernel[0].length && bJ <= eY; j++, bJ++){
                sum += bufferLines[i][max(0, min(bJ, bufferLines[0].length-1))] * kernel[i][j];
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
        int[] lastLineBuffer = new int[in_matrix[0].length];

        System.arraycopy(in_matrix[min(in_matrix.length - 1, endLine)], 0, lastLineBuffer, 0, in_matrix[0].length);

        int[] buffer1 = new int[in_matrix[0].length];
        System.arraycopy(in_matrix[max(0, startLine-1)], 0, buffer1, 0, in_matrix[0].length);

        try {
            barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            throw new RuntimeException(e);
        }

        int[] buffer2 = new int[in_matrix[0].length];
        System.arraycopy(in_matrix[startLine], 0, buffer2, 0, in_matrix[0].length);


        for(int i=startLine; i<endLine; ++i) {
            int[][] bufferLines;
            if (i < endLine - 1) {
                bufferLines = new int[][]{buffer1, buffer2, in_matrix[i + 1]};
            } else {
                bufferLines = new int[][]{buffer1, buffer2, lastLineBuffer};
            }

            for(int j=0; j < in_matrix[0].length; ++j){
                in_matrix[i][j] = computeNewConv(bufferLines, kernel, j);

            }

            //grabNextLineSnapshot
            if( i < endLine - 1){
                System.arraycopy(buffer2, 0, buffer1, 0, in_matrix[0].length);
                System.arraycopy(in_matrix[i+1], 0, buffer2, 0, in_matrix[0].length);

            }
        }

    }
}
