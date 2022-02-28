package doubleVectorElements;

public class MyThread implements Runnable {
    public int start;
    public int end;
    int[] v;

    public MyThread(int start, int end, int[] v) {
        this.start = start;
        this.end = end;
        this.v = v;
    }

    @Override
    public void run() {
        // Parallelize me using P threads
        for (int i = start; i < end; i++) {
            v[i] = v[i] * 2;
        }
    }
}
