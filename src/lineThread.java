public class lineThread extends Thread{

    int nrOfLines;
    int[][] in_matrix;
    int[][] kernel;
    int[][] out_matrix;


    int x,y,kLine;
    public lineThread(int[][] A, int[][] K, int[][] C, int x, int y, int kLine, int nrOfLines){
       this.in_matrix = A;
       this.kernel = K;
       this.out_matrix = C;

       this.x = x;
       this.y = y;
       this.kLine = kLine;
       this.nrOfLines = nrOfLines;
    }

    @Override
    public void run(){

        for (int j = 0; j < nrOfLines; j++) {
            if(kLine == kernel.length){
                kLine = 0;
                int temp = (y+1);
                y = temp % in_matrix[0].length;
                x += temp / in_matrix[0].length;
            }
            int compLine = x - (kernel[0].length/2 - kLine);
            int startY = y - (kernel[0].length/2);
            int sum = 0;

            for(int i = 0; i < kernel[0].length;++i){
                if(compLine >= 0 && compLine < in_matrix.length && startY+i>=0 && startY+i<in_matrix[0].length){
                    sum += in_matrix[compLine][startY + i] * kernel[kLine][i];
                }
            }

            out_matrix[x][y] += sum;
            kLine++;
        }

    }
}
