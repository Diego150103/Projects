//
//  main.cpp
//  DoublyLinkedList
//
//  Created by Madhuri Debnath on 9/23/20.
//

#include <iostream>
using namespace std;
#include "Node.h"
#include "List.h"

int main() {
    List head;
    int num, item, index;
    int menu;
    
    //Print a menu
    //Each of the menu option will be the operation of
    //Doubly Linked List Abstract DataType
    while(1)
    {
        cout << "Please enter a choice (1-8)" << endl;
        cout << "1:Insert at the end of the list" << endl;
        cout << "2:Insert at a given index" << endl;
        cout << "3:delete from a given index" << endl;
        cout << "4:read an item" << endl;
        cout << "5:reverse the list" << endl;
        cout << "6:print the list forward" << endl;
        cout << "7:print the list backward" << endl;
        cout << "8:exit" << endl;
        cin >> menu;
        if(menu == 8)
            break;
        switch(menu)
        {
            case 1:
                //user enters number to insert the item at the end of the list
                //enter -1 to end the input.
                cout << "enter item to insert, enter -1 to end" << endl;
                while(1)
                {
                    cin >> num;
                    if(num == -1)
                        break;
                    head.insert(num);
                    
                }
                break;
            case 2:
                cin >> item;
                cout << "Insert the index: ";
                cin >> index;
                head.insertAtIndex(item, index);
                break;
            case 3:
                cout << "Insert the index to delete: ";
                cin >> index;
                head.deleteAtIndex(index);
                break;
            case 4:
                cout << "Insert the index to read: ";
                cin >> index;
                head.readItem(index);
                break;
            case 5:
                cout << "Reversing the list" << endl;
                head.reverseList();
                break;
            case 6:
                head.printForward();
                break;
            case 7:
                head.printBackward();
                break;
            default:
                cout << "Enter a valid choice" << endl;
        }
                
    }
        
    return 0;
}
