#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct Voter {
    string id;
    string password;
    int hasVoted;
    string name;
};

const string VOTER_FILE = "voters.txt";
const string RESULT_FILE = "results.txt";

int candidateAVotes = 0;
int candidateBVotes = 0;

// ---------- Utility ----------
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ---------- Load Results ----------
void loadResults() {
    ifstream fin(RESULT_FILE);
    if (fin >> candidateAVotes >> candidateBVotes) {
        // Loaded successfully
    } else {
        candidateAVotes = 0;
        candidateBVotes = 0;
    }
    fin.close();
}

// ---------- Save Results ----------
void saveResults() {
    ofstream fout(RESULT_FILE);
    fout << candidateAVotes << " " << candidateBVotes;
    fout.close();
}

// ---------- Check Voter ----------
bool voterExists(const string& id) {
    ifstream fin(VOTER_FILE);
    Voter v;

    while (fin >> v.id >> v.password >> v.hasVoted) {
        fin.ignore();
        getline(fin, v.name);
        if (v.id == id) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

// ---------- Register ----------
void registerVoter() {
    Voter v;
    cout << "\n--- Voter Registration ---\n";

    cout << "Enter National ID: ";
    cin >> v.id;

    if (voterExists(v.id)) {
        cout << "This voter is already registered!\n";
        return;
    }

    cout << "Enter Password: ";
    cin >> v.password;

    clearInput();
    cout << "Enter Full Name: ";
    getline(cin, v.name);

    v.hasVoted = 0;

    ofstream fout(VOTER_FILE, ios::app);
    fout << v.id << " " << v.password << " " << v.hasVoted << " " << v.name << endl;
    fout.close();

    cout << "Registration successful!\n";
}

// ---------- Login ----------
bool login(string& activeID) {
    string id, pass;
    cout << "\n--- Login ---\n";
    cout << "ID: ";
    cin >> id;
    cout << "Password: ";
    cin >> pass;

    ifstream fin(VOTER_FILE);
    Voter v;

    while (fin >> v.id >> v.password >> v.hasVoted) {
        fin.ignore();
        getline(fin, v.name);

        if (v.id == id && v.password == pass) {
            activeID = id;
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

// ---------- Voting ----------
void vote(const string& voterID) {
    ifstream fin(VOTER_FILE);
    ofstream temp("temp.txt");

    Voter v;
    bool found = false, alreadyVoted = false;

    while (fin >> v.id >> v.password >> v.hasVoted) {
        fin.ignore();
        getline(fin, v.name);

        if (v.id == voterID) {
            found = true;
            if (v.hasVoted == 1)
                alreadyVoted = true;
            else
                v.hasVoted = 1;
        }

        temp << v.id << " " << v.password << " " << v.hasVoted << " " << v.name << endl;
    }

    fin.close();
    temp.close();

    if (!found || alreadyVoted) {
        remove("temp.txt");
        cout << "You are not allowed to vote again!\n";
        return;
    }

    remove(VOTER_FILE.c_str());
    rename("temp.txt", VOTER_FILE.c_str());

    int choice;
    cout << "\n--- Candidates ---\n";
    cout << "1. Candidate A\n";
    cout << "2. Candidate B\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (cin.fail() || (choice != 1 && choice != 2)) {
        clearInput();
        cout << "Invalid vote!\n";
        return;
    }

    if (choice == 1)
        candidateAVotes++;
    else
        candidateBVotes++;

    saveResults(); 

    cout << "Vote recorded successfully!\n";
}

// ---------- Show Results ----------
void showResults() {
    loadResults();  // 

    cout << "\n--- Election Results ---\n";
    cout << "Candidate A: " << candidateAVotes << " votes\n";
    cout << "Candidate B: " << candidateBVotes << " votes\n";
}

// ---------- Main ----------
int main() {
    int choice;
    string activeUser;

    loadResults();  

    do {
        cout << "\n===== SIMPLE E-VOTING SYSTEM =====\n";
        cout << "1. Register\n";
        cout << "2. Login & Vote\n";
        cout << "3. View Results\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input!\n";
            continue;
        }

        switch (choice) {
            case 1:
                registerVoter();
                break;
            case 2:
                if (login(activeUser))
                    vote(activeUser);
                else
                    cout << "Login failed!\n";
                break;
            case 3:
                showResults();
                break;
            case 0:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid option!\n";
        }

    } while (choice != 0);

    return 0;
}
