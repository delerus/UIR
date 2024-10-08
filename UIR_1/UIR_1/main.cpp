#include <iostream>
#include <windows.h>
#include <fstream>
#include <random>
#include <ctime>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>

#include "Data.hpp"

using namespace std;

class ListNode {
private:
    Data data;
    ListNode* next;
public:
    ListNode(Data value) : data(value), next(nullptr) {}

    ListNode* getNext() { return next; }
    void setNext(ListNode* node) { next = node; }
    Data getData() { return data; }
};

class List {
private:
    ListNode* begin;
    ListNode* end;  // Новый указатель на конец списка

public:
    List() : begin(nullptr), end(nullptr) {}

    void add(Data value) {
        ListNode* newNode = new ListNode(value);

        if (begin == nullptr) {  // Если список пуст
            begin = newNode;
            end = newNode;  // Устанавливаем конец на первый элемент
        }
        else {
            end->setNext(newNode);  // Присоединяем новый элемент в конец
            end = newNode;  // Обновляем конец списка
        }
    }

    void displayAsGraph() {
        ofstream file("list.dot", ios::out);

        if (!file) {
            cerr << "Error opening file." << endl;
            return;
        }

        file << "digraph List {" << endl;

        ListNode* current = begin;
        while (current != nullptr) {
            file << current->getData().subscriber << ";" << endl;
            if (current->getNext() != nullptr) {
                file << current->getData().subscriber << " -> " << current->getNext()->getData().subscriber << ";" << endl;
            }
            current = current->getNext();
        }

        file << "}" << endl;
        file.close();

        cout << "List saved in file: list.dot" << endl;
    }

    ListNode* searchElement(string subscriber) {
        ListNode* element = begin;
        for (; element != nullptr; element = element->getNext()) {
            if (element->getData().subscriber == subscriber) {
                return element;
            }
        }
        return nullptr;
    }

    void deleteElement(string subscriber) {
        if (!begin) {
            cout << "The list is empty." << endl;
            return;
        }

        if (begin->getData().subscriber == subscriber) {
            ListNode* temp = begin;
            begin = begin->getNext();
            delete temp;

            if (begin == nullptr) {  // Если удален последний элемент
                end = nullptr;  // Обнуляем указатель на конец
            }
            return;
        }

        ListNode* current = begin;
        while (current->getNext() != nullptr) {
            if (current->getNext()->getData().subscriber == subscriber) {
                ListNode* temp = current->getNext();
                current->setNext(current->getNext()->getNext());
                delete temp;

                if (current->getNext() == nullptr) {  // Если удален последний элемент
                    end = current;  // Обновляем указатель на конец
                }
                return;
            }
            current = current->getNext();
        }

        cout << "Element not found." << endl;
    }

    void printTimeTable() {
        double addTime = 0.0, deleteTime = 0.0, searchTime = 0.0;

        for (int i = 0; i < 5; i++) {

            auto start = chrono::high_resolution_clock::now();
            add(generateRandomData(i));
            auto end = chrono::high_resolution_clock::now();
            double oneElementAddTime = chrono::duration<double, milli>(end - start).count();
            cout << "Addition #" << i + 1 << ": " << oneElementAddTime << " ms" << endl;
            addTime += oneElementAddTime;

            start = chrono::high_resolution_clock::now();
            searchElement(generateRandomString());
            end = chrono::high_resolution_clock::now();
            double oneElementSearchTime = chrono::duration<double, milli>(end - start).count();
            cout << "Search #" << i + 1 << ": " << oneElementSearchTime << " ms" << endl;
            searchTime += oneElementSearchTime;

            start = chrono::high_resolution_clock::now();
            deleteElement(generateRandomString());
            end = chrono::high_resolution_clock::now();
            double oneElementDeleteTime = chrono::duration<double, milli>(end - start).count();
            cout << "Deletion #" << i + 1 << ": " << oneElementDeleteTime << " ms" << endl;
            deleteTime += oneElementDeleteTime;
        }

        cout << "Average operation times (in ms):" << endl;
        cout << "===================================" << endl;
        cout << "| Operation        | Avg. time    |" << endl;
        cout << "===================================" << endl;
        cout << "| Add              | " << fixed << setprecision(3) << setw(10) << addTime / 5 << " |" << endl;
        cout << "| Search           | " << fixed << setprecision(3) << setw(10) << searchTime / 5 << " |" << endl;
        cout << "| Delete           | " << fixed << setprecision(3) << setw(10) << deleteTime / 5 << " |" << endl;
        cout << "===================================" << endl;
    }
};


class TreeNode {
private:
    Data data;
    TreeNode* left;
    TreeNode* right;

public:
    TreeNode(Data value) : data(value), left(nullptr), right(nullptr) {}

    TreeNode* getLeft() { return left; }
    TreeNode* getRight() { return right; }
    void setLeft(TreeNode* node) { left = node; }
    void setRight(TreeNode* node) { right = node; }
    Data getData() { return data; }
};

class Tree {
private:
    TreeNode* root;

public:
    Tree() : root(nullptr) {}

    void add(Data value) {
        if (root == nullptr) {
            root = new TreeNode(value);
        }
        else {
            addRecursively(root, value);
        }
    }

    void addRecursively(TreeNode* node, Data value) {
        if (value.subscriber < node->getData().subscriber) {
            if (node->getLeft() == nullptr) {
                node->setLeft(new TreeNode(value));
            }
            else {
                addRecursively(node->getLeft(), value);
            }
        }
        else {
            if (node->getRight() == nullptr) {
                node->setRight(new TreeNode(value));
            }
            else {
                addRecursively(node->getRight(), value);
            }
        }
    }


    void displayAsGraph() {
        ofstream file("tree.dot", ios::out);

        if (!file) {
            cerr << "Error opening file." << endl;
            return;
        }

        file << "digraph Tree {" << endl;

        displayNodeAsGraph(file, root);

        file << "}" << endl;
        file.close();

        cout << "Tree saved in file: tree.dot" << endl;
    }

    void displayNodeAsGraph(ofstream& file, TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        // Записываем текущий узел в файл
        file << node->getData().subscriber << ";" << endl;

        // Если у узла есть левый дочерний элемент, записываем связь
        if (node->getLeft() != nullptr) {
            file << node->getData().subscriber << " -> " << node->getLeft()->getData().subscriber << ";" << endl;
            displayNodeAsGraph(file, node->getLeft());
        }

        // Если у узла есть правый дочерний элемент, записываем связь
        if (node->getRight() != nullptr) {
            file << node->getData().subscriber << " -> " << node->getRight()->getData().subscriber << ";" << endl;
            displayNodeAsGraph(file, node->getRight());
        }
    }


    // Поиск элемента в дереве
    TreeNode* searchElement(string subscriber) {
        return searchRecursive(root, subscriber);
    }

    TreeNode* searchRecursive(TreeNode* node, string subscriber) {
        if (node == nullptr || node->getData().subscriber == subscriber) {
            return node;
        }

        if (subscriber < node->getData().subscriber) {
            return searchRecursive(node->getLeft(), subscriber);
        }
        else {
            return searchRecursive(node->getRight(), subscriber);
        }
    }

    // Удаление элемента из дерева
    void deleteElement(string subscriber) {
        root = deleteRecursive(root, subscriber);
    }

    TreeNode* deleteRecursive(TreeNode* node, string subscriber) {
        if (node == nullptr) {
            return node;
        }

        if (subscriber < node->getData().subscriber) {
            node->setLeft(deleteRecursive(node->getLeft(), subscriber));
        }
        else if (subscriber > node->getData().subscriber) {
            node->setRight(deleteRecursive(node->getRight(), subscriber));
        }
        else {
            if (node->getLeft() == nullptr) {
                TreeNode* temp = node->getRight();
                delete node;
                return temp;
            }
            else if (node->getRight() == nullptr) {
                TreeNode* temp = node->getLeft();
                delete node;
                return temp;
            }

            TreeNode* temp = findMin(node->getRight());
            node->getData() = temp->getData();
            node->setRight(deleteRecursive(node->getRight(), temp->getData().subscriber));
        }

        return node;
    }

    TreeNode* findMin(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->getLeft() != nullptr) {
            current = current->getLeft();
        }
        return current;
    }

    void printTimeTable() {
        double addTime = 0.0, deleteTime = 0.0, searchTime = 0.0;

        for (int i = 0; i < 5; i++) {
            auto start = chrono::high_resolution_clock::now();
            add(generateRandomData(i));
            auto end = chrono::high_resolution_clock::now();
            double oneElementAddTime = chrono::duration<double, milli>(end - start).count();
            cout << "Addition #" << i + 1 << ": " << oneElementAddTime << " ms" << endl;
            addTime += oneElementAddTime;

            start = chrono::high_resolution_clock::now();
            searchElement(generateRandomString());
            end = chrono::high_resolution_clock::now();
            double oneElementSearchTime = chrono::duration<double, milli>(end - start).count();
            cout << "Search #" << i + 1 << ": " << oneElementSearchTime << " ms" << endl;
            searchTime += oneElementSearchTime;

            start = chrono::high_resolution_clock::now();
            deleteElement(generateRandomString());
            end = chrono::high_resolution_clock::now();
            double oneElementDeleteTime = chrono::duration<double, milli>(end - start).count();
            cout << "Deletion #" << i + 1 << ": " << oneElementDeleteTime << " ms" << endl;
            deleteTime += oneElementDeleteTime;
        }

        cout << "Average operation times (in ms):" << endl;
        cout << "===================================" << endl;
        cout << "| Operation        | Avg. time    |" << endl;
        cout << "===================================" << endl;
        cout << "| Add              | " << fixed << setprecision(3) << setw(10) << addTime / 5 << " |" << endl;
        cout << "| Search           | " << fixed << setprecision(3) << setw(10) << searchTime / 5 << " |" << endl;
        cout << "| Delete           | " << fixed << setprecision(3) << setw(10) << deleteTime / 5 << " |" << endl;
        cout << "===================================" << endl;
    }
};

void addMultipleElements(int elementsAmount, Tree& tree) {
    for (int i = 0; i < elementsAmount; i++) {
        Data data = generateRandomData(i);
        tree.add(data);
    }
}

void addMultipleElements(int elementsAmount, List& list) {
    for (int i = 0; i < elementsAmount; i++) {
        Data data = generateRandomData(i);
        list.add(data);
    }
}

int main() {
    List list;
    Tree tree;

    int elementsAmount;

    cout << "How many elements to add?" << endl << "Amount: ";
    cin >> elementsAmount;

    cout << "List:" << endl;
    addMultipleElements(elementsAmount, list);
    list.printTimeTable();
    list.displayAsGraph();

    cout << "Tree:" << endl;
    addMultipleElements(elementsAmount, tree);
    tree.printTimeTable();
    tree.displayAsGraph();


    return 0;
}
