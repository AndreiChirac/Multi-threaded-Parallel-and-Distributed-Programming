public class Elevi implements Runnable {
    private final int id;

    public Elevi(int id) {
        this.id = id;
    }

    @Override
    public void run() {

        System.out.println("Elevul cu id-ul " + this.id + " a luat un servetel");
        Testapd.semaphore.release();
    }
}
