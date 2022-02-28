import java.util.concurrent.Semaphore;

public class Testapd {

    public static int nr_elevi;
    public static int nr_of_threads;
    public static Semaphore semaphore;

    public static void main(String[] args) {

        nr_elevi = Integer.parseInt(args[0]);
        nr_of_threads = nr_elevi;
        semaphore = new Semaphore(- nr_elevi + 1);
        Thread[] persoane = new Thread[nr_of_threads + 1];

        for (int i = 0; i < nr_of_threads + 1; i++) {
            if (i == nr_of_threads) {
                persoane[i] = new Thread(new Profesor());
            } else {
                persoane[i] = new Thread(new Elevi(i));
            }
            persoane[i].start();
        }

        for (int i = 0; i < nr_of_threads + 1; i++) {
            try {
                persoane[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
