package task7;

import util.BSTOperations;
import util.BinarySearchTreeNode;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    private static <T extends Comparable<T>> void searchValue(BinarySearchTreeNode<T> binarySearchTree, T value) {
        if (binarySearchTree != null) {
            if (value.equals(binarySearchTree.getValue())) {
                System.out.println("Found value: " + binarySearchTree.getValue());
            } else if (binarySearchTree.getValue().compareTo(value) > 0) {
                searchValue(binarySearchTree.getLeft(), value);
            } else {
                searchValue(binarySearchTree.getRight(), value);
            }
        }
    }

    public static void main(String[] args) {
        ExecutorService tpe = Executors.newFixedThreadPool(4);
        CompletableFuture<String> completableFuture = new CompletableFuture<>();
        AtomicInteger counter = new AtomicInteger(0);

        BinarySearchTreeNode<Integer> binarySearchTree = new BinarySearchTreeNode<>(3);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 6);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 9);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 2);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 10);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 1);

        //searchValue(binarySearchTree, 5);
        counter.incrementAndGet();
        tpe.submit(new MyTAsk<>(tpe, counter, binarySearchTree, 6, completableFuture));

        String result = null;
        try {
            result = completableFuture.get();
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }

        if (result != null) {
            System.out.println("Value was found " + result);
        } else {
            System.out.println("Value was not found");
        }
    }
}
