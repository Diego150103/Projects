#include "BSTree.h"
#include <iostream>
using namespace std;


BSTNode* BSTree::getRoot(){

    return root;

}

void BSTree::setRoot(BSTNode* node){

    root = node;

}

bool BSTree::isEmpty(){

    if(getRoot() == NULL)
        return true;

    return false;

}

void BSTree::insert(int data)
{

    //Create node object
    BSTNode *node = new BSTNode (data) ;
    BSTNode *parent = NULL;

    //check whether the tree is empty
    if(isEmpty()){

        root = node;

    } else {
   
        BSTNode *current = root;

        //Check which side of the tree we need to insert the node
        while(current)
        {
            parent = current;

            if (node -> getData() > current -> getData()) 
                current = current -> getRight();
            else
                current = current -> getLeft();
        }

        //When the parent node is greater, then set the left child
        if(node -> getData() < parent -> getData())
        {

            parent -> setLeft(node);

        } else {

            //Otherwise set the Right node
            parent -> setRight(node);

        }
    }
}	

BSTNode* BSTree::find(BSTNode* node, int data) const 
{

    //Base case
    if (node == nullptr || node->getData() == data) {

        return node;

    } 

    //Recursive function when data is less than node
    else if (data < node->getData()) {

        return find(node->getLeft(), data);

    } else {

        //Recursive function when data is grater than the one from the parent
        return find(node->getRight(), data);

    }
}

void BSTree::traversePreOrder(BSTNode *node) const
{
    if (node != nullptr) {

        cout << node->getData() << " ";
        traversePreOrder(node->getLeft());
        traversePreOrder(node->getRight());

    }

}

void BSTree::traverseInOrder(BSTNode *node) const
{
    if (node != nullptr) {

        traverseInOrder(node->getLeft());
        cout << node->getData() << " ";
        traverseInOrder(node->getRight());

    }
}

void BSTree::traversePostOrder(BSTNode *node) const
{
    if (node != nullptr) {

        traversePostOrder(node->getLeft());
        traversePostOrder(node->getRight());
        cout << node->getData() << " ";
        
    }

}

int BSTree::getNodeHeight(BSTNode *node) const
{
    if (node == NULL) {

        //Base case
        return -1; 

    } else {
    
        //Recursive functions
        int leftHeight = getNodeHeight(node->getLeft());
        int rightHeight = getNodeHeight(node->getRight());

        //Incrementing counter for each iteration and returning the greatest height
        return max(leftHeight, rightHeight) + 1;

    }
}

int BSTree::getTreeHeight() const
{

    //Calling height function for the root node
    return getNodeHeight(root);

}