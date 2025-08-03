
🏦 Bank Management System (C++)
📌 Overview
The Bank Management System is a console-based application built in C++ that simulates real-world banking operations. It allows different types of users (Admin, Staff, Customer) to perform various actions securely.
This project showcases Object-Oriented Programming (OOP), file handling, basic encryption, and role-based access control, making it an excellent learning project and a base for future enhancements.

🚀 Features
Feature	Status
Role-based dashboards (Admin, Staff, Customer)	✅
Login with password masking	✅
Account lockout after 3 failed logins	✅
OTP verification (mock)	✅
Account creation (by staff and admin)	✅
Deposits, withdrawals, transfers	✅
Transaction logs with timestamps	✅
File persistence for users, accounts, transactions	✅
Summary reports (Admin)	✅
Unit testing framework	✅
Encryption for password/PIN (XOR-based)	✅
ASCII UI effects (loading/delay messages)	✅
Modular OOP-based C++ design	✅

🏗️ Architecture
Core Classes:

User → Base class (with derived classes Admin, Staff, Customer)

Account → Holds account details (balance, account number, owner)

Transaction → Records transactions with timestamps

Database → Handles users, accounts, transactions, and file persistence

Data Files:

users.dat → Stores encrypted user credentials

accounts.dat → Stores account balances and ownership

transactions.log → Logs all transactions with timestamps

🛠️ Technologies Used
Programming Language: C++

Concepts: OOP, File Handling, Encapsulation, Basic Security

Libraries:

<iostream>, <fstream> → Input/Output & file persistence

<ctime> → Timestamp generation

<conio.h> → Password masking

<thread>, <chrono> → Delay effects for UI

Clone this repository or download the source code.

Compile the project using any modern C++ compiler:

👥 User Roles
Admin

Can create new users (Admin, Staff, Customer)

Can view all transactions

Can generate system summary reports

Staff

Can create customer accounts

Can view transaction history

Customer

Can check balance, deposit, withdraw, transfer funds

Can view their own transaction history

🔒 Security Features
Password masking during login

Account lockout after 3 failed login attempts

OTP verification (mock)

Basic XOR encryption for password/PIN storage

🧪 Unit Testing
The system includes a simple unit testing framework to validate:

Account creation

Login authentication

Transaction processing

File persistence

📈 Future Enhancements
GUI integration using graphics.h

Web-based version with database integration (MySQL/SQLite)

Stronger password encryption (SHA256/BCrypt)

Real-time notifications and multi-threaded processing

🖥️ Example Usage

=== Bank Management System ===
1. Login
2. Create Account
Choice: 2
Username: john
Password: ******
Account created successfully.

Login:
Username: john
Password: ******
OTP: 123456
Input OTP: 123456
Login successful!

Customer Dashboard:
1. Check Balance
2. Deposit
3. Withdraw
4. Transfer
5. View Transactions
0. Logout
Choice: 1
Balance: 5000.00

🤝 Contribution
Contributions are welcome!
Feel free to fork this repository, open issues, and submit pull requests to enhance functionality.

📄 License
This project is licensed under the MIT License. lolllllllll








