package multipleProducersMultipleConsumersNBuffer;

import java.lang.invoke.SwitchPoint;
import java.util.Queue;
import java.util.concurrent.Semaphore;

public class Buffer {
    
    Queue<Integer> queue;
    public Semaphore gol;
    public Semaphore plin;

    public Buffer(int size) {
        queue = new LimitedQueue<>(size);
        this.gol = new Semaphore(size);
        this.plin = new Semaphore(0);
    }

	public void put(int value) {
        try {
            gol.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        synchronized (Main.class) {
            queue.add(value);
        }
        plin.release();
	}

	public int get() {
        try {
            plin.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        int a = -1;
        synchronized (Main.class) {
            Integer result = queue.poll();
            if (result != null) {
                a = result;
            }
        }
        gol.release();
        return a;
	}
}
