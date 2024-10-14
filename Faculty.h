#ifndef FACULTY_H
#define FACULTY_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>
#include "Book.h"
#include "IssueRecord.h"

const std::string FACULTY_MASTER_PASSWORD = "faculty_password";

class Faculty {
public:
    std::string name;
    std::string id;
    std::string password;
    std::vector<Book> borrowedBooks;
    int returnDays;
    int borrowedBooksCount;

    Faculty(std::string name, std::string id, std::string password)
        : name(std::move(name)), id(std::move(id)), password(std::move(password)), returnDays(0), borrowedBooksCount(0)  {}

    bool facultyAuthenticate(const std::string& enteredPassword) const;
    void facultyDisplayBooks() const;
    bool facultyBorrowBook(std::vector<Book>& books, std::vector<IssueRecord>& issues);
    void facultyReturnBook(Book& book, std::vector<IssueRecord>& issues);
    
    static void facultyShowBooks(const std::vector<Book>& books);
    static void facultySortBooks(std::vector<Book>& books);
    static Book* facultySearchBook(std::vector<Book>& books, const std::string& bookTitle);
    int getBorrowedBooksCount() const { return borrowedBooksCount; }


    static void facultySignUp(std::vector<Faculty>& faculty);
    static bool facultyLogin(const std::vector<Faculty>& faculty, int& facultyIndex);
    
    static void saveFacultyToCSV(const std::vector<Faculty>& faculty, const std::string& filename);
    static void loadFacultyFromCSV(std::vector<Faculty>& faculty, const std::string& filename, const std::vector<Book>& allBooks);
    
    static void facultyMenu(std::vector<Book>& books, std::vector<Faculty>& faculty, 
                            std::vector<IssueRecord>& issues, const std::string& issueFilename, 
                            const std::string& facultyFilename);
};

#endif