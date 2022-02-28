import java.util.concurrent.CyclicBarrier;

public class Testapd {

    public static int size;
    public static int nr_of_threads;
    public static Integer []arr;
    public static Integer []cnt;
    public static Integer []arr_helper;
    public static CyclicBarrier barrier;

    public static void main(String[] args) {

        size = Integer.parseInt(args[0]);
        nr_of_threads = Integer.parseInt(args[1]);

        Thread[] rank_sort_thread = new Thread[nr_of_threads];
        barrier = new CyclicBarrier(nr_of_threads);

        arr = new Integer[size];
        cnt = new Integer[size];
        arr_helper = new Integer[size];

        for (int i = 0; i < size; i++) {
            arr[i] = size - i + 5;
            arr_helper[i] = arr[i];
            cnt[i] = 0;
            System.out.println(arr[i] + " ");
        }

        for (int i = 0; i < nr_of_threads; i++) {
            rank_sort_thread[i] = new Thread(new Rank_Sort(i));
            rank_sort_thread[i].start();
        }

        for (int i = 0; i < nr_of_threads; i++) {
            try {
                rank_sort_thread[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
