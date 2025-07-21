#ifndef BSTNode_h
#define BSTNode_h

class BSTNode
{
private:
    int data;
    BSTNode *left;
    BSTNode *right;

public:
    // constructors
    BSTNode () :data(0), left(0), right(0) {}
    BSTNode (int val):data(val), left (0), right (0){}
    BSTNode (int val, BSTNode *l, BSTNode *r): data(val), left (l), right (r) {}

    // accessor functions
    int getData () const {return data;}
    BSTNode* getLeft () const {return left;}
    BSTNode* getRight () const {return right;}

    //Mutator functions
    void setData (int val) {data = val;}
    void setLeft (BSTNode *leftPointer) {left = leftPointer;}
    void setRight (BSTNode *rightPointer) {right = rightPointer;}
};


#endif