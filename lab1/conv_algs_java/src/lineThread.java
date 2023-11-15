import static java.lang.Math.max;
import static java.lang.Math.min;

public class lineThread extends Thread{

    int[][] in_matrix, kernel, out_matrix;
    int startLine, endLine;


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

    public lineThread(int[][] in_matrix, int[][] kernel, int[][] out_matrix, int startLine, int endLine){
        this.in_matrix = in_matrix;
        this.out_matrix = out_matrix;
        this.kernel = kernel;
        this.startLine = startLine;
        this.endLine = endLine;
    }


    @Override
    public void run(){
        for(int i = startLine; i < endLine; ++i){
            for(int j = 0; j < in_matrix[0].length; ++j){
                computeConv(i,j);
            }
        }
    }
}
