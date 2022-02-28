import java.util.Arrays;
import java.util.Collections;
import java.util.concurrent.BrokenBarrierException;

import static java.lang.Math.log;

public class Sheer implements Runnable {
    private final int id;
    private final int start;
    private final int end;

    public Sheer(int id) {
        super();
        this.id = id;
        this.start = id * Testapd.size / Testapd.nr_of_threads;
        this.end = Math.min((id + 1) * Testapd.size / Testapd.nr_of_threads, Testapd.size);
    }

    @Override
    public void run() {
        Integer [] aux = new Integer[Testapd.size];

        for (int k = 0; k < log(Testapd.size) + 1; k++) {
            for (int i = start; i < end; i++) {
                if(i % 2 == 1){
                    Arrays.sort(Testapd.matrix[i], Collections.reverseOrder());
                } else {
                    Arrays.sort(Testapd.matrix[i]);
                }
            }
            if (id == 0) {
                System.out.println("INSIDE PE LINII:");
                for (int i = 0; i < Testapd.size; i++) {
                    for (int j = 0; j < Testapd.size; j++) {
                        System.out.printf(Testapd.matrix[i][j] + " ");
                    }
                    System.out.println();
                }
            }

            try {
                Testapd.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }

            for (int i = start; i < end; i++) {
                for (int j = 0; j < Testapd.size; j++) {
                    aux[j] = Testapd.matrix[j][i];
                }

                Arrays.sort(aux);

                for (int j = 0; j < Testapd.size; j++) {
                    Testapd.matrix[j][i] = aux[j];
                }
            }

            try {
                Testapd.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }

            if (id == 0) {
                System.out.println("INSIDE PE COLOANE:");
                for (int i = 0; i < Testapd.size; i++) {
                    for (int j = 0; j < Testapd.size; j++) {
                        System.out.printf(Testapd.matrix[i][j] + " ");
                    }
                    System.out.println();
                }
            }

        }
    }
}
