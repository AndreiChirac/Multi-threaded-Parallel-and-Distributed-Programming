package readersWriters.writerPriority;

import java.rmi.MarshalledObject;
import java.util.concurrent.BrokenBarrierException;

public class Writer extends Thread {
    private final int id;

    public Writer(int id) {
        super();
        this.id = id;
    }

    @Override
    public void run() {
        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        do {
            // TODO
            if (Main.currentReaders > 0 || Main.currentWriters > 0) {
                Main.waitingWriters++;
                Main.enter.release();
                try {
                    Main.sem_writer.acquire();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Main.currentWriters++;
            // TODO
            Main.enter.release();

            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Writer " + id + " is writing");
            Main.hasWritten[id] = true;

            try {
                Main.enter.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            // TODO
            Main.currentWriters--;
            if (Main.waitingReaders > 0 && Main.waitingWriters == 0 ) {
                Main.currentReaders--;
                Main.sem_reader.release();
            } else if (Main.waitingWriters > 0) {
                Main.waitingWriters--;
                Main.sem_writer.release();
            } else if (Main.waitingWriters == 0 && Main.waitingReaders == 0) {
                Main.enter.release();
            }
            // TODO

        } while (!Main.hasWritten[id]);
    }
}
