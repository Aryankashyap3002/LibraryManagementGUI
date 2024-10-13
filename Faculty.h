#ifndef FACULTY_H
#define FACULTY_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include "Book.h"

const std::string FACULTY_MASTER_PASSWORD = "faculty_password";

class Faculty {
public:
    std::string name;
    std::string id;
    std::string password;
    std::vector<Book> borrowedBooks;

    Faculty(std::string name, std::string id, std::string password)
        : name(std::move(name)), id(std::move(id)), password(std::move(password)) {}

    bool authenticate(const std::string& enteredPassword) const;
    void displayBooks() const;
    bool borrowBook(Book& book);
    void returnBook(Book& book);
    
    static void showBooks(const std::vector<Book>& books);
    static void sortBooks(std::vector<Book>& books);
    static Book* searchBook(std::vector<Book>& books, const std::string& bookTitle);
    
    static void facultySignUp(std::vector<Faculty>& faculty);
    static bool facultyLogin(const std::vector<Faculty>& faculty, int& facultyIndex);
    
    static void saveFacultyToCSV(const std::vector<Faculty>& faculty, const std::string& filename);
    static void loadFacultyFromCSV(std::vector<Faculty>& faculty, const std::string& filename, const std::vector<Book>& allBooks);
    
    static void facultyMenu(std::vector<Faculty>& faculty, std::vector<Book>& books, const std::string& facultyFilename);
};

#endif