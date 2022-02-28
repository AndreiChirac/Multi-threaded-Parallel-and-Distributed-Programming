package EX1;

public class MyThread implements Runnable {
    public int id;

    public MyThread (int i){
        id = i;
    }

    @Override
    public void run() {
            System.out.println("Hello from thread #" + id);
    }
}
