import java.io.*;
import java.util.ArrayList;

public class Fragmentation {

    private final int octetSize;

    public Fragmentation(int octetSize) {
        this.octetSize = octetSize;
    }

    /**
     * the method is based on the idea that we iterate through a list of files
     * processing each file in order to create information to be used in Map
     * operations. So we open the file and while we didn't get to the end of
     * it we read octetSize (a given number of octets), in the same time we
     * fallow where we are in the file by using an offset (we increment it
     * with the number of bytes we read), we add it to our list of tasks and
     * carry on in the same manner.
     * @param files name of the file from where we will read data
     * @return a list that contains information used in Map operation
     */
    public ArrayList<TaskMap> fragment(ArrayList<String> files) throws IOException {
        ArrayList<TaskMap> taskMap = new ArrayList<>();

        int bytesRead;
        int offset;
        char[] myBuffer = new char[octetSize];

        for (String file : files) {
            offset = 0;

            BufferedReader in = new BufferedReader(new FileReader(file));

            while ((bytesRead = in.read(myBuffer, 0, octetSize)) != -1) {
                TaskMap newTask = new TaskMap(file, offset, bytesRead);
                taskMap.add(newTask);
                offset += bytesRead;
            }
        }
        return taskMap;
    }

}
