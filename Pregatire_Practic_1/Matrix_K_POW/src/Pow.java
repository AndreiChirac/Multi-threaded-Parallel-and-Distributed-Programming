import java.util.concurrent.BrokenBarrierException;

public class Pow implements Runnable {
    private final int id;
    private final int start;
    private final int end;

    public Pow(int id) {
        super();
        this.id = id;
        this.start = id * Testapd.N / Testapd.nr_of_threads;
        this.end = Math.min((id + 1) * Testapd.N / Testapd.nr_of_threads, Testapd.N);
    }

    @Override
    public void run() {
        for (int pow = 0; pow < Testapd.K - 1; pow++) {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < Testapd.N; j++) {
                    Testapd.matrix_result[i][j] = 0;
                    for (int k = 0; k < Testapd.N; k++) {
                        Testapd.matrix_result[i][j] += Testapd.matrix[i][k] * Testapd.matrix_helper[k][j];
                    }
                }
            }

            for (int i = start; i < end; i++){
                for (int j = 0; j < Testapd.N; j++) {
                    Testapd.matrix[i][j] = Testapd.matrix_result[i][j];
                }
            }

            try {
                Testapd.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
        }


        if (id == 0) {
            for (int i = 0; i < Testapd.N; i++) {
                for (int j = 0; j < Testapd.N; j++) {
                    System.out.print(Testapd.matrix_result[i][j] + " ");
                }
                System.out.println();
            }
        }

    }
}
