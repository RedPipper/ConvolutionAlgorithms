import static java.lang.Math.max;
import static java.lang.Math.min;

public class columnThread extends Thread {

    int[][] in_matrix, kernel, out_matrix;
    int startColumn, endColumn;

    void computeConv(int x, int y){

        int sX = x - kernel.length/2;
        int sY = y - kernel[0].length/2;
        int eX = x + kernel.length/2;
        int eY = y + kernel[0].length/2;

        for (int i = sX, Ki=0; i < eX && Ki < kernel.length; i++, Ki++) {
            for (int j = sY, Kj = 0; j < eY && Kj < kernel[0].length; j++, Kj++) {
                out_matrix[x][y] += in_matrix[max(0, min(i, in_matrix.length-1))][max(0, min(j, in_matrix[0].length-1))] * kernel[Ki][Kj];
            }
        }

    }

    public columnThread(int[][] in_matrix, int[][] kernel, int[][] out_matrix, int startColumn, int endColumn) {
        this.in_matrix = in_matrix;
        this.kernel = kernel;
        this.out_matrix = out_matrix;
        this.startColumn = startColumn;
        this.endColumn = endColumn;
    }

    @Override
    public void run(){
        for(int i = 0; i < in_matrix.length; ++i){
            for(int j = startColumn; j < endColumn; ++j){
                computeConv(j, i);
            }
        }
    }
}
