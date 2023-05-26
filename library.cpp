#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Book struct 
struct Book {
    string title;
    string author;
    string ISBN;
    int year;
    int copies;
};

// Linked list node struct
struct ListNode {
    Book book;
    ListNode* next;
};

// Binary search tree node struct
struct TreeNode {
    Book book;
    TreeNode* left;
    TreeNode* right;
};

void intro();

void intro()
{
    cout << "\n\tWelcome to Our Library Managment System :)";
    cout << "\n\n\tThis Program Was Made by:- IT Students";
    cin.get();
}

// Linked list class
class BookList {
private:
    ListNode* head;

public:
    BookList() {
        head = nullptr;
    }

    void addBook(Book book) {
        ListNode* newNode = new ListNode;
        newNode->book = book;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        }
        else {
            ListNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    bool removeBook(string ISBN) { 
        if (head == nullptr) { 
            return false;
        } 

        if (head->book.ISBN == ISBN) { 
            ListNode* temp = head; 
            head = head->next; 
            delete temp;  
            return true; 
        }

        ListNode* temp = head; 
        while (temp->next != nullptr && temp->next->book.ISBN != ISBN) { 
            temp = temp->next; 
        }

        if (temp->next != nullptr) {
            ListNode* toDelete = temp->next;
            temp->next = toDelete->next; 
            delete toDelete;
            return true;
        } 

        // Return false if book with ISBN not found
        return false;
    }

    Book* searchBook(string ISBN) {
        ListNode* temp = head;
        while (temp != nullptr) {
            if (temp->book.ISBN == ISBN) {
                return &(temp->book);
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void display() {
        ListNode* temp = head;
        while (temp != nullptr) {
            cout << "\n\t\tTitle: " << temp->book.title << endl;
            cout << "\n\t\tAuthor: " << temp->book.author << endl;
            cout << "\n\t\tISBN: " << temp->book.ISBN << endl;
            cout << "\n\t\tPublication Year: " << temp->book.year << endl;
            cout << "\n\t\tNumber of Copies Available: " << temp->book.copies << endl;
            cout << "\n\t\t------------------------------" << endl;
            temp = temp->next;
        }
    }

    void saveToFile(string filename) {
        ofstream output(filename);
        ListNode* temp = head;
        while (temp != nullptr) {
            output << temp->book.title << endl;
            output << temp->book.author << endl;
            output << temp->book.ISBN << endl;
            output << temp->book.year << endl;
            output << temp->book.copies << endl;
            temp = temp->next;
        }
        output.close();
    }

    void loadFromFile(string filename) {
        ifstream input(filename);
        if (input.is_open()) {
            string line;
            while (getline(input, line)) {
                Book book;
                book.title = line;
                getline(input, line);
                book.author = line;
                getline(input, line);
                book.ISBN = line;
                getline(input, line);
                book.year = stoi(line);
                getline(input, line);
                book.copies = stoi(line);
                addBook(book);
            }
            input.close();
        }
    }
};

// Binary search tree class
class BookTree {
private:
    TreeNode* root;

public:
    BookTree() {
        root = nullptr;
    }

    void addBook(Book book) {
        TreeNode* newNode = new TreeNode;
        newNode->book = book;
        newNode->left = nullptr;
        newNode->right = nullptr;

        if (root == nullptr) {
            root = newNode;
        }
        else {
            TreeNode* temp = root;
            while (true) {
                if (book.ISBN < temp->book.ISBN) {
                    if (temp->left == nullptr) {
                        temp->left = newNode;
                        break;
                    }
                    else {
                        temp = temp->left;
                    }
                }
                else {
                    if (temp->right == nullptr) {
                        temp->right = newNode;
                        break;
                    }
                    else {
                        temp = temp->right;
                    }
                }
            }
        }
    }

    void removeBook(string ISBN) {
        root = removeBookHelper(root, ISBN);
    }

    TreeNode* removeBookHelper(TreeNode* node, string ISBN) {
        if (node == nullptr) {
            return nullptr;
        }

        if (ISBN < node->book.ISBN) {
            node->left = removeBookHelper(node->left, ISBN);
        }
        else if (ISBN > node->book.ISBN) {
            node->right = removeBookHelper(node->right, ISBN);
        }
        else {
            if (node->left == nullptr) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }

            TreeNode* successor = node->right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }

            node->book = successor->book;
            node->right = removeBookHelper(node->right, successor->book.ISBN);
        }

        return node;
    }

    Book* searchBook(string ISBN) {
        TreeNode* temp = root;
        while (temp != nullptr) {
            if (temp->book.ISBN == ISBN) {
                return &(temp->book);
            }
            else if (ISBN < temp->book.ISBN) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        return nullptr;
    }

    void display(TreeNode* node) {
        if (node != nullptr) {
            display(node->left);
            cout << "\n\t\tTitle: " << node->book.title << endl;
            cout << "\n\t\tAuthor: " << node->book.author << endl;
            cout << "\n\t\tISBN: " << node->book.ISBN << endl;
            cout << "\n\t\tPublication Year: " << node->book.year << endl;
            cout << "\n\t\tNumber of Copies Available: " << node->book.copies << endl;
            cout << "\n\t\t------------------------------" << endl;
            display(node->right);
        }
    }

    void saveToFileHelper(TreeNode* node, ofstream& output) {
        if (node != nullptr) {
            output << node->book.title << endl;
            output << node->book.author << endl;
            output << node->book.ISBN << endl;
            output << node->book.year << endl;
            output << node->book.copies << endl;
            saveToFileHelper(node->left, output);
            saveToFileHelper(node->right, output);
        }
    }

    void saveToFile(string filename) {
        ofstream output(filename);
        saveToFileHelper(root, output);
        output.close();
    }

    void loadFromFileHelper(TreeNode*& node, ifstream& input) {
        if (input.peek() == EOF) {
            return;
        }
        Book book;
        getline(input, book.title);
        getline(input, book.author);
        getline(input, book.ISBN);
        input >> book.year;
        input >> book.copies;
        node = new TreeNode;
        node->book = book;
        node->left = nullptr;
        node->right = nullptr;
        loadFromFileHelper(node->left, input);
        loadFromFileHelper(node->right, input);
    }

    void loadFromFile(string filename) {
        ifstream input(filename);
        if (input.is_open()) {
            loadFromFileHelper(root, input);
            input.close();
        }
    }


};

// Main function
int main() {
    BookList bookList;
    BookTree bookTree;

    // Load data from files
    bookList.loadFromFile("books.txt");
    bookTree.loadFromFile("books.txt");

    int choice;
    intro();
    do {
        system("CLS");
        cout << "\n\n\t\tLibrary Management System" << endl;
        cout << "\n\n\t\t1. Add Book" << endl;
        cout << "\n\t\t2. Remove Book" << endl;
        cout << "\n\t\t3. Search Book" << endl;
        cout << "\n\t\t4. Display All Books" << endl;
        cout << "\n\t\t5. Save Data to File" << endl;
        cout << "\n\t\t6. Exit" << endl;
        cout << "\n\t\tPlease enter your choice: ";
        cin >> choice;


        system("CLS");
        switch (choice) {
        case 1: {
            Book book;
            cout << "\n\t\tEnter Book Title: ";
            cin.ignore();
            getline(cin, book.title);
            cout << "\n\t\tEnter Author Name: ";
            getline(cin, book.author);
            cout << "\n\t\tEnter ISBN: ";
            getline(cin, book.ISBN);
            cout << "\n\t\tEnter Publication Year: ";
            cin >> book.year;
            cout << "\n\t\tEnter Number of Copies Available: ";
            cin >> book.copies;

            bookList.addBook(book);
            bookTree.addBook(book);
            cout << "\n\t\tBook Added Successfully!" << endl;
            cout << "\n\n\n";
            break;
        }
        case 2: {
            string ISBN; 
            cout << "\n\t\tEnter ISBN of Book to Remove: ";
            cin >> ISBN; 

            // Remove the book from the linked list and the binary search tree
            bool bookRemoved = bookList.removeBook(ISBN);
            bookTree.removeBook(ISBN);

            if (bookRemoved) { 
                cout << "\n\t\tBook with ISBN " << ISBN << " has been successfully removed from the library." << endl; 
            }
            else {
                cout << "\n\t\tError: Book with ISBN " << ISBN << " not found in the library." << endl; 
            }
            cout << "\n\n\n"; 
            break;
        }
        case 3: {
            string ISBN;
            cout << "\n\t\tEnter the ISBN of the book to search for: ";
            cin >> ISBN;

            // Search for the book in the linked list
            Book* bookInList = bookList.searchBook(ISBN);

            // Search for the book in the binary search tree
            Book* bookInTree = bookTree.searchBook(ISBN);

            // Check if the book was found
            if (bookInList != nullptr) {
                cout << "\n\t\tBook found in the linked list:" << endl;
                cout << "\n\t\tTitle: " << bookInList->title << endl;
                cout << "\n\t\tAuthor: " << bookInList->author << endl;
                cout << "\n\t\tISBN: " << bookInList->ISBN << endl;
                cout << "\n\t\tPublication Year: " << bookInList->year << endl;
                cout << "\n\t\tNumber of Copies Available: " << bookInList->copies << endl;
            }
            else if (bookInTree != nullptr) {
                cout << "\n\t\tBook found in the binary search tree:" << endl;
                cout << "\n\t\tTitle: " << bookInTree->title << endl;
                cout << "\n\t\tAuthor: " << bookInTree->author << endl;
                cout << "\n\t\tISBN: " << bookInTree->ISBN << endl;
                cout << "\n\t\tPublication Year: " << bookInTree->year << endl;
                cout << "\n\t\tNumber of Copies Available: " << bookInTree->copies << endl;
            }
            else {
                cout << "\n\t\tBook not found in the library." << endl;
            }
            cout << "\n\n\n";
            break;
        }
        case 4: {
            cout << "\n\t\tAll books in the library:" << endl;
            cout << "\n\t\t     - - - - - - - - - - - " << endl;
            bookList.display();
            //bookTree.display();
            cout << "\n\n\n";
            break;
        }
        case 5: {
            bookList.saveToFile("books.txt");
            bookTree.saveToFile("books.txt");
            cout << "\n\t\tData saved to file." << endl;
            cout << "\n\n\n";
            break;
        }
        case 6: {
            cout << "\n\t\tThank you for using our Management System! " << endl;
            cout << "\n\n\n";
            break;
        }
        default:
            cout << "\n\t\t!!!Invalid choice. Please enter a number between 1 and 6.!!!" << endl;
        }

        cout << endl;
        cout << "\n\n\n";
        system("pause");

    } while (choice != 6);

    return 0;
} 
