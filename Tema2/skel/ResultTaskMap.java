import java.util.ArrayList;
import java.util.Map;

public class ResultTaskMap {
    private final String fileName;
    private final Map<Integer, Integer> dictionary;
    private final ArrayList<String> longestWords;

    public ResultTaskMap(String fileName, Map<Integer, Integer> dictionary, ArrayList<String> longestWords) {
        this.fileName = fileName;
        this.dictionary = dictionary;
        this.longestWords = longestWords;
    }

    public String getFileName() {
        return fileName;
    }

    public Map<Integer, Integer> getDictionary() {
        return dictionary;
    }

    public ArrayList<String> getLongestWords() {
        return longestWords;
    }
}
