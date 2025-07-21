#include "PriorityQueue.h"

// Constructor for the priority queue
PriorityQueue::PriorityQueue(int capacity) : theSize(0), array(capacity + 1) {}

// Checks if the priority queue is empty
bool PriorityQueue::isEmpty() const {

    return theSize == 0;

}


void PriorityQueue::insert(int value) {

    array[0] = value;

    // If the current size of the priority queue reaches the array's capacity, resize the array
    if( theSize + 1 == array.size() )

        array.resize(array.size( ) * 2 + 1 );

    // Percolate up
    int hole = ++theSize;

    for(; value < array[hole / 2]; hole /= 2)

        array[hole] = array[ hole / 2 ];

    array[hole] = value;

}

void PriorityQueue::deleteMin() {

    if (isEmpty()) {

        throw underflow_error("Priority queue underflow!");

    }

    // Move the last element to the root and decrease the size of the heap
    array[1] = array[theSize--];

    // Percolate down to restore the heap property
    percolateDown(1);
}

void PriorityQueue::percolateDown(int hole) {

    int child;
    int tmp = array[hole]; // Store the value at the current hole

    // Move down the heap to find the correct position for the value at 'hole'
    for (; hole * 2 <= theSize; hole = child) {

        child = hole * 2;

        // Check if there is a right child and it is smaller than the left child
        if (child != theSize && array[child + 1] < array[child]) {

            ++child;

        }

        // If the child is smaller than the temp, move the child up
        if (array[child] < tmp) {

            array[hole] = array[child];

        } else {

            break;

        }
    }

    array[hole] = tmp; // Place the temp value in its correct position

}

void PriorityQueue::printArray() const {

    //Print elements in priority queue.
    for (int i = 1; i <= theSize; ++i) {

        cout << array[i] << " ";

    }

    cout << endl;
}
