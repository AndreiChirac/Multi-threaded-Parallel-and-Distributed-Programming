package task7;

import util.BinarySearchTreeNode;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyTAsk<T extends Comparable<T>> implements Runnable {
    ExecutorService tpe;
    AtomicInteger counter;
    BinarySearchTreeNode<T> binarySearchTree;
    T value;
    CompletableFuture<String> completableFuture;

    public MyTAsk(ExecutorService tpe, AtomicInteger counter,
                  BinarySearchTreeNode<T> binarySearchTree, T value,
                  CompletableFuture<String> completableFuture) {
        this.tpe = tpe;
        this.counter = counter;
        this.binarySearchTree = binarySearchTree;
        this.value = value;
        this.completableFuture = completableFuture;
    }

    @Override
    public void run() {
        if (binarySearchTree != null) {
            if (value.equals(binarySearchTree.getValue())) {
                completableFuture.complete(String.valueOf(binarySearchTree.getValue()));
                tpe.shutdown();
            } else if (binarySearchTree.getValue().compareTo(value) > 0) {
                //searchValue(binarySearchTree.getLeft(), value);
                counter.incrementAndGet();
                tpe.submit(new MyTAsk<>(tpe, counter, binarySearchTree.getLeft(), value, completableFuture));
            } else {
                //searchValue(binarySearchTree.getRight(), value);
                counter.incrementAndGet();
                tpe.submit( new MyTAsk<>(tpe, counter, binarySearchTree.getRight(), value, completableFuture));
            }
        }

        int left = counter.decrementAndGet();
        if (left == 0) {
            completableFuture.complete(null);
            tpe.shutdown();
        }

    }
}
