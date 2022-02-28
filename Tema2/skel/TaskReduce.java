import java.util.Collections;
import java.util.Map;
import java.util.TreeMap;

public class TaskReduce implements Runnable {
    private final String fileName;
    private final FinalResultTaskMap finalResultFromTaskMap;
    Map<Integer,Integer> appearanceLen = new TreeMap<>(Collections.reverseOrder());
    private int fiboSum;
    private int totalApp;

    public TaskReduce(String fileName, FinalResultTaskMap finalResultFromTaskMap) {
        this.fileName = fileName;
        this.finalResultFromTaskMap = finalResultFromTaskMap;
        this.fiboSum = 0;
        this.totalApp = 0;
    }

    /**
     * function that calculates what is the n th number in Fibonacci series
     * @param n n th number in Fibonacci series
     * @return what is the n th number in Fibonacci series
     */
    static int fib(int n)
    {
        if (n <= 1)
            return n;
        return fib(n-1) + fib(n-2);
    }

    /**
     * method that will take care of the Reduce stage by processing the strings in the wanted manner
     */
    @Override
    public void run() {

        // in order to get the longest word and calculate the rang we should iterate through each of the dictionary
        // provided by the Merge tasks for this file, in order to do this in fiboSum we will store the sum off al words
        // converted by the given formula and in totalApp we will store the numbers of words that were found in the file
        for(int i = 0; i < finalResultFromTaskMap.getDictionary().size(); i++) {
            for (Map.Entry<Integer, Integer> inside : finalResultFromTaskMap.getDictionary().get(i).entrySet()) {
                appearanceLen.merge(inside.getKey(), inside.getValue(), Integer::sum);
                fiboSum += fib(inside.getKey() + 1) * inside.getValue();
                totalApp += inside.getValue();
            }
        }

        // this map is already sorted in descending order so we will need only the first element from it because that
        // will indicate what is the length of the longest word and how many times words with such length had appeared
        // in our file
        Map.Entry<Integer,Integer> entry = appearanceLen.entrySet().iterator().next();
        String helper = String.format("%.2f", (double)fiboSum/totalApp);

        // because there are many threads that process information in the same time it might occur some problems in this
        // region because of the add operation, so we will treat this portion of the code as a critical region
        try {
            Executor.lock.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        Executor.resultsTaskReduce.put(Float.parseFloat(helper),
                new FinalResultTaskReduce(fileName, entry.getKey(), entry.getValue()));
        Executor.lock.release();

        // this semaphore is used in order to know when all the tasks have ended in order to get the final result
        // based on each task results
        Executor.sem.release();

        // left indicates how many tasks are left so in case there is no task that require to be done we can let the
        // threads rest :)
        int left = Executor.inQueue.decrementAndGet();
        if (left == 0) {
            Executor.tpe.shutdown();
        }
    }
}
