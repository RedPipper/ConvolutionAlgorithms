public class colThread extends Thread {

    int nrOfLines;
    int[][] in_matrix;
    int[][] kernel;
    int[][] out_matrix;


    int x,y, kCol;
    public colThread(int[][] A, int[][] K, int[][] C, int x, int y, int kCol, int nrOfLines){
        this.in_matrix = A;
        this.kernel = K;
        this.out_matrix = C;

        this.x = x;
        this.y = y;
        this.kCol = kCol;
        this.nrOfLines = nrOfLines;
    }

    @Override
    public void run(){

        for (int j = 0; j < nrOfLines; j++) {
            if(kCol == kernel.length){
                kCol = 0;
                int temp = (x+1);
                x = temp % in_matrix.length;
                y += temp / in_matrix.length;
            }
            int compCol = y - (kernel.length/2 - kCol);
            int startX = x - kernel.length/2;
            int sum = 0;

            for(int i = 0; i < kernel.length;++i){
                if(compCol >= 0 && compCol < in_matrix[0].length && startX + i >= 0 && startX + i < in_matrix[0].length){
                    sum += in_matrix[startX + i][compCol] * kernel[i][kCol];
                }
            }

            out_matrix[x][y] += sum;
            kCol++;
        }

    }

}
