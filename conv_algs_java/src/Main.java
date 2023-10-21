import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import java.util.Scanner;

public class Main {

    public static void computeIndexedConv(int[][] input, int[][] kernel, int[][] answer, int p){
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
        List<Thread> threads = new ArrayList<>();
        for (int j = 0; j < p; j++) {

            flag = 0;
            if(remaining > 0){
                remaining--;
                flag=1;
            }

//            System.out.println(String.format("Thread %d starts at %d,%d, kLine %d -- %d lines", j, startX, startY, kLine, lines_per_thread + flag));

            threads.add(new indexThread(input, kernel, answer, startX, startY, kLine, lines_per_thread + flag));
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

    }


    public static void computeLineConv(int[][] input, int[][] kernel, int[][] answer, int p){
        int lines_per_thread = input.length / p;
        int remainder = input.length % p;
        int flag = 0;
        if( remainder > 0){
            flag = 1;
            --remainder;
        }
        int startLine = 0 ;

        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < p; i++) {
//            System.out.println(String.format("Thread %d started between the lines %d and %d", i, startLine, startLine + lines_per_thread + flag ));
            threads.add(new lineThread(input, kernel, answer, startLine, startLine + lines_per_thread + flag));
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

    public static void computeColumnConv(int[][] input, int[][] kernel, int[][] answer, int p){
        int columns_per_thread = input[0].length / p;
        int remainder = input[0].length % p;
        int flag = 0;
        if( remainder > 0){
            flag = 1;
            --remainder;
        }
        int startColumn = 0 ;

        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < p; i++) {
//            System.out.println(String.format("Thread %d started between the columns %d and %d", i, startColumn, startColumn + columns_per_thread + flag ));
            threads.add(new columnThread(input, kernel, answer, startColumn, startColumn + columns_per_thread + flag));
            threads.get(i).start();
            startColumn = startColumn + columns_per_thread + flag ;
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
           File inputFile = new File(args[2]);
           scanner = new Scanner(inputFile);
        } catch (FileNotFoundException e) {
           throw new RuntimeException(e);
        }
       int i_n = 0, i_m = 0;



       i_n = scanner.nextInt(10);
       i_m = scanner.nextInt(10);
       int[][] input = new int[i_n][i_m];
       int[][] answer = new int[i_n][i_m];
       for (int i = 0; i < i_n; i++) {
           for (int j = 0; j < i_m; j++) {
                input[i][j] = scanner.nextInt(10);
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

       // check for correct read
        System.out.println("--------------------Read check-----------------------");
       for (int i = 0; i < k_n; i++) {
           for (int j = 0; j < k_m; j++) {
               System.out.print(String.valueOf(kernel[i][j]) + ' ');
           }
           System.out.println();
       }

        int p = Integer.parseInt(args[0]);
        System.out.println("--------------------Start compute--------------------");
       //start the algorithm
        long startTime, endTime;
        if(args[1].compareTo("indexed") == 0){
            startTime = System.nanoTime();
            computeIndexedConv(input, kernel, answer, p);
            endTime = System.nanoTime();
        }else if (args[1].compareTo("lined") == 0){
            startTime = System.nanoTime();
            computeLineConv(input, kernel, answer, p);
            endTime = System.nanoTime();
        }else if(args[1].compareTo("columned") == 0){
            startTime = System.nanoTime();
            computeColumnConv(input, kernel, answer, p);
            endTime = System.nanoTime();
        }else{
            System.out.println("No algorithm found. Exiting...");
            return;
        }
        System.out.println((float)(endTime - startTime)/1000000);
//        for (int i = 0; i < input.length; i++) {
//            for (int j = 0; j < input[0].length; j++) {
//                System.out.print(String.format("%d ", answer[i][j]));
//            }
//            System.out.println();
//        }

    }
}
