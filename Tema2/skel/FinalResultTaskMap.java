import java.util.ArrayList;
import java.util.Map;

public class FinalResultTaskMap {
    private final ArrayList< Map<Integer, Integer> > dictionary;
    private final ArrayList<String> longestWords;

    public FinalResultTaskMap(ArrayList<Map<Integer, Integer>> dictionary, ArrayList<String> longestWords) {
        this.dictionary = dictionary;
        this.longestWords = longestWords;
    }

    public ArrayList<Map<Integer, Integer>> getDictionary() {
        return dictionary;
    }

    public ArrayList<String> getLongestWords() {
        return longestWords;
    }

}
