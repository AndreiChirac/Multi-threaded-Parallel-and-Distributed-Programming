package shortestPathsFloyd_Warshall;


import java.util.concurrent.CyclicBarrier;

public class Main {
    public static CyclicBarrier barrier;

    public static void main(String[] args) {
        int M = 9;
        int[][] graph = {{0, 1, M, M, M},
                {1, 0, 1, M, M},
                {M, 1, 0, 1, 1},
                {M, M, 1, 0, M},
                {M, M, 1, M, 0}};
        int P = 3; // the program should work for any P <= N
        Thread[] threads = new Thread[P];
        barrier = new CyclicBarrier(P);
        /*
        // Parallelize me (You might want to keep the original code in order to compare)
        for (int k = 0; k < 5; k++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                System.out.print(graph[i][j] + " ");
            }
            System.out.println();
        }
        */
        System.out.println("==========================================================");

        for (int i = 0; i < P; i++) {

            threads[i] = new Thread(new MyThread(i * 5 / P, (i + 1) * 5 / P, graph));
            threads[i].start();
        }

        for (int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                System.out.print(graph[i][j] + " ");
            }
            System.out.println();
        }
    }
}
