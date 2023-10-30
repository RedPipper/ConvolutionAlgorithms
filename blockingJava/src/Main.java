import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import java.util.Scanner;
import java.util.concurrent.CyclicBarrier;

public class Main {


    public static void computeLineConv(int[][] input, int[][] kernel, int p){
        int lines_per_thread = input.length / p;
        int remainder = input.length % p;
        int flag = 0;
        if( remainder > 0){
            flag = 1;
            --remainder;
        }
        int startLine = 0 ;

        List<Thread> threads = new ArrayList<>();
        CyclicBarrier barrier = new CyclicBarrier(p);
        for (int i = 0; i < p; i++) {
//            System.out.println(String.format("Thread %d started between the lines %d and %d", i, startLine, startLine + lines_per_thread + flag ));
            threads.add(new lineThread(input, kernel, startLine, startLine + lines_per_thread + flag, barrier));
            threads.get(i).start();
            startLine = startLine + lines_per_thread + flag ;
            flag = 0;
            if( remainder > 0){
                flag = 1;
                --remainder;
            }

        }
        for (int i = 0; i < p; i++) {
            try {
                threads.get(i).join();
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }

    }



    public static void main(String[] args) {

        Scanner scanner = null;
        try{
           File inputFile = new File(args[0]);
           scanner = new Scanner(inputFile);
        } catch (FileNotFoundException e) {
           throw new RuntimeException(e);
        }
       int i_n = 0, i_m = 0;

       i_n = scanner.nextInt(10);
       i_m = scanner.nextInt(10);
       int[][] input = new int[i_n][i_m];
       for (int i = 0; i < i_n; i++) {
           for (int j = 0; j < i_m; j++) {
                input[i][j] = scanner.nextInt(10);
           }
       }

       int k_n = 0, k_m = 0;
       k_n = scanner.nextInt();
       k_m = scanner.nextInt();

       int[][] kernel = new int[k_n][k_m];
       for (int i = 0; i < k_n; i++) {
           for (int j = 0; j < k_m; j++) {
               kernel[i][j] = scanner.nextInt();
           }
       }

       int p = Integer.parseInt(args[1]);
       long startTime, endTime;

       startTime = System.nanoTime();
       computeLineConv(input, kernel, p);
       endTime = System.nanoTime();

        for (int i = 0; i < i_n; i++) {
            for (int j = 0; j < i_m; j++) {
                System.out.print(String.format("%d ", input[i][j]));
            }
            System.out.println();
        }
       System.out.println((endTime - startTime)/1000);

    }
}
