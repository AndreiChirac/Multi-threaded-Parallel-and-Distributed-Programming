package barber;

public class Barber extends Thread {
    @Override
    public void run() {
        int servedClients = 0;

        do {
            // TODO

            try {
                Main.clients_sem.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            try {
                Main.chairs_sem.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            Main.chairs++;

            // TODO
            Main.barber_ready_sem.release();
            Main.chairs_sem.release();

            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            System.out.println("Barber served client");
            ++servedClients;

        } while (checkIfThereAnyClients());
    }

    private boolean checkIfThereAnyClients() {
        int count = 0;
        for (var client: Main.leftClients) {
            if (client == 0) {
                count++;
            }
        }

        return count != 0;
    }
}
