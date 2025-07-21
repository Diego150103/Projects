#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <vector>
#include <iostream>  
    using namespace std;

class PriorityQueue {

public:
    PriorityQueue(int capacity = 100);
    bool isEmpty() const;
    void insert(int value);
    void deleteMin();
    void printArray() const;

private:
    int theSize;                 // Number of elements in heap
    std::vector<int> array;      // The heap array
    void buildHeap();
    void percolateDown(int hole);

};

#endif // PRIORITYQUEUE_H

