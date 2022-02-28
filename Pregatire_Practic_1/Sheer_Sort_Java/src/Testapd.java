import java.util.Scanner;
import java.util.concurrent.CyclicBarrier;

public class Testapd {

    public static int size;
    public static int nr_of_threads;
    public static Integer[][] matrix;
    public static CyclicBarrier barrier;

    public static void main(String[] args) {
        size = Integer.parseInt(args[0]);
        nr_of_threads = Integer.parseInt(args[1]);
        matrix = new Integer[size][size];

        Scanner myObj = new Scanner(System.in);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrix[i][j] = myObj.nextInt();
            }
        }

        System.out.println("BEFOR :");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                System.out.printf(matrix[i][j] + " ");
            }
            System.out.println();
        }

        Thread[] sheer = new Thread[nr_of_threads];
        barrier = new CyclicBarrier(nr_of_threads);

        for (int i = 0; i < nr_of_threads; i++) {
            sheer[i] = new Thread(new Sheer(i));
            sheer[i].start();
        }

        for (int i = 0; i < nr_of_threads; i++) {
            try {
                sheer[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("AFTER :");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                System.out.printf(matrix[i][j] + " ");
            }
            System.out.println();
        }
    }
}
