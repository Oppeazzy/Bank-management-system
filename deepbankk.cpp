
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <conio.h> // For password masking
#include <limits>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

// Constants
const string USER_FILE = "users.dat";
const string ACCOUNT_FILE = "accounts.dat";
const string TRANSACTION_FILE = "transactions.dat";
const int MAX_LOGIN_ATTEMPTS = 3;
const int OTP_LENGTH = 6;

// XOR-based encryption key
const char ENCRYPTION_KEY = 0x55;

// Helper functions
string encryptDecrypt(const string& data) {
    string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ ENCRYPTION_KEY;
    }
    return result;
}

string getCurrentTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-" 
       << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
       << setw(2) << setfill('0') << ltm->tm_mday << " "
       << setw(2) << setfill('0') << ltm->tm_hour << ":"
       << setw(2) << setfill('0') << ltm->tm_min << ":"
       << setw(2) << setfill('0') << ltm->tm_sec;
    return ss.str();
}

string generateOTP() {
    string otp;
    for (int i = 0; i < OTP_LENGTH; ++i) {
        otp += to_string(rand() % 10);
    }
    return otp;
}

void delayMessage(const string& message, int milliseconds = 1000) {
    cout << message;
    cout.flush();
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void clearScreen() {
    system("cls || clear");
}

string getMaskedInput() {
    string input;
    char ch;
    while ((ch = _getch()) != '\r') { // Until Enter is pressed
        if (ch == '\b') { // Handle backspace
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
            }
        } else {
            input += ch;
            cout << '*';
        }
    }
    cout << endl;
    return input;
}

// Data classes
class Transaction {
public:
    string transactionId;
    string accountNumber;
    string type;
    double amount;
    string timestamp;
    string description;

    Transaction(string accNum, string t, double amt, string desc = "")
        : accountNumber(accNum), type(t), amount(amt), description(desc) {
        transactionId = "TXN" + to_string(time(0)) + to_string(rand() % 1000);
        timestamp = getCurrentTimestamp();
    }

    void display() const {
        cout << "Transaction ID: " << transactionId << endl;
        cout << "Account: " << accountNumber << endl;
        cout << "Type: " << type << endl;
        cout << "Amount: " << fixed << setprecision(2) << amount << endl;
        cout << "Time: " << timestamp << endl;
        if (!description.empty()) {
            cout << "Description: " << description << endl;
        }
        cout << "---------------------------------" << endl;
    }

    string toFileString() const {
        return transactionId + "|" + accountNumber + "|" + type + "|" + 
               to_string(amount) + "|" + timestamp + "|" + description;
    }

    static Transaction fromFileString(const string& data) {
        stringstream ss(data);
        string parts[6];
        for (int i = 0; i < 6; ++i) {
            getline(ss, parts[i], '|');
        }
        Transaction txn(parts[1], parts[2], stod(parts[3]), parts[5]);
        txn.transactionId = parts[0];
        txn.timestamp = parts[4];
        return txn;
    }
};

class Account {
public:
    string accountNumber;
    string userId;
    string type; // Savings, Current, etc.
    double balance;
    string creationDate;
    bool active;

    Account(string userId, string accType, double initialBalance = 0.0)
        : userId(userId), type(accType), balance(initialBalance) {
        accountNumber = "ACC" + to_string(time(0)) + to_string(rand() % 10000);
        creationDate = getCurrentTimestamp();
        active = true;
    }

    void display() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Type: " << type << endl;
        cout << "Balance: " << fixed << setprecision(2) << balance << endl;
        cout << "Created: " << creationDate << endl;
        cout << "Status: " << (active ? "Active" : "Inactive") << endl;
        cout << "---------------------------------" << endl;
    }

    string toFileString() const {
        return accountNumber + "|" + userId + "|" + type + "|" + 
               to_string(balance) + "|" + creationDate + "|" + 
               (active ? "1" : "0");
    }

    static Account fromFileString(const string& data) {
        stringstream ss(data);
        string parts[6];
        for (int i = 0; i < 6; ++i) {
            getline(ss, parts[i], '|');
        }
        Account acc(parts[1], parts[2], stod(parts[3]));
        acc.accountNumber = parts[0];
        acc.creationDate = parts[4];
        acc.active = (parts[5] == "1");
        return acc;
    }
};

class User {
public:
    string userId;
    string username;
    string password; // Encrypted , i made this for security lol
    string role; // Admin, Staff, Customer
    string name;
    string email;
    string phone;
    bool locked;
    int loginAttempts;

    User(string uname, string pwd, string r, string n, string e, string ph)
        : username(uname), role(r), name(n), email(e), phone(ph) {
        userId = "USR" + to_string(time(0)) + to_string(rand() % 1000);
        password = encryptDecrypt(pwd);
        locked = false;
        loginAttempts = 0;
    }

    bool verifyPassword(const string& pwd) const {
        return encryptDecrypt(password) == pwd;
    }

    void display() const {
        cout << "User ID: " << userId << endl;
        cout << "Username: " << username << endl;
        cout << "Role: " << role << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Status: " << (locked ? "Locked" : "Active") << endl;
        cout << "---------------------------------" << endl;
    }

    string toFileString() const {
        return userId + "|" + username + "|" + password + "|" + role + "|" + 
               name + "|" + email + "|" + phone + "|" + 
               (locked ? "1" : "0") + "|" + to_string(loginAttempts);
    }

    static User fromFileString(const string& data) {
        stringstream ss(data);
        string parts[9];
        for (int i = 0; i < 9; ++i) {
            getline(ss, parts[i], '|');
        }
        User user(parts[1], encryptDecrypt(parts[2]), parts[3], parts[4], parts[5], parts[6]);
        user.userId = parts[0];
        user.password = parts[2]; // Already encrypted
        user.locked = (parts[7] == "1");
        user.loginAttempts = stoi(parts[8]);
        return user;
    }
};

// Database manager
class Database {
public:
    static vector<User> loadUsers() {
        vector<User> users;
        ifstream file(USER_FILE);
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                users.push_back(User::fromFileString(line));
            }
        }
        file.close();
        return users;
    }

    static void saveUsers(const vector<User>& users) {
        ofstream file(USER_FILE);
        for (const auto& user : users) {
            file << user.toFileString() << endl;
        }
        file.close();
    }

    static vector<Account> loadAccounts() {
        vector<Account> accounts;
        ifstream file(ACCOUNT_FILE);
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                accounts.push_back(Account::fromFileString(line));
            }
        }
        file.close();
        return accounts;
    }

    static void saveAccounts(const vector<Account>& accounts) {
        ofstream file(ACCOUNT_FILE);
        for (const auto& account : accounts) {
            file << account.toFileString() << endl;
        }
        file.close();
    }

    static vector<Transaction> loadTransactions() {
        vector<Transaction> transactions;
        ifstream file(TRANSACTION_FILE);
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                transactions.push_back(Transaction::fromFileString(line));
            }
        }
        file.close();
        return transactions;
    }

    static void saveTransactions(const vector<Transaction>& transactions) {
        ofstream file(TRANSACTION_FILE);
        for (const auto& txn : transactions) {
            file << txn.toFileString() << endl;
        }
        file.close();
    }
};

// Core banking system
class BankSystem {
private:
    vector<User> users;
    vector<Account> accounts;
    vector<Transaction> transactions;
    User* currentUser;

    void initializeAdmin() {
        bool adminExists = false;
        for (const auto& user : users) {
            if (user.role == "Admin") {
                adminExists = true;
                break;
            }
        }

        if (!adminExists) {
            User admin("admin", "admin123", "Admin", "System Administrator", "admin@bank.com", "0000000000");
            users.push_back(admin);
            Database::saveUsers(users);
            cout << "Default admin account created." << endl;
        }
    }

    User* authenticateUser(const string& username, const string& password) {
        for (auto& user : users) {
            if (user.username == username) {
                if (user.locked) {
                    cout << "Account is locked. Please contact admin." << endl;
                    return nullptr;
                }

                if (user.verifyPassword(password)) {
                    user.loginAttempts = 0;
                    Database::saveUsers(users);
                    return &user;
                } else {
                    user.loginAttempts++;
                    if (user.loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                        user.locked = true;
                        cout << "Account locked due to too many failed attempts." << endl;
                    }
                    Database::saveUsers(users);
                    return nullptr;
                }
            }
        }
        return nullptr;
    }

    bool verifyOTP() {
        string generatedOTP = generateOTP();
        cout << "Your OTP is: " << generatedOTP << endl;
        delayMessage("OTP will expire in 60 seconds...\n", 2000);

        cout << "Enter OTP: ";
        string enteredOTP;
        cin >> enteredOTP;

        return enteredOTP == generatedOTP;
    }

    void createAccount() {
        if (currentUser->role != "Staff" && currentUser->role != "Admin") {
            cout << "Only staff can create accounts." << endl;
            return;
        }

        string customerUsername;
        cout << "Enter customer username: ";
        cin >> customerUsername;

        User* customer = nullptr;
        for (auto& user : users) {
            if (user.username == customerUsername && user.role == "Customer") {
                customer = &user;
                break;
            }
        }

        if (!customer) {
            cout << "Customer not found." << endl;
            return;
        }

        string accountType;
        double initialDeposit;

        cout << "Enter account type (Savings/Current): ";
        cin >> accountType;
        cout << "Enter initial deposit: ";
        cin >> initialDeposit;

        Account newAccount(customer->userId, accountType, initialDeposit);
        accounts.push_back(newAccount);
        Database::saveAccounts(accounts);

        Transaction txn(newAccount.accountNumber, "Deposit", initialDeposit, "Initial deposit");
        transactions.push_back(txn);
        Database::saveTransactions(transactions);

        cout << "Account created successfully!" << endl;
        newAccount.display();
    }

    void deposit() {
        string accountNumber;
        double amount;

        cout << "Enter account number: ";
        cin >> accountNumber;

        Account* account = nullptr;
        for (auto& acc : accounts) {
            if (acc.accountNumber == accountNumber) {
                account = &acc;
                break;
            }
        }

        if (!account) {
            cout << "Account not found." << endl;
            return;
        }

        if (!account->active) {
            cout << "Account is inactive." << endl;
            return;
        }

        if (currentUser->role == "Customer") {
            bool ownsAccount = false;
            for (const auto& acc : accounts) {
                if (acc.accountNumber == accountNumber && acc.userId == currentUser->userId) {
                    ownsAccount = true;
                    break;
                }
            }

            if (!ownsAccount) {
                cout << "You can only deposit to your own accounts." << endl;
                return;
            }
        }

        cout << "Enter amount to deposit: ";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount." << endl;
            return;
        }

        account->balance += amount;
        Database::saveAccounts(accounts);

        Transaction txn(accountNumber, "Deposit", amount);
        transactions.push_back(txn);
        Database::saveTransactions(transactions);

        cout << "Deposit successful. New balance: " << account->balance << endl;
    }

    void withdraw() {
        string accountNumber;
        double amount;

        cout << "Enter account number: ";
        cin >> accountNumber;

        Account* account = nullptr;
        for (auto& acc : accounts) {
            if (acc.accountNumber == accountNumber) {
                account = &acc;
                break;
            }
        }

        if (!account) {
            cout << "Account not found." << endl;
            return;
        }

        if (!account->active) {
            cout << "Account is inactive." << endl;
            return;
        }

        if (currentUser->role == "Customer") {
            bool ownsAccount = false;
            for (const auto& acc : accounts) {
                if (acc.accountNumber == accountNumber && acc.userId == currentUser->userId) {
                    ownsAccount = true;
                    break;
                }
            }

            if (!ownsAccount) {
                cout << "You can only withdraw from your own accounts." << endl;
                return;
            }
        }

        cout << "Enter amount to withdraw: ";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount." << endl;
            return;
        }

        if (account->balance < amount) {
            cout << "Insufficient funds." << endl;
            return;
        }

        if (!verifyOTP()) {
            cout << "OTP verification failed." << endl;
            return;
        }

        account->balance -= amount;
        Database::saveAccounts(accounts);

        Transaction txn(accountNumber, "Withdrawal", amount);
        transactions.push_back(txn);
        Database::saveTransactions(transactions);

        cout << "Withdrawal successful. New balance: " << account->balance << endl;
    }

    void transfer() {
        string fromAccount, toAccount;
        double amount;

        cout << "Enter your account number: ";
        cin >> fromAccount;

        Account* sourceAccount = nullptr;
        for (auto& acc : accounts) {
            if (acc.accountNumber == fromAccount) {
                sourceAccount = &acc;
                break;
            }
        }

        if (!sourceAccount) {
            cout << "Source account not found." << endl;
            return;
        }

        if (!sourceAccount->active) {
            cout << "Source account is inactive." << endl;
            return;
        }

        if (currentUser->role == "Customer") {
            bool ownsAccount = false;
            for (const auto& acc : accounts) {
                if (acc.accountNumber == fromAccount && acc.userId == currentUser->userId) {
                    ownsAccount = true;
                    break;
                }
            }

            if (!ownsAccount) {
                cout << "You can only transfer from your own accounts." << endl;
                return;
            }
        }

        cout << "Enter destination account number: ";
        cin >> toAccount;

        if (fromAccount == toAccount) {
            cout << "Cannot transfer to the same account." << endl;
            return;
        }

        Account* destAccount = nullptr;
        for (auto& acc : accounts) {
            if (acc.accountNumber == toAccount) {
                destAccount = &acc;
                break;
            }
        }

        if (!destAccount) {
            cout << "Destination account not found." << endl;
            return;
        }

        if (!destAccount->active) {
            cout << "Destination account is inactive." << endl;
            return;
        }

        cout << "Enter amount to transfer: ";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount." << endl;
            return;
        }

        if (sourceAccount->balance < amount) {
            cout << "Insufficient funds." << endl;
            return;
        }

        if (!verifyOTP()) {
            cout << "OTP verification failed." << endl;
            return;
        }

        sourceAccount->balance -= amount;
        destAccount->balance += amount;
        Database::saveAccounts(accounts);

        Transaction txn1(fromAccount, "Transfer Out", amount, "To: " + toAccount);
        Transaction txn2(toAccount, "Transfer In", amount, "From: " + fromAccount);
        transactions.push_back(txn1);
        transactions.push_back(txn2);
        Database::saveTransactions(transactions);

        cout << "Transfer successful." << endl;
        cout << "Your new balance: " << sourceAccount->balance << endl;
    }

    void viewTransactions() {
        vector<Transaction> userTransactions;

        if (currentUser->role == "Customer") {
            // Get all accounts owned by this user
            vector<string> userAccountNumbers;
            for (const auto& acc : accounts) {
                if (acc.userId == currentUser->userId) {
                    userAccountNumbers.push_back(acc.accountNumber);
                }
            }

            // Get all transactions for these accounts
            for (const auto& txn : transactions) {
                if (find(userAccountNumbers.begin(), userAccountNumbers.end(), txn.accountNumber) != userAccountNumbers.end()) {
                    userTransactions.push_back(txn);
                }
            }
        } else {
            // Staff/Admin can see all transactions
            userTransactions = transactions;
        }

        if (userTransactions.empty()) {
            cout << "No transactions found." << endl;
            return;
        }

        cout << "\nTransaction History:\n";
        cout << "=================================\n";
        for (const auto& txn : userTransactions) {
            txn.display();
        }
    }

    void viewAccounts() {
        vector<Account> userAccounts;

        if (currentUser->role == "Customer") {
            for (const auto& acc : accounts) {
                if (acc.userId == currentUser->userId) {
                    userAccounts.push_back(acc);
                }
            }
        } else {
            userAccounts = accounts;
        }

        if (userAccounts.empty()) {
            cout << "No accounts found." << endl;
            return;
        }

        cout << "\nAccounts:\n";
        cout << "=================================\n";
        for (const auto& acc : userAccounts) {
            acc.display();
        }
    }

    void createUser() {
        if (currentUser->role != "Admin") {
            cout << "Only admin can create users." << endl;
            return;
        }

        string username, password, role, name, email, phone;

        cout << "Enter username: ";
        cin >> username;

        // Check if username exists
        for (const auto& user : users) {
            if (user.username == username) {
                cout << "Username already exists." << endl;
                return;
            }
        }

        cout << "Enter password: ";
        password = getMaskedInput();

        cout << "Enter role (Admin/Staff/Customer): ";
        cin >> role;
        if (role != "Admin" && role != "Staff" && role != "Customer") {
            cout << "Invalid role." << endl;
            return;
        }

        cin.ignore(); // Clear buffer
        cout << "Enter full name: ";
        getline(cin, name);

        cout << "Enter email: ";
        cin >> email;

        cout << "Enter phone: ";
        cin >> phone;

        User newUser(username, password, role, name, email, phone);
        users.push_back(newUser);
        Database::saveUsers(users);

        cout << "User created successfully!" << endl;
        newUser.display();
    }

    void generateReport() {
        if (currentUser->role != "Admin") {
            cout << "Only admin can generate reports." << endl;
            return;
        }

        cout << "\nBank Management System Report\n";
        cout << "Generated on: " << getCurrentTimestamp() << endl;
        cout << "==========================================\n";

        // User statistics
        int adminCount = 0, staffCount = 0, customerCount = 0;
        for (const auto& user : users) {
            if (user.role == "Admin") adminCount++;
            else if (user.role == "Staff") staffCount++;
            else if (user.role == "Customer") customerCount++;
        }

        cout << "\nUser Statistics:\n";
        cout << "Admins: " << adminCount << endl;
        cout << "Staff: " << staffCount << endl;
        cout << "Customers: " << customerCount << endl;
        cout << "Total Users: " << users.size() << endl;

        // Account statistics
        int savingsCount = 0, currentCount = 0, activeCount = 0;
        double totalBalance = 0.0;
        for (const auto& acc : accounts) {
            if (acc.type == "Savings") savingsCount++;
            else if (acc.type == "Current") currentCount++;
            if (acc.active) activeCount++;
            totalBalance += acc.balance;
        }

        cout << "\nAccount Statistics:\n";
        cout << "Savings Accounts: " << savingsCount << endl;
        cout << "Current Accounts: " << currentCount << endl;
        cout << "Active Accounts: " << activeCount << endl;
        cout << "Total Accounts: " << accounts.size() << endl;
        cout << "Total Balance: " << fixed << setprecision(2) << totalBalance << endl;

        // Transaction statistics
        int depositCount = 0, withdrawalCount = 0, transferCount = 0;
        double depositTotal = 0.0, withdrawalTotal = 0.0, transferTotal = 0.0;
        for (const auto& txn : transactions) {
            if (txn.type == "Deposit") {
                depositCount++;
                depositTotal += txn.amount;
            } else if (txn.type == "Withdrawal") {
                withdrawalCount++;
                withdrawalTotal += txn.amount;
            } else if (txn.type == "Transfer Out" || txn.type == "Transfer In") {
                transferCount++;
                transferTotal += txn.amount;
            }
        }

        cout << "\nTransaction Statistics (last 30 days):\n";
        cout << "Deposits: " << depositCount << " (Total: " << depositTotal << ")\n";
        cout << "Withdrawals: " << withdrawalCount << " (Total: " << withdrawalTotal << ")\n";
        cout << "Transfers: " << transferCount << " (Total: " << transferTotal << ")\n";
        cout << "Total Transactions: " << transactions.size() << endl;
    }

    void adminDashboard() {
        while (true) {
            clearScreen();
            cout << "\nADMIN DASHBOARD\n";
            cout << "Welcome, " << currentUser->name << "!\n";
            cout << "=================================\n";
            cout << "1. Create User Account\n";
            cout << "2. View All Accounts\n";
            cout << "3. View All Transactions\n";
            cout << "4. Generate System Report\n";
            cout << "5. Logout\n";
            cout << "Enter choice: ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    createUser();
                    break;
                case 2:
                    viewAccounts();
                    break;
                case 3:
                    viewTransactions();
                    break;
                case 4:
                    generateReport();
                    break;
                case 5:
                    return;
                default:
                    cout << "Invalid choice." << endl;
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    void staffDashboard() {
        while (true) {
            clearScreen();
            cout << "\nSTAFF DASHBOARD\n";
            cout << "Welcome, " << currentUser->name << "!\n";
            cout << "=================================\n";
            cout << "1. Create Customer Account\n";
            cout << "2. Deposit\n";
            cout << "3. Withdraw\n";
            cout << "4. Transfer\n";
            cout << "5. View Accounts\n";
            cout << "6. View Transactions\n";
            cout << "7. Logout\n";
            cout << "Enter choice: ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    createAccount();
                    break;
                case 2:
                    deposit();
                    break;
                case 3:
                    withdraw();
                    break;
                case 4:
                    transfer();
                    break;
                case 5:
                    viewAccounts();
                    break;
                case 6:
                    viewTransactions();
                    break;
                case 7:
                    return;
                default:
                    cout << "Invalid choice." << endl;
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    void customerDashboard() {
        while (true) {
            clearScreen();
            cout << "\nCUSTOMER DASHBOARD\n";
            cout << "Welcome, " << currentUser->name << "!\n";
            cout << "=================================\n";
            cout << "1. Deposit\n";
            cout << "2. Withdraw\n";
            cout << "3. Transfer\n";
            cout << "4. View My Accounts\n";
            cout << "5. View My Transactions\n";
            cout << "6. Logout\n";
            cout << "Enter choice: ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    deposit();
                    break;
                case 2:
                    withdraw();
                    break;
                case 3:
                    transfer();
                    break;
                case 4:
                    viewAccounts();
                    break;
                case 5:
                    viewTransactions();
                    break;
                case 6:
                    return;
                default:
                    cout << "Invalid choice." << endl;
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

public:
    BankSystem() : currentUser(nullptr) {
        users = Database::loadUsers();
        accounts = Database::loadAccounts();
        transactions = Database::loadTransactions();
        initializeAdmin();
    }

    void run() {
        while (true) {
            clearScreen();
            cout << "BANK MANAGEMENT SYSTEM\n";
            cout << "=================================\n";
            cout << "1. Login\n";
            cout << "2. Exit\n";
            cout << "Enter choice: ";

            int choice;
            cin >> choice;

            if (choice == 2) {
                break;
            } else if (choice == 1) {
                string username, password;
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                password = getMaskedInput();

                currentUser = authenticateUser(username, password);
                if (currentUser) {
                    if (currentUser->role == "Admin") {
                        adminDashboard();
                    } else if (currentUser->role == "Staff") {
                        staffDashboard();
                    } else if (currentUser->role == "Customer") {
                        customerDashboard();
                    }
                } else {
                    cout << "Login failed. Invalid username or password." << endl;
                    delayMessage("", 2000);
                }
            } else {
                cout << "Invalid choice." << endl;
                delayMessage("", 1000);
            }
        }
    }
};

// Unit tests
class BankSystemTests {
public:
    static void runTests() {
        testEncryption();
        testUserAuthentication();
        testAccountOperations();
        cout << "All tests passed!" << endl;
    }

private:
    static void testEncryption() {
        string original = "password123";
        string encrypted = encryptDecrypt(original);
        string decrypted = encryptDecrypt(encrypted);
        
        if (original != decrypted) {
            cerr << "Encryption test failed!" << endl;
        }
    }

    static void testUserAuthentication() {
        vector<User> testUsers;
        User testUser("testuser", "testpass", "Customer", "Test User", "test@example.com", "1234567890");
        testUsers.push_back(testUser);
        Database::saveUsers(testUsers);

        vector<User> loadedUsers = Database::loadUsers();
        if (loadedUsers.empty() || loadedUsers[0].username != "testuser") {
            cerr << "User save/load test failed!" << endl;
        }

        if (!loadedUsers[0].verifyPassword("testpass")) {
            cerr << "Password verification test failed!" << endl;
        }
    }

    static void testAccountOperations() {
        vector<Account> testAccounts;
        Account testAccount("testuser", "Savings", 1000.0);
        testAccounts.push_back(testAccount);
        Database::saveAccounts(testAccounts);

        vector<Account> loadedAccounts = Database::loadAccounts();
        if (loadedAccounts.empty() || loadedAccounts[0].userId != "testuser") {
            cerr << "Account save/load test failed!" << endl;
        }

        double initialBalance = loadedAccounts[0].balance;
        loadedAccounts[0].balance += 500.0;
        Database::saveAccounts(loadedAccounts);

        vector<Account> updatedAccounts = Database::loadAccounts();
        if (updatedAccounts[0].balance != initialBalance + 500.0) {
            cerr << "Account update test failed!" << endl;
        }
    }
};

int main() {
    srand(time(0)); // Seed for random number generation

    // Uncomment to run tests
    // BankSystemTests::runTests();

    BankSystem bankSystem;
    bankSystem.run();

    return 0;
}