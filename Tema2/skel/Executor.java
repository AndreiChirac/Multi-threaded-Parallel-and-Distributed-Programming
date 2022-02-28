import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

public class Executor {
    public static AtomicInteger inQueue;
    public static ExecutorService tpe;
    private final int nrWorkers;
    private final ArrayList<TaskMap> taskMap;
    private ArrayList<TaskReduce> taskReduce;
    public static ArrayList<ResultTaskMap> resultsTaskMap;
    public static Map<Float, FinalResultTaskReduce> resultsTaskReduce;
    Map<String,FinalResultTaskMap> finalResultsTaskMap = new HashMap<>();
    public static Semaphore sem;
    public static Semaphore lock;

    public Executor(int nrWorkers, ArrayList<TaskMap> taskMap) {
        inQueue = new AtomicInteger(0);
        tpe = Executors.newFixedThreadPool(nrWorkers);
        resultsTaskMap = new ArrayList<>();
        resultsTaskReduce = new TreeMap<>(Collections.reverseOrder());
        sem = new Semaphore(- taskMap.size() + 1);
        lock = new Semaphore(1);
        this.nrWorkers = nrWorkers;
        this.taskMap = taskMap;
    }

    public void setTaskReduce(ArrayList<TaskReduce> taskReduce) {
        this.taskReduce = taskReduce;
    }

    public void newExecutor(){
        inQueue = new AtomicInteger(0);
        tpe = Executors.newFixedThreadPool(nrWorkers);
        sem = new Semaphore(- taskReduce.size() + 1);
        lock = new Semaphore(1);
    }

    /**
     * this method is used in order to save time on the requests that
     * have been received in the taskMap. So the operation are inserted
     * in a pool from which each thread will take one request and will
     * provide its answer in an array (results). We need to have all the
     * results in order to process the information so a semaphore that
     * will release() each time a thread will end the task will be the
     * solution because it can acquire only when all the tasks were finished
     * (- taskMap.size() + 1 ). After this we process the information
     * to advance the information to the next stage (Reduce).
     */
    public void runTaskMap() throws InterruptedException {
        // adding the tasks to the task pool
        for (TaskMap task : taskMap) {
            inQueue.incrementAndGet();
            tpe.submit(task);
        }
        // in case all the tasks have ended we can continue to our final step
        sem.acquire();

        // we take the result from each task and put it together on each file so the finalResultsTaskMap key will be the
        // name of the file and as the value all the data that has been processed for that file
        for (ResultTaskMap resultTaskMap : resultsTaskMap) {
            ArrayList<Map<Integer, Integer>> dictionaryFile = new ArrayList<>();
            if (finalResultsTaskMap.get(resultTaskMap.getFileName()) != null) {

                finalResultsTaskMap.get(resultTaskMap.getFileName()).getDictionary().add(resultTaskMap.getDictionary());

                for (int j = 0; j < resultTaskMap.getLongestWords().size(); j++) {
                    finalResultsTaskMap.get(resultTaskMap.getFileName()).getLongestWords().add(resultTaskMap.getLongestWords().get(j));
                }
            } else {
                dictionaryFile.add(resultTaskMap.getDictionary());
                FinalResultTaskMap finalResultTaskMap = new FinalResultTaskMap(dictionaryFile, resultTaskMap.getLongestWords());
                finalResultsTaskMap.put(resultTaskMap.getFileName(), finalResultTaskMap);
            }
        }
    }

    /**
     * this function works in the same manner as the runTaskMap() but instead of processing data for the Map stage it
     * will operate on taskReduce and as the name says it will provide the final result after the Reduce stage
     */
    public void runTaskReduce() throws InterruptedException {
        newExecutor();

        // adding the tasks to the task pool
        for (TaskReduce task : taskReduce) {
            inQueue.incrementAndGet();
            tpe.submit(task);
        }

        // in case all the tasks have ended we can continue to our final step
        sem.acquire();
    }
}
