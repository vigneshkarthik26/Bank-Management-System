# Bank Management System in C++

A simple console based bank management system built using C++ that allows users to create, view, deposit into, withdraw from, search, and delete bank accounts. Data is persistently stored in a local text-based database file (`accounts.txt`).

## Features
- Create new bank accounts (with 16-digit account number validation)
- Prevent duplicate account numbers
- View all existing accounts from file
- Deposit money (reflected in the local database)
- withdraw money (reflected in the local database)
- Search for account details by account number
- Delete account from the system
- Persistent storage using 'accounts.txt'

## Technologies Used
- C++ Standard Libraries ('iostream', 'fstream', 'sstream', 'iomanip', 'string')
- File I/O for data persistence
- OOP (Object-Oriented Programming)

## File Structure
BankManagementSystem/
|--- bankSys.cpp   #Main Source code
|___accounts.tct   #Database file (auto-generated)

## Example account entry format
1234567890123456,John Doe,5000.00
4567890123456789,John Appleseed,10000.00
