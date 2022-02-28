import java.io.*;
import java.util.*;

public class TaskMap implements Runnable {
    private final String fileName;
    private final int offset;
    private final int dimension;
    private int longestLen;

    public TaskMap(String fileName, int offset, int dimension) {
        this.fileName = fileName;
        this.offset = offset;
        this.dimension = dimension;
        longestLen = -1;
    }

    /**
     * method that will take care of the Map stage by processing the strings in the wanted manner
     */
    @Override
    public void run() {

        Map<Integer, Integer> dictionary = new HashMap<>();
        ArrayList<String> longestWords = new ArrayList<>();
        // word is used for storing the sequence to be processed
        byte[] word = new byte[dimension];
        // beforeWord is used to see if the starting word from this sequence has ended or not
        byte[] beforeWord = new byte[1];
        // afterWord is used to see if the last word from this sequence has ended or not
        byte[] afterWord = new byte[1];

        // we read the information using the given offset from the file that was attributed to this task
        try {
            RandomAccessFile raf = new RandomAccessFile(fileName, "r");
            // the first word in a file will not have a beforeWord
            if(offset > 0) {
                // read in beforeWord the last character from the previous sequence
                raf.seek(offset - 1);
                raf.read(beforeWord);
            } else {
                beforeWord[0] = ' ';
            }
            // read in afterWord the first character from the next sequence
            raf.seek(offset + dimension);
            raf.read(afterWord);

            // read in word the current sequence
            raf.seek(offset);
            raf.read(word);
        } catch (IOException e) {
            e.printStackTrace();
        }
        String separators = ";:/?~\\.,><`[]{}()!@#$%^&-_+'=*\"| \t\r\n";
        // we will create our ward letter by letter
        StringBuilder helper = new StringBuilder();
        for (int i = 0; i < dimension; i++){
            // this part consists of two things combined:
            // - if the word is the fist word from our current sequence it can be part of another word, so we have to
            // check if the previous sequence has ended in that case we can concatenate the letter and keep going until
            // we get a separator that will let us know that there is the end of the word and the character that
            // indicates what was previous will be changed
            // - if the word is a regular word that is placed between separators
            if ((Character.isLetter((char)word[i]) || Character.isDigit((char)word[i]))
                    && separators.indexOf((char)beforeWord[0]) != -1) {
                helper.append((char) word[i]);
            } else if (!Character.isLetter((char)word[i])){
                if(helper.length() != 0){
                    finishResult(dictionary, longestWords, helper);
                }
                helper = new StringBuilder();
                beforeWord[0] = word[i];
            }
        }
        // in case we have reached the last word from a sequence, there are two cases :
        // - the word might end there
        // - the word might be continued in another sequence so in this case we will read until we will reach a
        // separator because that will indicate our end
        if(helper.length() != 0 ){
            if (afterWord[0] != 0) {
                try {
                    BufferedReader bufferedReader = new BufferedReader(new FileReader(fileName));
                    bufferedReader.skip(offset + dimension);
                    int content;
                    while ((content = bufferedReader.read()) != -1) {
                        if (!Character.isLetter((char) content) && !Character.isDigit((char) content)) {
                            break;
                        }
                        helper.append((char) content);
                    }

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            finishResult(dictionary, longestWords, helper);
        }

        ResultTaskMap result = new ResultTaskMap(fileName, dictionary, longestWords);

        // because there are many threads that process information in the same time it might occur some problems in this
        // region because of the add operation, so we will treat this portion of the code as a critical region
        try {
            Executor.lock.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        Executor.resultsTaskMap.add(result);
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


    private void finishResult(Map<Integer, Integer> dictionary, ArrayList<String> longestWords, StringBuilder helper) {
        dictionary.merge(helper.length(), 1, Integer::sum);
        // in case the length of the word is the largest from its sequence we should replece what is in the list with
        // the largest word with this one and update the longestLen
        if (helper.length() > longestLen) {
            longestLen = helper.length();
            longestWords.clear();
            longestWords.add(helper.toString());
        // in case the length of the word is equal to the largest word that has found from the sequence we add it to the
        // list
        } else if (helper.length() == longestLen) {
            longestWords.add(helper.toString());
        }
    }
}
