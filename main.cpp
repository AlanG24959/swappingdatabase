#include <iostream>
#include <cassert>
using namespace std;

class Database {
public:
    virtual void store(string str) = 0;
    virtual string retrieve() = 0;
};

class ProductionDatabase : public Database {
public:
    void store(string str) override {
        cout << "I am running some low-level code that stores \"" + str + "\" to an actual, production database!" << endl;
    }

    string retrieve() override {
        cout << "I am retrieving the data from the actual, production database!\n";
        return "I am retrieving the data from the actual, production database!\n";
    }
};

class TestingDatabase : public Database {
private:
    string currentData;

public:
    void store(string str) override {
        cout << "I am storing \"" + str + "\" in memory! This allows me to write unit tests that don't actually reach out to a database on disk!" << endl;
        currentData = str;
    }

    string retrieve() override {
        cout << "I am retrieving data from in memory!\n";
        return currentData;
    }
};

class Application {
private:
    Database* db;

public:
    Application(Database* database) {
        db = database;
    }

    void storeInDatabase(string str) {
        db->store(str);
    }

    string retrieveFromDatabase() {
        return db->retrieve();
    }
};

int main() {
    ProductionDatabase productionDatabase;
    Application productionVersion(&productionDatabase);

    TestingDatabase testingDatabase;
    Application testingVersion(&testingDatabase);

    productionVersion.storeInDatabase("my data");
    testingVersion.storeInDatabase("my data");

    assert(testingVersion.retrieveFromDatabase() == "my data");

    productionVersion.retrieveFromDatabase();
    testingVersion.retrieveFromDatabase();

    return 0;
}
