#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include <vector>
#include "Book.h"
#include "Student.h"
#include "Faculty.h"
#include "IssueRecord.h" 
class Student;


class Librarian {
public:
    // Menu function
    static void librarianMenu(std::vector<Book>& books, std::vector<Student>& students, 
                              std::vector<IssueRecord>& issues, std::vector<int> book_ids, const std::string& bookFilename, 
                              const std::string& issueFilename);
    static void showLibrarianMenu();  // Helper function to show menu
    
    // Authentication
    static bool authenticate(const std::string& password);
    
    // Book management
    static void addBook(std::vector<Book>& books, std::vector<int> book_ids);
    static void deleteBook(std::vector<Book>& books);
    static void modifyBook(std::vector<Book>& books);
    static void displayBooks(const std::vector<Book>& books);

    // CSV handling functions
    static void saveBooksToCSV(const std::vector<Book>& books, const std::string& filename,
                              std::vector<IssueRecord>& issues);
    static void loadBooksFromCSV(std::vector<Book>& books, const std::string& filename);
    static void issueBook(std::vector<IssueRecord>& issues, std::vector<Book>& books, 
                          const std::vector<Student>& students, const std::vector<Faculty>& faculty);
    static void viewIssuedBooks(const std::vector<IssueRecord>& issues);
    static void viewBorrowings(const std::vector<IssueRecord>& issues);
    static void saveIssuesToCSV(const std::vector<IssueRecord>& issues, const std::string& filename);
    static void loadIssuesFromCSV(std::vector<IssueRecord>& issues, const std::string& filename);
    static std::string generateIssueID();
    static std::string convertUnixToDate(time_t timestamp);
    static int countCurrentBorrowings(const std::vector<IssueRecord>& issues, const std::string& borrowerID);
    static int countCurrentBookIssued(const std::vector<IssueRecord>& issues, const std::string& bookID);
private:
    static const std::string PASSWORD;
    
};

#endif
