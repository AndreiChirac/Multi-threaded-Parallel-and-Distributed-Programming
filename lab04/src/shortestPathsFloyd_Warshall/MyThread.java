package shortestPathsFloyd_Warshall;

import java.util.concurrent.BrokenBarrierException;

public class MyThread implements Runnable {
    public int start;
    public int end;
    int[][] graph;

    public MyThread(int start, int end, int[][] graph) {
        this.start = start;
        this.end = end;
        this.graph = graph;
    }

    @Override
    public void run() {
        // Parallelize me using P threads
        for (int k = 0; k < 5; k++) {
            try {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < 5; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);

                }
            }
            Main.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }

        }
    }
}
