/*  Lab 22: Upgrading dll demo to add additional functionality
    Objectives:        
        1. Create a method delete_pos() that will delete a node, not by its value, but by position.
            The method should also cover the case where we want to delete the head or tail node.
        2. Create a method pop_front(), that will delete the head node.
        3. Create a method pop_back() that will delete the tail node.
        4. Update the current delete() method to a better name, delete_val(). Keep the functionality in the method that will still delete the head node,
            even though the new pop_front() method will also do that. This is fine to code both, for the purposes of class completeness.
        5. Write a driver that fully exercises these changes made to the class.

    Design decisions:
        1. Regarding item 4 above, I didn't find a delete() method, but I did find delete_node() which is what I think was being referred to. That was changed
            to delete_val() and a prompt was created in main to get an integer value from user to delete the referenced node.
        2. Assignment didn't specify a return value for the pop methods, so I assumed a return value of the data type as is consistent with pop() functionality
            where it doesn't merely delete, but also returns the value. IN THIS CASE, the return value SHOULD be type Node, however because Node is private,
            main is unable to access the Node data structure. Even if a method getNode() was created, main would have no way of referring to the return type
            since a Node* could not be created in main.
        3. Try/catch statements were used to handle the pop and delete_val methods in main. delete_val() has no return type, but both pop_front() and pop_back() do.
            In order to handle a situation where main attempts to pop from an empty list, there would be no return value, and returning a value like 0 or -1 could be
            deceptive and lead the user to think the value 0 or -1 was actually removed from an empty list. Try/Catch is also beneficial because the delet_val()
            collects user input as a string and attempts to convert to int with stoi() which could throw invalid_argument().
*/

#include <iostream>
#include <string> // getline()
using namespace std;

class DoublyLinkedList {
private:
    struct Node {
        int data;
        Node* prev;
        Node* next;
        Node(int val, Node* p = nullptr, Node* n = nullptr) {
            data = val; 
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    // constructor
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    void push_back(int value) {
        Node* newNode = new Node(value);
        if (!tail)  // if there's no tail, the list is empty
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void push_front(int value) {
        Node* newNode = new Node(value);
        if (!head)  // if there's no head, the list is empty
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    void insert_after(int value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode; // Inserting at the end
        temp->next = newNode;
    }

    // Changed from delete_node. I think this is the method referenced in requirement 4
    void delete_val(int value) {
        if (!head) throw underflow_error("Trying to delete from empty list."); // Empty list

        Node* temp = head;
        while (temp && temp->data != value)
            temp = temp->next;

        if (!temp) throw invalid_argument("Item not found in list."); // Value not found

        if (temp->prev) {
            temp->prev->next = temp->next;
        } else {
            head = temp->next; // Deleting the head
        }

        if (temp->next) {
            temp->next->prev = temp->prev;
        } else {
            tail = temp->prev; // Deleting the tail
        }

        delete temp;
    }

    // Added method for assignment to delete based on index
    void delete_pos(int index) {
        if (!head) return; // Empty list

        Node* temp = head;
        // Loop to correct index in list
        for (size_t i = 0; i < index && temp; i++)
        {
            temp = temp->next;
        }
        
        if (!temp) return; // Value not found

        // Checking for head element and prev value
        if (temp->prev) {
            temp->prev->next = temp->next;
        } else {
            head = temp->next; // Deleting the head
        }

        // Checking for tail and next value
        if (temp->next) {
            temp->next->prev = temp->prev;
        } else {
            tail = temp->prev; // Deleting the tail
        }

        delete temp;
    }

    // Delete head, reassign head pointer and prev, return data value
    int pop_front(){
        if (!head) throw underflow_error("Trying to pop from empty list."); // Empty list
        int data = head->data;
        Node* temp = head;
        head = head->next; // Reassign head and delete
        head->prev = nullptr;
        delete temp;
        return data;
    }

    // Delete tail, reassign tail pointer and next, return data value
    int pop_back(){
        if (!tail) throw underflow_error("Trying to pop from empty list."); // Empty list
        int data = tail->data;
        Node* temp = tail;
        tail = tail->prev; // Reassign head and delete
        tail->next = nullptr;
        delete temp;
        return data;
    }

    void print() {
        Node* current = head;
        if (!current) return;
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) return;
        while (current) {
            cout << current->data << " ";
            current = current->prev;
        }
        cout << endl;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Driver program
int main() {
    const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;
    DoublyLinkedList list;
    int size = rand() % (MAX_LS-MIN_LS+1) + MIN_LS;
    string userValue = ""; // To collect userinput for value to delete

    for (int i = 0; i < size; ++i)
        list.push_back(rand() % (MAX_NR-MIN_NR+1) + MIN_NR);
    cout << "List forward: ";
    list.print();

    // Test delete index method
    cout << "\tDeleting index: 2" << endl;
    list.delete_pos(2);

    // Test popping the head node
    cout << "\tPopping head value: ";
    try
    {
        cout << list.pop_front();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what();
    }
    cout << endl;    

    // Test popping the tail node
    cout << "\tPopping tail value: ";
    try
    {
        cout << list.pop_back();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what();
    }
    cout << endl;  

    // Get user value, and attempt to delete
    cout << "\tPlease enter a value to delete: ";
    getline(cin, userValue);
    try
    {
        cout << "\tAttempting to delete: " << userValue;
        list.delete_val(stoi(userValue));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what();
    }
    cout << endl;  

    cout << "List backward: ";
    list.print_reverse();

    cout << "Deleting list, then trying to print.\n";
    list.~DoublyLinkedList();
    cout << "List forward: ";
    list.print();
    cout << endl;

    return 0;
}
