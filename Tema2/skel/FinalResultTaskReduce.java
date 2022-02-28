public class FinalResultTaskReduce {

    private final String fileName;
    private final int maxLength;
    private final int app;

    public FinalResultTaskReduce(String fileName, int maxLength, int app) {
        this.fileName = fileName;
        this.maxLength = maxLength;
        this.app = app;
    }

    public String getFileName() {
        return fileName;
    }

    public int getMaxLength() {
        return maxLength;
    }

    public int getApp() {
        return app;
    }
}
