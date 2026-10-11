#include <iostream>
#include <string>
using namespace std;


void clearInput() {
    cin.ignore();
}

class Book {
public:
    int id;
    string title;
    string author;
    bool issued;

    Book* prev;
    Book* next;

    Book(int i, string t, string a) {
        id = i;
        title = t;
        author = a;
        issued = false;
        prev = next = NULL;
    }
};

class BookList {
private:
    Book* head;
    Book* tail;

public:
    BookList() {
        head = tail = NULL;
    }

    void addBook(int id, string title, string author) {
        Book* newBook = new Book(id, title, author);

        if (head == NULL) {
            head = tail = newBook;
        }
        else {
            tail->next = newBook;
            newBook->prev = tail;
            tail = newBook;
        }
    }

    Book* findBook(int id) {
        Book* temp = head;
        while (temp != NULL) {
            if (temp->id == id)
                return temp;
            temp = temp->next;
        }
        return NULL;
    }

    void deleteBook(int id) {
        Book* target = findBook(id);
        if (target == NULL) {
            cout << "Book not found.\n";
            return;
        }

        if (target == head)
            head = head->next;
        if (target == tail)
            tail = tail->prev;

        if (target->prev)
            target->prev->next = target->next;
        if (target->next)
            target->next->prev = target->prev;

        delete target;
        cout << "Book deleted successfully.\n";
    }

    void displayBooks() {
        Book* temp = head;
        cout << "\nID | Title | Author | Status\n";
        cout << "-------------------------------------------\n";

        while (temp != NULL) {
            cout << temp->id << " | "
                << temp->title << " | "
                << temp->author << " | "
                << (temp->issued ? "Issued" : "Available") << endl;
            temp = temp->next;
        }
    }
};

struct BSTNode {
    int id;
    BSTNode* left;
    BSTNode* right;
};

class BookIndexBST {
public:
    BSTNode* root;

    BookIndexBST() {
        root = NULL;
    }

    BSTNode* insert(BSTNode* node, int id) {
        if (node == NULL)
            return new BSTNode{ id, NULL, NULL };

        if (id < node->id)
            node->left = insert(node->left, id);
        else
            node->right = insert(node->right, id);

        return node;
    }

    bool search(BSTNode* node, int id) {
        if (node == NULL) return false;
        if (node->id == id) return true;
        if (id < node->id)
            return search(node->left, id);
        return search(node->right, id);
    }
};

class AdminStack {
private:
    string stack[20];
    int top;

public:
    AdminStack() {
        top = -1;
    }

    void push(string action) {
        if (top < 9)
            stack[++top] = action;
    }

    void display() {
        cout << "\nAdmin Activity Log:\n";
        if (top == -1) {
            cout << "No activity.\n";
            return;
        }
        for (int i = top; i >= 0; i--)
            cout << stack[i] << endl;
    }
};


class ReservationQueue {
private:
    string queue[20];
    int front, rear;

public:
    ReservationQueue() {
        front = rear = -1;
    }

    void enqueue(string name) {
        if (rear == 19) return;
        if (front == -1) front = 0;
        queue[++rear] = name;
    }

    void display() {
        if (front == -1) {
            cout << "No reservations.\n";
            return;
        }
        cout << "Waiting List: ";
        for (int i = front; i <= rear; i++)
            cout << queue[i] << " ";
        cout << endl;
    }
};

bool adminLogin() {
    string pass;
    cout << "Enter Admin Password: ";
    cin >> pass;
    return pass == "admin123";
}

int main() {

    cout << "====================================\n";
    cout << "      LIBRARY MANAGEMENT SYSTEM      \n";
    cout << "====================================\n\n";

    BookList library;
    BookIndexBST bst;
    AdminStack adminLog;
    ReservationQueue reservation;

    int ids[] = { 101,102,103,104,105,106,107,108,109,110 };
    string titles[] = {
        "Clean Code","Pragmatic Programmer","Atomic Habits","Deep Work",
        "Thinking Fast and Slow","The Alchemist","1984",
        "To Kill a Mockingbird","The Kite Runner","Harry Potter"
    };
    string authors[] = {
        "Martin","Hunt","Clear","Newport","Kahneman",
        "Coelho","Orwell","Lee","Hosseini","Rowling"
    };

    for (int i = 0; i < 10; i++) {
        library.addBook(ids[i], titles[i], authors[i]);
        bst.root = bst.insert(bst.root, ids[i]);
    }

    int role;
    cout << "1. Admin\n2. Student\nChoose Role: ";
    cin >> role;

    if (role == 1 && adminLogin()) {
        int ch;
        do {
            cout << "\n1. Add Book\n2. Delete Book\n3. View Books\n4. View Admin Log\n0. Exit\n";
            cin >> ch;

            if (ch == 1) {
                int id;
                string t, a;
                cout << "ID: "; cin >> id;
                cout << "Title: "; cin >> t;
                cout << "Author: "; cin >> a;
                library.addBook(id, t, a);
                bst.root = bst.insert(bst.root, id);
                adminLog.push("Added book " + to_string(id));
            }
            else if (ch == 2) {
                int id;
                cout << "ID: "; cin >> id;
                library.deleteBook(id);
                adminLog.push("Deleted book " + to_string(id));
            }
            else if (ch == 3)
                library.displayBooks();
            else if (ch == 4)
                adminLog.display();

        } while (ch != 0);
    }
    else if (role == 2) {
        int ch;
        do {
            cout << "\n1. Search Book\n2. View Books\n3. Issue Book\n4. Return Book\n0. Exit\n";
            cin >> ch;

            if (ch == 1) {
                int id;
                cout << "Enter ID: "; cin >> id;
                cout << (bst.search(bst.root, id) ? "Book Found\n" : "Book Not Found\n");
            }
            else if (ch == 2)
                library.displayBooks();
            else if (ch == 3) {
                int id;
                cout << "ID: "; cin >> id;
                Book* b = library.findBook(id);
                if (b && !b->issued) {
                    b->issued = true;
                    cout << "Book Issued Successfully\n";
                }
                else cout << "Cannot issue book\n";
            }
            else if (ch == 4) {
                int id;
                cout << "ID: "; cin >> id;
                Book* b = library.findBook(id);
                if (b && b->issued) {
                    b->issued = false;
                    cout << "Book Returned Successfully\n";
                }
                else cout << "Cannot return book\n";
            }

        } while (ch != 0);
    }

    cout << "\nSystem Closed Successfully.\n";
    return 0;
}
