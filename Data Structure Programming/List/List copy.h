//
//  List.h
//  DoublyLinkedList
//
//  Created by Madhuri Debnath on 9/24/20.
//

#include<iostream>
#include "Node.h"
using namespace std;
#ifndef List_h
#define List_h

class List
{
private:
    Node* headPtr;
    Node* tailPtr;
public:
    List()
    {
        headPtr = new Node();
        tailPtr = new Node();
        headPtr -> setNextPtr(NULL);
        tailPtr -> setPrevPtr(NULL);
    }
    
    //accessor functions
    //complete the definitions of accessor functions.
    Node* getHeadPtr() const;
    Node* getTailPtr() const;
    
    // insert function
    // takes a integer number as an input
    // insert the number at the end of the list
    void insert(int value);
    
    // insertAtIndex function
    // takes a integer number and an index as inputs
    // insert the number at the given index
    void insertAtIndex(int data, int index);
    
    // ideleteAtIndex function
    // takes an index as input
    // delete the number from the given index
    void deleteAtIndex(int index);
    
    // readItem function
    // takes an index as input
    // return the number from the given index
    void readItem(int index);
    
    // reverseList function
    // reverse the list. Don not swap the values, please swap the nodes.
    void reverseList();
    
    //printforward function
    //print the list in forward direction
    void printForward();
    
    //printBackward
    //print the list in backward (reverse) direction
    void printBackward();
    
    
};

#endif /* List_h */
