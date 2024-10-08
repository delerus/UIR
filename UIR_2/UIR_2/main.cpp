#include <iostream>
#include <windows.h>
#include <fstream>
#include <random>
#include <ctime>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;

struct Data {
    int ip;
    string subscriber;
    string phoneNumber;
    string name;
    int amount;

    friend ostream& operator<<(ostream& os, const Data& data);
    friend istream& operator>>(istream& is, Data& data);
};

ostream& operator<<(ostream& os, const Data& data) {
    os << data.ip << " " << data.subscriber << " " << data.phoneNumber << " " << data.name << " " << data.amount;
    return os;
}

istream& operator>>(istream& is, Data& data) {
    is >> data.ip >> data.subscriber >> data.phoneNumber >> data.name >> data.amount;
    return is;
}

string generateRandomPhoneNumber() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);

    string phoneNumber = "";
    for (int i = 0; i < 10; i++) {
        if (i == 0) {
            phoneNumber += to_string(dis(gen) + 1);
        }
        else {
            phoneNumber += to_string(dis(gen));
        }
    }

    return "+7" + phoneNumber;
};

vector<string> firstNames = {
    "Liam", "Emma", "Noah", "Olivia", "William", "Ava", "James", "Isabella",
    "Oliver", "Sophia", "Benjamin", "Mia", "Elijah", "Charlotte", "Lucas", "Harper"
};

vector<string> lastNames = {
    "Smith", "Johnson", "Williams", "Brown", "Davis", "Miller", "Wilson", "Moore",
    "Taylor", "Anderson", "Thomas", "Jackson", "White", "Harris", "Martin", "Thompson"
};

string generateRandomName() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> firstNameDist(0, firstNames.size() - 1);
    static uniform_int_distribution<> lastNameDist(0, lastNames.size() - 1);

    string firstName = firstNames[firstNameDist(gen)];
    string lastName = lastNames[lastNameDist(gen)];

    return firstName + " " + lastName;
}

int generateRandomNumber() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100000, 999999);

    int randomNumber = dis(gen);

    return randomNumber;
}

string generateRandomString() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> charDist(48, 57);

    string randomString;
    for (int i = 0; i < 3; i++) {
        randomString += static_cast<char>(charDist(gen));
    }

    return randomString;
}

Data generateRandomData(int i) {
    Data data;
    data.ip = i + 1;
    data.subscriber = generateRandomString();
    data.phoneNumber = generateRandomPhoneNumber();
    data.name = generateRandomName();
    data.amount = generateRandomNumber();
    return data;
}

class Node {
public:
    Data data;
    Node* left;
    Node* right;
    int height;

    Node(Data data) : data(data), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;
public:
    int height(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    Node* rightRotate(Node* z) {
        Node* y = z->left;
        Node* T3 = y->right;

        y->right = z;
        z->left = T3;

        z->height = 1 + max(height(z->left), height(z->right));
        y->height = 1 + max(height(y->left), height(y->right));

        return y;
    }

    Node* leftRotate(Node* z) {
        Node* y = z->right;
        Node* T2 = y->left;

        y->left = z;
        z->right = T2;

        z->height = 1 + max(height(z->left), height(z->right));
        y->height = 1 + max(height(y->left), height(y->right));

        return y;
    }

    int getBalance(Node* node) {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }

    Node* insert(Data data, Node* node) {
        if (node == nullptr)
            return new Node(data);

        if (data.subscriber < node->data.subscriber)
            node->left = insert(data, node->left);
        else if (data.subscriber > node->data.subscriber)
            node->right = insert(data, node->right);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && data.subscriber < node->left->data.subscriber)
            return rightRotate(node);

        if (balance < -1 && data.subscriber > node->right->data.subscriber)
            return leftRotate(node);

        if (balance > 1 && data.subscriber > node->left->data.subscriber) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && data.subscriber < node->right->data.subscriber) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* remove(string subscriber, Node* node) {
        if (node == nullptr)
            return node;

        if (subscriber < node->data.subscriber)
            node->left = remove(subscriber, node->left);
        else if (subscriber > node->data.subscriber)
            node->right = remove(subscriber, node->right);
        else {
            if ((node->left == nullptr) || (node->right == nullptr)) {
                Node* temp = node->left ? node->left : node->right;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;
                delete temp;
            }
            else {
                Node* temp = minValueNode(node->right);
                node->data = temp->data;
                node->right = remove(temp->data.subscriber, node->right);
            }
        }

        if (node == nullptr)
            return node;

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    void inorderTraversal(Node* node) {
        if (node != nullptr) {
            inorderTraversal(node->left);
            cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

    void preorderTraversal(Node* node) {
        if (node != nullptr) {
            cout << node->data << " ";
            preorderTraversal(node->left);
            preorderTraversal(node->right);
        }
    }

    void postorderTraversal(Node* node) {
        if (node != nullptr) {
            postorderTraversal(node->left);
            postorderTraversal(node->right);
            cout << node->data << " ";
        }
    }

    AVLTree() : root(nullptr) {}

    void insert(Data data) {
        root = insert(data, root);
    }

    void deleteKey(string subscriber) {
        root = remove(subscriber, root);
    }

    void inorderTraversal() {
        inorderTraversal(root);
        cout << endl;
    }

    void preorderTraversal() {
        preorderTraversal(root);
        cout << endl;
    }

    void postorderTraversal() {
        postorderTraversal(root);
        cout << endl;
    }

    Node* find(Node* node, string subscriber) {
        if (node == nullptr || node->data.subscriber == subscriber)
            return node;

        if (subscriber < node->data.subscriber)
            return find(node->left, subscriber);
        else
            return find(node->right, subscriber);
    }

private:
    void createDotFile(Node* node, ofstream& file) {
        if (node == nullptr)
            return;

        file << node->data.subscriber << endl;

        if (node->left) {
            file << node->data.subscriber << " -> " << node->left->data.subscriber << ";" << endl;
            createDotFile(node->left, file);
        }

        if (node->right) {
            file << node->data.subscriber << " -> " << node->right->data.subscriber << ";" << endl;
            createDotFile(node->right, file);
        }
    }

public:
    void visualizeTree(const string& filename) {
        ofstream dotFile(filename);
        if (!dotFile.is_open()) {
            cerr << "Cannot access file:  " << filename << endl;
            return;
        }

        dotFile << "digraph AVLTree {" << endl;
        createDotFile(root, dotFile);
        dotFile << "}" << endl;

        dotFile.close();
        cout << "AVL-Tree graph saved as: " << filename << endl;
    }

    void printTimeTable() {
        double addTime = 0.0, deleteTime = 0.0, searchTime = 0.0;

        for (int i = 0; i < 5; i++) {

            auto start = chrono::high_resolution_clock::now();
            insert(generateRandomData(i));
            auto end = chrono::high_resolution_clock::now();
            double oneElementAddTime = chrono::duration<double, milli>(end - start).count();
            cout << "Addition #" << i + 1 << ": " << oneElementAddTime << " ms" << endl;
            addTime += oneElementAddTime;

            start = chrono::high_resolution_clock::now();
            find(root, generateRandomString());
            end = chrono::high_resolution_clock::now();
            double oneElementSearchTime = chrono::duration<double, milli>(end - start).count();
            cout << "Search #" << i + 1 << ": " << oneElementSearchTime << " ms" << endl;
            searchTime += oneElementSearchTime;

            start = chrono::high_resolution_clock::now();
            remove(generateRandomString(), root);
            end = chrono::high_resolution_clock::now();
            double oneElementDeleteTime = chrono::duration<double, milli>(end - start).count();
            cout << "Deletion #" << i + 1 << ": " << oneElementDeleteTime << " ms" << endl;
            deleteTime += oneElementDeleteTime;
        }

        cout << "Avg. operation times (in ms)" << endl;
        cout << "==================================" << endl;
        cout << "| Operation        | Avg. time   |" << endl;
        cout << "==================================" << endl;
        cout << "| Add              | " << fixed << setprecision(3) << setw(10) << addTime / 5 << " |" << endl;
        cout << "| Search           | " << fixed << setprecision(3) << setw(10) << searchTime / 5 << " |" << endl;
        cout << "| Delete           | " << fixed << setprecision(3) << setw(10) << deleteTime / 5 << " |" << endl;
        cout << "==================================" << endl;
    }
};

class BinaryTreeNode {
private:
    Data data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
public:
    BinaryTreeNode(Data value) : data(value), left(nullptr), right(nullptr) {}

    BinaryTreeNode* getLeft() { return left; }
    BinaryTreeNode* getRight() { return right; }

    void setLeft(BinaryTreeNode* node) { left = node; }
    void setRight(BinaryTreeNode* node) { right = node; }

    Data getData() { return data; }
    void setData(Data setData) { data = setData; }
};

class BinarySearchTree {
private:
    BinaryTreeNode* root;
public:
    BinarySearchTree() : root(nullptr) {}

    BinaryTreeNode* insertRecursive(BinaryTreeNode* node, Data value) {
        if (node == nullptr) {
            return new BinaryTreeNode(value);
        }

        if (value.subscriber < node->getData().subscriber) {
            node->setLeft(insertRecursive(node->getLeft(), value));
        }
        else if (value.subscriber > node->getData().subscriber) {
            node->setRight(insertRecursive(node->getRight(), value));
        }

        return node;
    }

    void insert(Data value) {
        root = insertRecursive(root, value);
    }

    void displayRecursive(BinaryTreeNode* node, ofstream& file) {
        if (node == nullptr) {
            return;
        }

        file << "  " << node->getData().subscriber << ";" << endl;

        if (node->getLeft() != nullptr) {
            file << "  " << node->getData().subscriber << " -> " << node->getLeft()->getData().subscriber << ";" << endl;
            displayRecursive(node->getLeft(), file);
        }

        if (node->getRight() != nullptr) {
            file << "  " << node->getData().subscriber
                << " -> " << node->getRight()->getData().subscriber << ";" << endl;
            displayRecursive(node->getRight(), file);
        }
    }

    void displayAsGraph() {
        ofstream file("tree.dot", ios::out);

        if (!file) {
            cerr << "Cannot access file:" << endl;
            return;
        }

        file << "digraph BinarySearchTree {" << endl;
        displayRecursive(root, file);
        file << "}" << endl;
        file.close();

        cout << "Tree saves as: tree.dot" << endl;
    }

    BinaryTreeNode* findRecursive(BinaryTreeNode* node, Data value) {
        if (node == nullptr || node->getData().subscriber == value.subscriber) {
            return node;
        }

        if (value.subscriber < node->getData().subscriber) {
            return findRecursive(node->getLeft(), value);
        }
        else {
            return findRecursive(node->getRight(), value);
        }
    }

    BinaryTreeNode* find(Data value) {
        return findRecursive(root, value);
    }

    BinaryTreeNode* findMinRecursive(BinaryTreeNode* node) {
        if (node->getLeft() == nullptr) {
            return node;
        }
        return findMinRecursive(node->getLeft());
    }

    BinaryTreeNode* findMin() {
        return findMinRecursive(root);
    }

    BinaryTreeNode* removeRecursive(BinaryTreeNode* node, Data value) {
        if (node == nullptr) {
            return nullptr;
        }

        if (value.subscriber < node->getData().subscriber) {
            node->setLeft(removeRecursive(node->getLeft(), value));
        }
        else if (value.subscriber > node->getData().subscriber) {
            node->setRight(removeRecursive(node->getRight(), value));
        }
        else {
            if (node->getLeft() == nullptr) {
                BinaryTreeNode* temp = node->getRight();
                delete node;
                return temp;
            }
            else if (node->getRight() == nullptr) {
                BinaryTreeNode* temp = node->getLeft();
                delete node;
                return temp;
            }

            node->setData(findMinRecursive(node->getRight())->getData());
            node->setRight(removeRecursive(node->getRight(), node->getData()));
        }

        return node;
    }

    void remove(Data value) {
        root = removeRecursive(root, value);
    }

    void printTimeTable() {
        double addTime = 0.0, deleteTime = 0.0, searchTime = 0.0;

        for (int i = 0; i < 5; i++) {

            auto start = chrono::high_resolution_clock::now();
            insert(generateRandomData(i));
            auto end = chrono::high_resolution_clock::now();
            double oneElementAddTime = chrono::duration<double, milli>(end - start).count();
            cout << "Addition #" << i + 1 << ": " << oneElementAddTime << " ms" << endl;
            addTime += oneElementAddTime;

            start = chrono::high_resolution_clock::now();
            find(generateRandomData(i));
            end = chrono::high_resolution_clock::now();
            double oneElementSearchTime = chrono::duration<double, milli>(end - start).count();
            cout << "Search #" << i + 1 << ": " << oneElementSearchTime << " ms" << endl;
            searchTime += oneElementSearchTime;

            start = chrono::high_resolution_clock::now();
            remove(generateRandomData(i));
            end = chrono::high_resolution_clock::now();
            double oneElementDeleteTime = chrono::duration<double, milli>(end - start).count();
            cout << "Deletion #" << i + 1 << ": " << oneElementDeleteTime << " ms" << endl;
            deleteTime += oneElementDeleteTime;
        }

        cout << "Avg. operation times (in ms)" << endl;
        cout << "==================================" << endl;
        cout << "| Operation        | Avg. time   |" << endl;
        cout << "==================================" << endl;
        cout << "| Add              | " << fixed << setprecision(3) << setw(10) << addTime / 5 << " |" << endl;
        cout << "| Search           | " << fixed << setprecision(3) << setw(10) << searchTime / 5 << " |" << endl;
        cout << "| Delete           | " << fixed << setprecision(3) << setw(10) << deleteTime / 5 << " |" << endl;
        cout << "==================================" << endl;
    }
};


void addMultiplyElements(int elementsAmount, AVLTree& avl, BinarySearchTree& tree) {
    for (int i = 0; i < elementsAmount; i++) {
        Data data = generateRandomData(i);

        avl.insert(data);
        tree.insert(data);
    }
}

int main() {
    SetConsoleOutputCP(65001);

    AVLTree avl;
    BinarySearchTree tree;

    int elementsAmount;

    cout << "How many elements to add?" << endl << " > ";
    cin >> elementsAmount;

    addMultiplyElements(elementsAmount, avl, tree);

    avl.visualizeTree("avl_tree.dot");
    avl.printTimeTable();

    tree.displayAsGraph();
    tree.printTimeTable();

    system("pause");

    return 0;
}
