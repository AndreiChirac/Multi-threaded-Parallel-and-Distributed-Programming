import java.util.Map;
import java.util.Random;
import java.util.TreeMap;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

public class Testapd {
    public static int nr_of_threads;
    public static Random random;
    public static Map<Integer, Integer> pairs = new TreeMap<Integer,Integer>();
    public static Semaphore mutex = new Semaphore(1);
    public static CyclicBarrier barrier;

    public static void main(String[] args) {
        nr_of_threads = Integer.parseInt(args[0]);

        Thread[] mk = new Thread[nr_of_threads];
        barrier = new CyclicBarrier(nr_of_threads);
        random = new Random();

        for (int i = 1; i < 11; i++){
            pairs.put(i, 0);
        }

        for (int i = 0; i < nr_of_threads; i++) {
            mk[i] = new Thread(new Mk(i));
            mk[i].start();
        }

        for (int i = 0; i < nr_of_threads; i++) {
            try {
                mk[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
