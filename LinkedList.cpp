//============================================================================
// Name        : LinkedList.cpp
// Author      : Andrew Biggs
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
    Bid(string ID) {
        bidId = ID;
    }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node *next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // dummyNode created. In empty list, both head and tail point to the dummy node.
    Bid dummyBid("__dummy__");
    Node* dummyNode = new Node(dummyBid);
    head = dummyNode;
    tail = dummyNode;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    // Create new node, make tail node point to new node
    // Make new node the tail node, increment size of list
    Node* newNode = new Node(bid);
    tail->next = newNode;
    tail = newNode;
    size++;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // Create new node, make new node point to what dummy/header node pointed to,
    // Make header/dummy node point to new node. If list is empty, tail = new node.
    // Increment size of list.
    Node* newNode = new Node(bid);
    newNode->next = head->next;
    head->next = newNode;
    if (head == tail)
        tail = newNode;
    size++;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // start at the head, while loop over each bid, printing each bid's attributes
    Node* curNode = head->next;
    while (curNode != nullptr) {
        cout << curNode->bid.bidId << ": " << curNode->bid.title << " | ";
        cout << curNode->bid.amount << " | " << curNode->bid.fund << endl;
        curNode = curNode->next;
    }
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
    // Dummy node must not be removed
    if (bidId == "__dummy__")
        return;
    
    // While loop to linear search bidId match with each successive node
    // Loop ends after tail is checked and curNode becomes nullptr
    Node* curNode = head;
    while (bidId != curNode->next->bid.bidId && curNode != nullptr)
        curNode = curNode->next;
    
    // If match is found, curNode deletes the node following it by changing its next pointer to
    // the successive node's next pointer.
    if (bidId == curNode->next->bid.bidId) {
        curNode->next = curNode->next->next;
        size--;
        cout << "Bid removed." << endl;
    }
    else
        cout << "bidID not found in list." << endl;
}

/**
 * Search for the specified bidId
 *
 * @param bidID The bid id to search for
 */
Bid LinkedList::Search(string bidID) {
    // curNode points to the first node after dummy node.
    // While loop to search for bidId, if found, return searched bid.
    // If not found, bid with empty fields is initialized and returned.
    Node* curNode = head->next;
    while (curNode != nullptr) {
        if (curNode->bid.bidId == bidID) {
            return curNode->bid;
        }
        curNode = curNode->next;
    }
    
    Bid emptyBid("");
    return emptyBid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * Append each bid to a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;
    Bid bidAlreadyExists;
    // variable to prevent same CSV file from being loaded twice
    bool flag = false;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidAlreadyExists = bidList.Search(bid.bidId);
            if (bidAlreadyExists.bidId.empty()) {
                displayBid(bid);
                bidList.Append(bid);
            }
            else
                cout << "bidID already in use." << endl;
            break;

        case 2:
            if (!flag) {
                ticks = clock();
                    
                loadBids(csvPath, &bidList);
                    
                cout << bidList.Size() << " bids read" << endl;
                    
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " milliseconds" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                
                flag = true;
                break;
            }
            else {
                
                cout << "Bids already loaded once." << endl;
                cout << "Restart program to load another or same set of bids." << endl;
                break;
            }

        case 3:
            bidList.PrintList();

            break;

        case 4:
            ticks = clock();
                
            cin >> bidKey;
            bid = bidList.Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 5:
            cin >> bidKey;
            bidList.Remove(bidKey);

            break;
        }
    }

    cout << "Thank you for using this program! Goodbye." << endl;

    return 0;
}
