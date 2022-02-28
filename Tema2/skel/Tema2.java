import java.io.*;

public class Tema2 {

    public static void main(String[] args) throws IOException, InterruptedException {
        if (args.length < 3) {
            System.err.println("Usage: Tema2 <workers> <in_file> <out_file>");
            return;
        }

        Data inputData = new Data();
        inputData.setNrWorkers(Integer.parseInt(args[0]));
        inputData.setInputFile(args[1]);
        inputData.setOutputFile(args[2]);
        inputData.parsInputFile();
        inputData.workMap();
        inputData.workReduce();
        inputData.writeToOutputFile();
    }
}
