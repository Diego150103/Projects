//
//  List.cpp
//  DoublyLinkedList
//
//  Created by Madhuri Debnath on 9/24/20.
//

#include<iostream>
#include "List.h"
using namespace std;

void List::insert(int value)
{
    //Create node object
    Node* newNode = new Node(value);

    //Check if list is empty
    if (tailPtr->getPrevPtr() == NULL) {

        tailPtr->setPrevPtr(newNode);
        headPtr->setNextPtr(newNode);
        newNode->setPrevPtr(NULL);

    //Insert at the end of list
    } else {

        Node* prev = tailPtr->getPrevPtr();
        tailPtr->setPrevPtr(newNode);
        newNode->setPrevPtr(prev);
        prev->setNextPtr(newNode);

    }

}

void List::insertAtIndex(int data, int index)
{
    Node* current = headPtr->getNextPtr();
    Node* prev = headPtr;
    Node* next = NULL;
    int i = 0;

    //Check the case that index selected is 0
    if(index == 0){

        //Create node object
        Node* newNode1 = new Node(data, current, NULL);

        //Set elemnt as the first on the list
        current->setPrevPtr(newNode1);
        headPtr->setNextPtr(newNode1);

    } else {

        //Traverse the list
        while(current != NULL)
        {
            //When index found stop traversing
            if(i == index){

                break;

            }

            prev = current;
            current = current -> getNextPtr();
            i++;

        }

        //Case where the element to insert is in the last index of the list
        if(tailPtr->getPrevPtr() == current){

            //Update the pointers
            Node* newNode2 = new Node(data, current, prev);
            current->setPrevPtr(newNode2);
            prev->setNextPtr(newNode2);

        } else {

            //Update pointers
            next = current->getNextPtr();
            Node *newNode = new Node(data, current, prev);

            current->setPrevPtr(newNode);
            current->setNextPtr(next);

            prev->setNextPtr(newNode);

            next->setPrevPtr(current);

        }

    }

}

void List::deleteAtIndex(int index)
{

    Node* current = headPtr->getNextPtr();
    Node* prev = headPtr;
    Node* nextPtr = NULL;

    int i = 0;

    //Check the case where the index to delete is the first element
    if(index == 0){

        nextPtr = current->getNextPtr();

        headPtr->setNextPtr(nextPtr);
        nextPtr->setPrevPtr(NULL);

    } else{

        //Traverse List
        while(current != NULL)
        {
            //When index found stop traversing
            if(i == index)
            {
                break;
            }

            prev = current;
            current = current->getNextPtr();
            i++;
        }

        //Check the case where the element to delete is the last element of the list
        if(tailPtr->getPrevPtr() == current){

            //Update pointers
            prev->setNextPtr(NULL);
            tailPtr->setPrevPtr(prev);

        } else {

            //Update pointers
            nextPtr = current->getNextPtr();
            prev->setNextPtr(nextPtr);
            nextPtr->setPrevPtr(prev);

        }

    }

}

void List::readItem(int index)
{

    Node* current = headPtr->getNextPtr();
    int i = 0;

    //Traverse list
    while (current != NULL)
    {
        //When index found stop traversing
        if(i == index)
            break;
        current = current->getNextPtr();
        i++;
    }

    cout << current->getData() << endl;

}

void List::reverseList()
{

    Node* current = headPtr->getNextPtr();
    Node* prev = headPtr;

    //Store last and first elements on the list
    Node* head = headPtr->getNextPtr();
    Node* tail = tailPtr->getPrevPtr();

    //Swap tail and head pointers
    tailPtr->setPrevPtr(head);
    headPtr->setNextPtr(tail);

    //Traverse list
    while (current != NULL)
    {
        //Update pointers
        prev = current->getPrevPtr();
        current->setPrevPtr(current->getNextPtr());

        current->setNextPtr(prev);

        current = current->getPrevPtr();
    }

}

void List::printForward()
{

    Node* current = headPtr->getNextPtr();

    //Traverse list (From head to tail)
    while (current != NULL) {

        //print list
        cout << current->getData() << "  ";
        current = current->getNextPtr();

    }

    cout << endl;

}

void List::printBackward()
{
    Node* current = tailPtr->getPrevPtr();

    //Traverse list (From tail to head)
    while (current != NULL) {

        //Print list
        cout << current->getData() << "  ";
        current = current->getPrevPtr();

    }

    cout << endl;

}
