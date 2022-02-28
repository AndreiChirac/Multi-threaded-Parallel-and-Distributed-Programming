package barber;

public class Client extends Thread {
    private final int id;

    public Client(int id) {
        super();
        this.id = id;
    }

    @Override
    public void run() {
        // TODO
        try {
            Main.chairs_sem.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        if (Main.chairs > 0) {
            // client occupies a seat
            Main.chairs--;

            System.out.println("Client " + id + " is waiting for haircut");
            System.out.println("Available seats: " + Main.chairs);

            Main.clients_sem.release();
            Main.chairs_sem.release();
            try {
                Main.barber_ready_sem.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            // TODO

            System.out.println("Client " + id + " is served by the barber");

            Main.leftClients[id] = Main.SERVED_CLIENT;
        } else {
            // TODO
            Main.chairs_sem.release();
            System.out.println("Client " + id + " left unserved");
            Main.leftClients[id] = Main.UNSERVED_CLIENT;
        }
    }
}
