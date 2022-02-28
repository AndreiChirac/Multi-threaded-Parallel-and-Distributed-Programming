package oneProducerOneConsumer;

import java.util.concurrent.Semaphore;

public class Buffer {
    private int a;
    public Semaphore gol = new Semaphore(1);
    public Semaphore plin = new Semaphore(0);

    void put(int value) {
        try {
            gol.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        a = value;
        plin.release();
    }

    int get() {
        try {
            plin.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        int help = a;
        gol.release();

        return help;
    }
}
