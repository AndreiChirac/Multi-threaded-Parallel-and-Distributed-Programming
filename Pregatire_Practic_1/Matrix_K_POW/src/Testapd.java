import java.util.concurrent.CyclicBarrier;

public class Testapd {
    public static int N;
    public static int nr_of_threads;
    public static int K;
    public static long matrix[][];
    public static long matrix_helper[][];
    public static long matrix_result[][];

    public static CyclicBarrier barrier;
    private static long start;
    private static long end;

    public static void main(String[] args) {
        start = System.currentTimeMillis();

        N = Integer.parseInt(args[0]);
        nr_of_threads = Integer.parseInt(args[1]);
        K = Integer.parseInt(args[2]);

        Thread[] pow = new Thread[nr_of_threads];
        barrier = new CyclicBarrier(nr_of_threads);


        matrix = new long[N][N];
        matrix_helper = new long[N][N];
        matrix_result = new long[N][N];

        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++) {
                matrix[i][j] = 1 + i * N + j;
                matrix_helper[i][j] = 1 + i * N + j;
                matrix_result[i][j] = 0;
                System.out.print(matrix[i][j] + " ");
            }
            System.out.println();
        }

        for (int i = 0; i < nr_of_threads; i++) {
            pow[i] = new Thread(new Pow(i));
            pow[i].start();
        }

        for (int i = 0; i < nr_of_threads; i++) {
            try {
                pow[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        end = System.currentTimeMillis();
        System.out.println("\n" + (end-start) + " ms");

    }
}
