#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

class BankAccount {
private:
    string accountNumber;
    string accountHolder;
    double balance;

public:
    void createAccount() {
        while (true) {
            cout << "Enter 16-digit Account Number: ";
            cin >> accountNumber;
            if (accountNumber.length() != 16 || !all_of(accountNumber.begin(), accountNumber.end(), ::isdigit)) {
                cout << "Invalid input! Account number must be exactly 16 digits.\n";
            } else {
                // Check for duplicate account number before proceeding
                ifstream checkFile("accounts.txt");
                string line;
                bool duplicate = false;
                while (getline(checkFile, line)) {
                    stringstream ss(line);
                    string accNo;
                    getline(ss, accNo, ',');
                    if (accNo == accountNumber) {
                        duplicate = true;
                        break;
                    }
                }
                checkFile.close();
                if (duplicate) {
                    cout << "Error: Account number already exists. Please try again.\n";
                    return;
                }
                break;
            }
        }
        cout << "\n" << "Enter Account Holder Name: ";
        cin.ignore(); // clear newline character left in input buffer
        getline(cin, accountHolder);
        cout << "\n" << "Enter Initial Balance: ";
        cin >> balance;
        cout << "\n" << "Account Created Successfully!\n";
        ofstream outFile("accounts.txt", ios::app);
        if (outFile.is_open()) {
            outFile << accountNumber << "," << accountHolder << "," << fixed << setprecision(2) << balance << "\n";
            outFile.close();
        } else {
            cout << "Error: Could not save account to database.\n";
        }
    }

    void showAccount() const {
        cout << "\nAccount Number: " << accountNumber;
        cout << "\nAccount Holder: " << accountHolder;
        cout << fixed << setprecision(2);
        cout << "\nBalance: ₹" << fixed << setprecision(2) << balance << "\n";
    }

    void deposit(double amount) {
        balance += amount;
        cout << "₹" << fixed << setprecision(2) << amount << " Deposited. New Balance: ₹" << fixed << setprecision(2) << balance << "\n";
        updateDatabase();
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient Balance!\n";
        } else {
            balance -= amount;
            cout << "₹" << fixed << setprecision(2) << amount << " Withdrawn. New Balance: ₹" << fixed << setprecision(2) << balance << "\n";
            updateDatabase();
        }
    }

    string getAccountNumber() const {
        return accountNumber;
    }

private:
    void updateDatabase() const {
        ifstream inFile("accounts.txt");
        ofstream tempFile("temp.txt");
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string accNo, name, bal;
            getline(ss, accNo, ',');
            getline(ss, name, ',');
            getline(ss, bal);
            if (accNo == accountNumber) {
                tempFile << accountNumber << "," << accountHolder << "," << fixed << setprecision(2) << balance << "\n";
            } else {
                tempFile << line << "\n";
            }
        }
        inFile.close();
        tempFile.close();
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
    }
};

int main() {
    const int MAX_ACCOUNTS = 100;
    BankAccount accounts[MAX_ACCOUNTS];
    int totalAccounts = 0;

    int choice;

    do {
        cout << "\n====== BANK MANAGEMENT SYSTEM ======\n";
        cout << "1. Create Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Search by Account Number\n";
        cout << "6. Delete Account\n";
        cout << "7. Exit\n";
        cout << "Enter your choice (1-7): ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); // clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                if (totalAccounts < MAX_ACCOUNTS) {
                    accounts[totalAccounts].createAccount();
                    totalAccounts++;
                } else { 
                    cout << "Account limit reached!\n";
                }
                break;

            case 2: {
                ifstream inFile("accounts.txt");
                string line;
                if (!inFile) {
                    cout << "No accounts found.\n";
                    break;
                }
                cout << "\n--- Account List ---\n";
                while (getline(inFile, line)) {
                    stringstream ss(line);
                    string accNo, name, bal;
                    getline(ss, accNo, ',');
                    getline(ss, name, ',');
                    getline(ss, bal);
                    cout << "\nAccount Number: " << accNo;
                    cout << "\nAccount Holder: " << name;
                    cout << "\nBalance: ₹" << fixed << setprecision(2) << stod(bal) << "\n";
                }
                inFile.close();
                break;
            }

            case 3: {
                string accNo;
                double amount;
                cout << "Enter Account Number: ";
                cin >> accNo;
                bool found = false;
                for (int i = 0; i < totalAccounts; i++) {
                    if (accounts[i].getAccountNumber() == accNo) {
                        cout << "Enter amount to deposit: ";
                        cin >> amount;
                        accounts[i].deposit(amount);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account not found!\n";
                break;
            }

            case 4: {
                string accNo;
                double amount;
                cout << "Enter Account Number: ";
                cin >> accNo;
                bool found = false;
                for (int i = 0; i < totalAccounts; i++) {
                    if (accounts[i].getAccountNumber() == accNo) {
                        cout << "Enter amount to withdraw: ";
                        cin >> amount;
                        accounts[i].withdraw(amount);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account not found!\n";
                break;
            }

            case 5: {
                string accNo;
                cout << "Enter Account Number: ";
                cin >> accNo;
                bool found = false;
                for (int i = 0; i < totalAccounts; i++) {
                    if (accounts[i].getAccountNumber() == accNo) {
                        accounts[i].showAccount();
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account not found!\n";
                break;
            }

            case 6: {
                string accNo;
                cout << "Enter Account Number to delete: ";
                cin >> accNo;

                ifstream inFile("accounts.txt");
                ofstream tempFile("temp.txt");
                string line;
                bool found = false;

                while (getline(inFile, line)) {
                    stringstream ss(line);
                    string fileAccNo;
                    getline(ss, fileAccNo, ',');
                    if (fileAccNo != accNo) {
                        tempFile << line << "\n";
                    } else {
                        found = true;
                    }
                }

                inFile.close();
                tempFile.close();
                remove("accounts.txt");
                rename("temp.txt", "accounts.txt");

                if (found) {
                    cout << "Account " << accNo << " deleted successfully.\n";
                } else {
                    cout << "Account not found.\n";
                }
                break;
            }

            case 7:
                cout << "Exiting... Thank you!\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 7);

    return 0;
}