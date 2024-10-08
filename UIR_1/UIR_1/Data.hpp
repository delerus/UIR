#ifndef DATA_HPP
#define DATA_HPP

#include <iostream>
#include <string>
#include <random>
#include <vector>

struct Data {
    int ip;
    std::string subscriber;
    std::string phoneNumber;
    std::string name;
    int amount;

    friend std::ostream& operator<<(std::ostream& os, const Data& data);
    friend std::istream& operator>>(std::istream& is, Data& data);
};

std::ostream& operator<<(std::ostream& os, const Data& data) {
    os << data.ip << " " << data.subscriber << " " << data.phoneNumber << " " << data.name << " " << data.amount;
    return os;
}

std::istream& operator>>(std::istream& is, Data& data) {
    is >> data.ip >> data.subscriber >> data.phoneNumber >> data.name >> data.amount;
    return is;
}

std::string generateRandomPhoneNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::string phoneNumber = "";
    for (int i = 0; i < 10; i++) {
        if (i == 0) {
            phoneNumber += std::to_string(dis(gen) + 1);
        }
        else {
            phoneNumber += std::to_string(dis(gen));
        }
    }

    return "+7" + phoneNumber;
}

std::vector<std::string> firstNames = {
    "Liam", "Emma", "Noah", "Olivia", "William", "Ava", "James", "Isabella",
    "Oliver", "Sophia", "Benjamin", "Mia", "Elijah", "Charlotte", "Lucas", "Harper"
};

std::vector<std::string> lastNames = {
    "Smith", "Johnson", "Williams", "Brown", "Davis", "Miller", "Wilson", "Moore",
    "Taylor", "Anderson", "Thomas", "Jackson", "White", "Harris", "Martin", "Thompson"
};

std::string generateRandomName() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> firstNameDist(0, firstNames.size() - 1);
    static std::uniform_int_distribution<> lastNameDist(0, lastNames.size() - 1);

    std::string firstName = firstNames[firstNameDist(gen)];
    std::string lastName = lastNames[lastNameDist(gen)];

    return firstName + " " + lastName;
}

int generateRandomNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);

    return dis(gen);
}

std::string generateRandomString() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> charDist(48, 57);

    std::string randomString;
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

#endif // DATA_HPP
