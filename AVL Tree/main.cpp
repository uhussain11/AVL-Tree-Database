#include <iostream>
#include "AVL.h"
#include <regex>
#include <string>

using namespace std;

int main() {
    AVLTree t;
    int commands = 0;
    string command;
    string name;
    int ufidInt;
    bool result;

    regex insert(R"(^insert \"(.+)\" (\d{8})$)");
    regex remove(R"(^remove (\d{8})$)");
    regex removeInorder(R"(^removeInorder (\d+)$)");
    regex searchByName("^search \"([a-zA-Z ]+)\"$");
    regex searchByUFID(R"(^search (\d{8})$)");
    regex printLevelCount(R"(^printLevelCount$)");
    regex printInorder(R"(^printInorder$)");
    regex printPostorder(R"(^printPostorder$)");
    regex printPreorder(R"(^printPreorder$)");

    cin >> commands;
    cin.ignore();

    for (int i = 0; i < commands; i++) {
        getline(cin, command);
        smatch match;

        if (regex_match(command, match, insert)) {
            name = match[1].str();
            ufidInt = stoi(match[2].str());
            result = t.Insert(name, ufidInt);
            cout << (result ? "successful" : "unsuccessful") << endl;
        }
        else if (regex_match(command, match, remove)) {
            ufidInt = stoi(match[1].str());
            result = t.Delete(ufidInt);
            cout << (result ? "successful" : "unsuccessful") << endl;
        }
        else if (regex_match(command, match, removeInorder)) {
            int N = stoi(match[1].str());
            t.removeInorder(N);
            cout << "successful" << endl;
        }
        else if (regex_match(command, match, searchByName)) {
            name = match[1].str();
            string searchResult = t.searchByName(name);
            cout << searchResult << endl;
        }

        else if (regex_match(command, match, searchByUFID)) {
            ufidInt = stoi(match[1].str());
            cout << t.searchByID(ufidInt) << endl;
        }

        else if (regex_match(command, match, printLevelCount)) {
            cout << t.printLevelCount() << endl;
        }
        else if (regex_match(command, match, printInorder)) {
            cout << t.printInorder() << endl;
        }
        else if (regex_match(command, match, printPostorder)) {
            cout << t.printPostorder() << endl;
        }
        else if (regex_match(command, match, printPreorder)) {
            cout << t.printPreorder() << endl;
        }
    }

    return 0;
}
