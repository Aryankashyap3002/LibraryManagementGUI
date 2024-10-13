#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>  
#include <ctime>

class Book {
public:
    int book_id;
    std::vector<int> book_ids;
    std::string title;
    std::string author;
    int year;
    int numberOfCopies;
    bool available;  // Tracks availability

    // Default Constructor
    Book() 
        : book_id(0), title(""), author(""), year(0), numberOfCopies(0), available(true) {}

    // Parameterized Constructor
    Book(const int book_id, const std::string& title, const std::string& author, int year, int numberOfCopies)
        : book_id(book_id), title(title), author(author), year(year), numberOfCopies(numberOfCopies), available(true) {}

    // Print Function
    void print() const {
        std::cout << "Book_ID: " << book_id 
                  << ", Title: " << title 
                  << ", Author: " << author 
                  << ", Year: " << year 
                  << ", No. of Copies: " << numberOfCopies 
                  << ", Available: " << (available ? "Yes" : "No") 
                  << "\n";
    }
};

#endif
