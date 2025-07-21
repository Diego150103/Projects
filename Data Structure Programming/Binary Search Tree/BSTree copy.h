#include <iostream>
    using namespace std;

#include "BSTNode.h"

#ifndef BSTree_h
#define BSTree_h

class BSTree
{
private:
    BSTNode *root;

public:
    BSTree ()
    {
        root = 0;
    }

    BSTNode* getRoot();
    void setRoot(BSTNode* node);

    bool isEmpty();
    void insert(int data);	
    BSTNode* find(BSTNode* node, int data) const;	
    void traversePreOrder(BSTNode *node) const;
    void traverseInOrder(BSTNode *node) const;
    void traversePostOrder(BSTNode *node) const;
    int getNodeHeight(BSTNode *node) const;
    int getTreeHeight() const;
 


};

#endif