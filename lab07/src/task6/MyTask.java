package task6;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class MyTask  extends RecursiveTask<Void> {
    int[] graph;
    int step;
    List<MyTask> tasks;

    public MyTask(int[] graph, int step) {
        this.graph = graph;
        this.step = step;
        this.tasks = new ArrayList<MyTask>();;

    }

    @Override
    protected Void compute() {
        if (Main.N == step) {
            Main.printQueens(graph);
            return null;
        }
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (Main.check(newGraph, step)) {
                //queens(newGraph, step + 1);
                MyTask t = new MyTask(newGraph,step + 1);
                tasks.add(t);
                t.fork();
            }
        }

        for (MyTask task : tasks) {
            task.join();
        }

        return null;
    }
}
