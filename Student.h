#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include "Book.h"
#include "IssueRecord.h"

const std::string MASTER_PASSWORD = "common_password";
const int MAX_BORROWED_BOOKS = 3;  // New constant for book limit

class Student {
public:
    std::string name;
    std::string id;
    std::string password;
    std::vector<Book> borrowedBooks;
    int returnDays;
    int borrowedBooksCount;

    Student(std::string name, std::string id, std::string password)
        : name(std::move(name)), id(std::move(id)), password(std::move(password)), returnDays(0), borrowedBooksCount(0) {}

    bool authenticate(const std::string& enteredPassword) const;
    void displayBooks() const;
    bool borrowBook(std::vector<Book>& books, std::vector<IssueRecord>& issues);
    void returnBook(Book& book);
    int getBorrowedBooksCount() const { return borrowedBooksCount; }  // New helper function
    
    static void showBooks(const std::vector<Book>& books);
    static void sortBooks(std::vector<Book>& books);
    static Book* searchBook(std::vector<Book>& books, const std::string& bookTitle);
    
    static void studentSignUp(std::vector<Student>& students);
    static bool studentLogin(const std::vector<Student>& students, int& studentIndex);
    
    static void saveStudentsToCSV(const std::vector<Student>& students, const std::string& filename);
    static void loadStudentsFromCSV(std::vector<Student>& students, const std::string& filename, const std::vector<Book>& allBooks);
    
    static void studentMenu(std::vector<Book>& books, std::vector<Student>& students, 
                              std::vector<IssueRecord>& issues, const std::string& issueFilename, 
                              const std::string& studentFilename);
};

#endif