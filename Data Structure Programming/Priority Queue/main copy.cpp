#include "PriorityQueue.h"
    using namespace std;

void printMenu() {

    cout << "\n--- Priority Queue Menu ---\n";
    cout << "1. Insert an item into the heap\n";
    cout << "2. Delete the min\n";
    cout << "3. Print the array representation of the queue\n";
    cout << "4. Exit\n";
    cout << "Select an option: ";

}

int main() {

    PriorityQueue pq;
    int choice = 0;
    int number = 0;

    cout << "Enter positive integer numbers (enter -1 to end): " << endl;

    while (cin >> number && number != -1) {

        if (number > 0) {

            pq.insert(number);

        } else {

            cout << "Invalid input, please enter a positive integer: " << endl;

        }
    }

    do {

        printMenu();
        cin >> choice;

        switch (choice) {

            case 1:
                cout << "Enter a number to insert: ";
                cin >> number;

                if (number > 0) {

                    pq.insert(number);

                } else {

                    cout << "Please enter a positive integer." << endl;

                }

                break;

            case 2:
                if (!pq.isEmpty()) {

                    pq.deleteMin();
                    cout << "Minimum element deleted." << endl;

                } else {

                    cout << "Heap is empty, nothing to delete." << endl;

                }
                break;

            case 3:
                cout << "Heap array: ";
                pq.printArray();
                break;

            case 4:
                cout << "Exiting program." << endl;
                break;

            default:
                cout << "Invalid choice, please try again." << endl;
        }

    } while (choice != 4);

    return 0;

}
