public class Profesor implements Runnable{


    @Override
    public void run() {
        try {
            Testapd.semaphore.acquire();
            System.out.println("Profesorul poate parasi sala de curs impreuna cu elevi");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
