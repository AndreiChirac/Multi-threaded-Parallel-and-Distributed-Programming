package EX1;


public class Main {
    public static void main(String[] args) {
        int numberCores = Runtime.getRuntime().availableProcessors();
        Thread[] threads = new Thread[numberCores];

        for (int i = 0; i < numberCores; i++) {
            threads[i] = new Thread(new EX1.MyThread(i));
            threads[i].start();
        }

        for (int i = 0; i < numberCores; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
