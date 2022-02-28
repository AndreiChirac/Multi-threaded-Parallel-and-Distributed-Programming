import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Map;
import java.util.Scanner;

public class Data {
    private int nrWorkers;
    private int octetSize;
    private String inputFile;
    private String outputFile;
    private ArrayList<String> files = new ArrayList<>();
    private ArrayList<TaskMap> taskMap = new ArrayList<>();
    private final ArrayList<TaskReduce> taskReduce = new ArrayList<>();
    private Executor executor;

    public void setNrWorkers(int nrWorkers) {
        this.nrWorkers = nrWorkers;
    }

    public void setTaskMap(ArrayList<TaskMap> taskMap) {
        this.taskMap = taskMap;
    }

    public ArrayList<String> getFiles() {
        return files;
    }

    public void setFiles(ArrayList<String> files) {
        this.files = files;
    }

    public void setInputFile(String inputFile) {
        this.inputFile = inputFile;
    }

    public void setOutputFile(String outputFile) {
        this.outputFile = outputFile;
    }

    /**
     * method that allows us to parse the input given
     * octetSize (number of octets that we are allowed
     * to read at once), nrFiles (number of files that
     * we have to process)
     */
    public void parsInputFile() throws IOException {
        File myObj = new File(inputFile);
        Scanner myReader = new Scanner(myObj);

        octetSize = Integer.parseInt(myReader.nextLine());
        int nrFiles = Integer.parseInt(myReader.nextLine());

        for (int i = 0; i < nrFiles; i++) {
            files.add(myReader.nextLine());
        }
    }

    /**
     * method that allows us to put in the output file
     * the information we need
     */
    public void writeToOutputFile () throws IOException {
        FileWriter myWriter = new FileWriter(outputFile);

        for (Map.Entry<Float,FinalResultTaskReduce> entry : Executor.resultsTaskReduce.entrySet()){
            String[] arrOfStr = entry.getValue().getFileName().split("/");
            myWriter.write(arrOfStr[arrOfStr.length - 1] + "," + entry.getKey() +
                    "," + entry.getValue().getMaxLength() + "," + entry.getValue().getApp() + "\n");

        }

        myWriter.close();
    }

    /**
     * starts the execution process on Map operations on
     * multiple threads
     */
    public void workMap() throws InterruptedException, IOException {
        createMapWorkers();
        executor = new Executor(nrWorkers, taskMap);
        executor.runTaskMap();
    }

    /**
     * starts the execution process on Reduce operations on
     * multiple threads
     */
    public void workReduce() throws InterruptedException {
        createReduceWorkers();
        executor.runTaskReduce();
    }

    /**
     * this method creates the data that a worker will use
     * in the Reduce operation
     */
    public void createReduceWorkers(){
        for (Map.Entry<String,FinalResultTaskMap> entry : executor.finalResultsTaskMap.entrySet()) {
            taskReduce.add(new TaskReduce(entry.getKey(), entry.getValue()));
        }
        executor.setTaskReduce(taskReduce);
    }

    /**
     * this method creates the data that a worker will use
     * in the Map operation
     */
    public void createMapWorkers() throws IOException {
        Fragmentation fragmentation = new Fragmentation(octetSize);
        setTaskMap(fragmentation.fragment(files));
    }

}
