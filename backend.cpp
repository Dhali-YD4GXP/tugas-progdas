#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

string DB_PATH = "contacts.json";

//LOAD DATABASE 
json loadDB() {
    ifstream file(DB_PATH);
    if (!file.good()) return json::array();
    json data;
    file >> data;
    return data;
}

// SAVE DATABASE 
void saveDB(const json& data) {
    ofstream file(DB_PATH);
    file << data.dump(4);
}

//  MAIN 
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "{\"error\":\"no command\"}";
        return 0;
    }

    string cmd = argv[1];
    json db = loadDB();

    // ADD CONTACT
    if (cmd == "add") {
        if (argc < 6) {
            cout << "{\"error\":\"missing fields\"}";
            return 0;
        }

        json contact = {
            {"name", argv[2]},
            {"phone", argv[3]},
            {"email", argv[4]},
            {"address", argv[5]}
        };

        db.push_back(contact);
        saveDB(db);

        cout << "{\"status\":\"ok\"}";
        return 0;
    }

    // LIST CONTACTS
    if (cmd == "list") {
        cout << db.dump();
        return 0;
    }

    // SEARCH
    if (cmd == "search") {
        if (argc < 3) {
            cout << "{\"error\":\"missing name\"}";
            return 0;
        }

        string name = argv[2];
        json result = json::array();

        for (auto &c : db) {
            if (c["name"].get<string>().find(name) != string::npos) {
                result.push_back(c);
            }
        }

        cout << result.dump();
        return 0;
    }

    // DELETE
    if (cmd == "delete") {
        if (argc < 3) {
            cout << "{\"error\":\"missing name\"}";
            return 0;
        }

        string n = argv[2];
        auto it = remove_if(db.begin(), db.end(),
            [&](json &c) { return c["name"] == n; });

        if (it == db.end()) {
            cout << "{\"error\":\"not found\"}";
            return 0;
        }

        db.erase(it, db.end());
        saveDB(db);

        cout << "{\"status\":\"deleted\"}";
        return 0;
    }

    // EDIT
    if (cmd == "edit") {
        if (argc < 7) {
            cout << "{\"error\":\"missing fields\"}";
            return 0;
        }

        string oldname = argv[2];
        string newname = argv[3];
        string phone = argv[4];
        string email = argv[5];
        string address = argv[6];

        bool found = false;

        for (auto &c : db) {
            if (c["name"] == oldname) {
                c["name"] = newname;
                c["phone"] = phone;
                c["email"] = email;
                c["address"] = address;
                found = true;
            }
        }

        if (!found) {
            cout << "{\"error\":\"not found\"}";
            return 0;
        }

        saveDB(db);
        cout << "{\"status\":\"edited\"}";
        return 0;
    }

    cout << "{\"error\":\"unknown command\"}";
    return 0;
}
