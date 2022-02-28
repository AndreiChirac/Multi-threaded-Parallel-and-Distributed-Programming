import java.util.Map;
import java.util.concurrent.BrokenBarrierException;

public class Mk implements Runnable{
    private final int id;
    private int mk_perechi;

    public int getMk_perechi() {
        return mk_perechi;
    }

    public void setMk_perechi(int mk_perechi) {
        this.mk_perechi = mk_perechi;
    }

    public Mk(int id) {
        this.id = id;
    }


    @Override
    public void run() {
        setMk_perechi(Testapd.random.nextInt(10 - 2 + 1) + 2);

        for (int i = 0; i < getMk_perechi(); i++) {
            int k_pereche = Testapd.random.nextInt(10 - 1 + 1) + 1;
            int v_pereche = Testapd.random.nextInt(100000 - 1 + 1) + 1;
            System.out.println( id + " : Am generat perechea (" + k_pereche + ", " + v_pereche + ")");

            try {
                Testapd.mutex.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            int citire = Testapd.pairs.get(k_pereche);

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            if (citire < v_pereche) {
                Testapd.pairs.replace(k_pereche, v_pereche);
            }

            Testapd.mutex.release();

        }

        try {
            Testapd.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        if (id == 0) {
            System.out.println("Final :");
            for(Map.Entry<Integer,Integer> entry : Testapd.pairs.entrySet()) {
                System.out.print( "(" + entry.getKey() + " " + entry.getValue() + ") ");
            }
            System.out.println();
        }

    }
}
