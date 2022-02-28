package task8;

import util.BinarySearchTreeNode;

import java.util.concurrent.RecursiveTask;

public class MyTask<T extends Comparable<T>> extends RecursiveTask<Integer> {
    BinarySearchTreeNode<T> binarySearchTree;


    public MyTask(BinarySearchTreeNode<T> binarySearchTree) {
        this.binarySearchTree = binarySearchTree;
    }

    @Override
    protected Integer compute() {
        if (binarySearchTree == null) {
            return 0;
        }

        MyTask t1 = new MyTask(binarySearchTree.getRight());
        MyTask t2 = new MyTask(binarySearchTree.getLeft());

        t1.fork();
        t2.fork();

        return 1 + Math.max((Integer)t1.join(),
                (Integer)t2.join());

    }
}
