package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyTask implements Runnable{
    int[] graph;
    int step;
    ExecutorService tpe;
    AtomicInteger inQueue;

    public MyTask(int[] graph, int step, ExecutorService tpe,
                  AtomicInteger inQueue) {
        this.graph = graph;
        this.step = step;
        this.tpe = tpe;
        this.inQueue = inQueue;
    }

    @Override
    public void run() {
        if (Main.N == step) {
            Main.printQueens(graph);

            int left = inQueue.decrementAndGet();
            if (left == 0) {
                tpe.shutdown();
            }

            return;
        }

        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (Main.check(newGraph, step)) {
                inQueue.incrementAndGet();
                tpe.submit(new MyTask(newGraph, step + 1, tpe, inQueue));
            }
        }

        int left = inQueue.decrementAndGet();
        if (left == 0) {
            tpe.shutdown();
        }
    }
}
