#include <iostream>
#include <windows.h>
#include <fstream>
#include <random>
#include <ctime>
#include <vector>
#include <chrono>
#include <iomanip>
#include <list>
#include <string>

using namespace std;

struct Data {
    int ip;
    string subscriber;
    string phoneNumber;
    string name;
    int amount;
};

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

class HashTable {
private:
    const int SIZE = 100;
    vector<list<Data>> table;

public:
    HashTable() : table(SIZE) {}

    int hash(const string& key) {
        int third_digit = key[2] - '0';
        int last_digit = key[key.length() - 1] - '0';
        return third_digit * 10 + last_digit;
    }

    void insert(const Data& data) {
        int index = hash(data.phoneNumber);
        table[index].push_back(data);
    }

    bool remove(const string& phoneNumber) {
        int index = hash(phoneNumber);
        auto it = table[index].begin();
        while (it != table[index].end()) {
            if (it->phoneNumber == phoneNumber) {
                table[index].erase(it);
                return true;
            }
            ++it;
        }
        return false;
    }

    bool update(const string& phoneNumber, const Data& newData) {
        int index = hash(phoneNumber);
        auto it = table[index].begin();
        while (it != table[index].end()) {
            if (it->phoneNumber == phoneNumber) {
                *it = newData;
                return true;
            }
            ++it;
        }
        return false;
    }

    Data* find(const string& phoneNumber) {
        int index = hash(phoneNumber);
        auto it = table[index].begin();
        while (it != table[index].end()) {
            if (it->phoneNumber == phoneNumber) {
                return &(*it);
            }
            ++it;
        }
        return nullptr;
    }

    void printTable() {
        cout << "+-------+--------------------------------------------------+" << endl;
        cout << "| hash  |                 Data                             |" << endl;
        cout << "+-------+--------------------------------------------------+" << endl;

        for (int i = 0; i < SIZE; i++) {
            if (!table[i].empty()) {
                cout << "|" << right << setw(6) << i << " |                                                  |" << endl;
                for (const auto& data : table[i]) {
                    cout << "|       |" << setw(3) << data.ip << " " << setw(4) << data.subscriber
                        << " " << setw(13) << data.phoneNumber << " " << setw(18) << data.name
                        << " " << setw(7) << data.amount << " |" << endl;
                }
                cout << "+-------+--------------------------------------------------+" << endl;
            }
        }
    }

    void printTimeTable() {
        double addTime = 0.0, deleteTime = 0.0, searchTime = 0.0, editTime = 0.0;

        auto start = chrono::high_resolution_clock::now();
        auto end = chrono::high_resolution_clock::now();

        ofstream out("result.txt", ios::app);

        for (int i = 0; i < 5; i++) {
            auto start = chrono::high_resolution_clock::now();
            insert(generateRandomData(i));
            auto end = chrono::high_resolution_clock::now();
            double oneElementAddTime = chrono::duration<double, milli>(end - start).count();
            cout << "Addition #" << i << ": " << oneElementAddTime << " ms" << endl;
            addTime += oneElementAddTime;
            out << oneElementAddTime << " ";

            start = chrono::high_resolution_clock::now();
            remove(generateRandomPhoneNumber());
            end = chrono::high_resolution_clock::now();
            double oneElementDeleteTime = chrono::duration<double, milli>(end - start).count();
            cout << "Deletion #" << i << ": " << oneElementDeleteTime << " ms" << endl;
            deleteTime += oneElementDeleteTime;
            out << oneElementDeleteTime << " ";

            start = chrono::high_resolution_clock::now();
            find(generateRandomPhoneNumber());
            end = chrono::high_resolution_clock::now();
            double oneElementSearchTime = chrono::duration<double, milli>(end - start).count();
            cout << "Search #" << i << ": " << oneElementSearchTime << " ms" << endl;
            searchTime += oneElementSearchTime;
            out << oneElementSearchTime << " ";

            start = chrono::high_resolution_clock::now();
            update(generateRandomPhoneNumber(), generateRandomData(0));
            end = chrono::high_resolution_clock::now();
            double oneElementEditTime = chrono::duration<double, milli>(end - start).count();
            cout << "Update #" << i << ": " << oneElementEditTime << " ms" << endl;
            editTime += oneElementEditTime;
            out << oneElementEditTime << " ";
            out << endl;
        }

        out << addTime / 5 << " " << searchTime / 5 << " " << deleteTime / 5 << " " << editTime / 5 << endl;

        cout << "Avg. operations time:" << endl;
        cout << "| Add      | " << fixed << setprecision(3) << setw(12) << addTime / 5 << " |" << endl;
        cout << "| Search   | " << fixed << setprecision(3) << setw(12) << searchTime / 5 << " |" << endl;
        cout << "| Delete   | " << fixed << setprecision(3) << setw(12) << deleteTime / 5 << " |" << endl;
        cout << "| Update   | " << fixed << setprecision(3) << setw(12) << editTime / 5 << " |" << endl;
    }
};

void createHashTable(HashTable& hashTable) {
    int elementsAmount;
    cout << "Enter elements amount: ";
    cin >> elementsAmount;

    for (int i = 0; i < elementsAmount; i++) {
        hashTable.insert(generateRandomData(i));
    }
}

int main() {
    SetConsoleOutputCP(65001);

    HashTable hashTable;

    createHashTable(hashTable);

    hashTable.printTable();

    hashTable.printTimeTable();

    system("pause");

    return 0;
}
