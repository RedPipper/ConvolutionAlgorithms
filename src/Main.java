import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {

    public static void computeConv(int[][] input, int[][] kernel, int[][] answer, int p){
        int i_n = input.length, i_m = input[0].length, k_n = kernel.length, k_m = kernel[0].length;
        int nr_of_lines = i_n * i_m * k_n;
        int lines_per_thread = nr_of_lines / p;
        int remaining = nr_of_lines % p;
        int flag = 0;
        if(remaining > 0){
            remaining--;
            flag=1;
        }

        int startX=0, startY=0, kLine=0;
        List<lineThread> threads = new ArrayList<>();
        for (int j = 0; j < p; j++) {

            flag = 0;
            if(remaining > 0){
                remaining--;
                flag=1;
            }

            System.out.println(String.format("Thread %d starts at %d,%d, kLine %d -- %d lines", j, startX, startY, kLine, lines_per_thread + flag));

            threads.add(new lineThread(input, kernel, answer, startX, startY, kLine, lines_per_thread + flag));
            threads.get(j).start();
            int addP = (lines_per_thread + flag) / kernel.length;
            int addK = (lines_per_thread + flag) % kernel.length;
            kLine = (kLine + addK) % kernel.length;

            int temp = startY + addP;
            startY = temp%input[0].length;
            startX += temp/input[0].length;


        }

        for (int i = 0; i < p; i++) {
            try {
                threads.get(i).join();
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }

        for (int i = 0; i < input.length; i++) {
            for (int j = 0; j < input[0].length; j++) {
                System.out.print(String.format("%d ", answer[i][j]));
            }
            System.out.println();
        }

    }


    public static void main(String[] args) {
       try{
           File inputFile = new File("/Users/snastasa/IdeaProjects/LabPPD/src/data.txt");
           Scanner scanner = new Scanner(inputFile);

           int i_n = 0, i_m = 0;


           i_n = scanner.nextInt();
           i_m = scanner.nextInt();
           int[][] inM = new int[i_n][i_m];
           int[][] answer = new int[i_n][i_m];
           for (int i = 0; i < i_n; i++) {
               for (int j = 0; j < i_m; j++) {
                    inM[i][j] = scanner.nextInt();
                    answer[i][j] = 0;
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

           // check for read
           for (int i = 0; i < k_n; i++) {
               for (int j = 0; j < k_m; j++) {
                   System.out.print(String.valueOf(kernel[i][j]) + ' ');
               }
               System.out.println();
           }

            int p = scanner.nextInt();

           computeConv(inM, kernel, answer, p);


       } catch (FileNotFoundException e) {
           throw new RuntimeException(e);
       }
    }
}