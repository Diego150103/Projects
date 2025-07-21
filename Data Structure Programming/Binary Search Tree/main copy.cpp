#include <iostream>
#include <vector>
#include "BSTree.h" 

using namespace std;

// Function to create a minimum height BST.
BSTNode* sortedArrayToBST(const vector<int>& arr, int start, int end) {
    if (start > end) {
        return nullptr;
    }

    // Middle element to maintain minimum height.
    int mid = start + (end - start) / 2;
    BSTNode* node = new BSTNode(arr[mid]);

    // Recursively construct the left subtree and right subtree.
    node->setLeft(sortedArrayToBST(arr, start, mid - 1));
    node->setRight(sortedArrayToBST(arr, mid + 1, end));

    return node;
}

int main() {

    vector<int> inputs;
    int input{0};

    cout << "Enter numbers in sorted order (Press -1 to stop):" << endl;

    while (cin >> input && input != -1) {

        inputs.push_back(input);
        
    }

    BSTree* tree = new BSTree();

    // Construct the BST.
    tree->setRoot(sortedArrayToBST(inputs, 0, inputs.size() - 1));

    // Output the height of the binary tree.
    cout << "Height of the tree: " << tree->getTreeHeight() << endl;

    // Printing the tree traversals.
    cout << "Inorder Traversal: ";
    tree->traverseInOrder(tree->getRoot());
    cout << endl;

    cout << "Preorder Traversal: ";
    tree->traversePreOrder(tree->getRoot());
    cout << endl;

    cout << "Postorder Traversal: ";
    tree->traversePostOrder(tree->getRoot());
    cout << endl;

    return 0;
    
}
