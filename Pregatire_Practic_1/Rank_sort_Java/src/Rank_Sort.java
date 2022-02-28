
import java.util.concurrent.BrokenBarrierException;

public class Rank_Sort implements Runnable{
    private final int id;
    private final int start;
    private final int end;

    public Rank_Sort(int id) {
        super();
        this.id = id;
        this.start = id * Testapd.size / Testapd.nr_of_threads;
        this.end = Math.min((id + 1) * Testapd.size / Testapd.nr_of_threads, Testapd.size);

    }

    @Override
    public void run() {
        for (int i = start; i < end; i++) {
            for (int j = 0; j < Testapd.size; j++){
                if (Testapd.arr[j] < Testapd.arr[i]) {
                    Testapd.cnt[i] = Testapd.cnt[i] + 1;
                }
            }
        }

        try {
            Testapd.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        if (id == 0) {
            System.out.println("Cnt for each elem :\n");
            for (int i = 0; i < Testapd.size; i++) {
                System.out.println(Testapd.cnt[i] + " ");
            }
            System.out.println();
        }

        for (int i = start; i < end; i++) {
            Testapd.arr[Testapd.cnt[i]] = Testapd.arr_helper[i];
        }

        try {
            Testapd.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        if (id == 0) {
            System.out.println("Sorted arr:\n");
            for (int i = 0; i < Testapd.size; i++) {
                System.out.println(Testapd.arr[i] + " ");
            }
            System.out.println();
        }
    }
}
