//
//  Node.h
//  DoublyLinkedList
//
//  Created by Madhuri Debnath on 9/24/20.
//


#ifndef Node_h
#define Node_h
class Node
{
private:
    int data;
    Node* next;
    Node* prev;
public:
    Node():data(0),next(NULL),prev(NULL){}
    Node(int val):data(val),next(NULL),prev(NULL){}
    Node(int val, Node* n, Node* p):data(val),next(n),prev(p){}
    
    //mutator functions
    void setData(int val){data = val;}
    void setNextPtr(Node* n){next = n;}
    void setPrevPtr(Node* p){prev = p;}
    
    //accessor functions
    int getData() const {return data;}
    Node* getNextPtr() const {return next;}
    Node* getPrevPtr() const {return prev;}
};


#endif /* Node_h */
