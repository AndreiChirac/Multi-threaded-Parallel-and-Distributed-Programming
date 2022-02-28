package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;

public class Buffer {
	int value;
	ArrayBlockingQueue array = new ArrayBlockingQueue(1);

	void put(int value) {
		try {
			array.put(value);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	int get() {
		return value;
	}
}
