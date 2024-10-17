#include "Student.h"
#include "Sorting.h"
#include "Librarian.h"
#include "IssueRecord.h"
#include "Book.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <ctime>
#include <iomanip>
#include <cctype> 


bool Student::authenticate(const std::string& enteredPassword) const {
    return this->password == enteredPassword;
}

void Student::displayBooks() const {
    if (borrowedBooks.empty()) {
        std::cout << name << " has no borrowed books.\n";
    } else {
        std::cout << "Books borrowed by " << name << ":\n";
        for (const auto& book : borrowedBooks) {
            std::cout << "- " << book.title << " by " << book.author << "\n";
        }
    }
}



bool Student::borrowBook(std::vector<Book>& books, std::vector<IssueRecord>& issues) {
    // Check if the student has already borrowed the maximum number of books
    if (this->borrowedBooksCount >= MAX_BORROWED_BOOKS) {
        std::cout << "You have already borrowed the maximum number of books ("
                  << MAX_BORROWED_BOOKS << ").\n";
        return false;
    }

    // Prompt the user for the book title
    std::string bookTitle;
    std::cout << "Enter the book title to borrow: ";
    std::getline(std::cin >> std::ws, bookTitle);

    // Search for the book in the library
    auto bookIt = std::find_if(books.begin(), books.end(),
        [&bookTitle](const Book& book) { return book.title == bookTitle; });

    if (bookIt == books.end()) {
        std::cout << "Book not found.\n";
        return false;
    }

    // Check if there are available copies
    if (bookIt->numberOfCopies <= 0) {
        std::cout << "Sorry, there are no available copies of this book for borrowing.\n";
        return false;
    }

    // Decrease the number of available copies
    bookIt->numberOfCopies--;

    // Update the book's availability status if it's the last copy
    if (bookIt->numberOfCopies == 0) {
        bookIt->available = false;
    }

    // Generate a unique issue ID
    std::string issueID = "ISS" + std::to_string(issues.size() + 1);

    // Get current time
    std::time_t now = std::time(nullptr);

    // Calculate due date (assuming 14 days borrowing period)
    std::time_t dueDate = now + (14 * 24 * 60 * 60);

    // Create and add the issue record
    IssueRecord newIssue(
        issueID,
        std::to_string(bookIt->book_id),
        this->id,
        "Student",
        now,
        dueDate,
        bookIt->title,
        this->name
    );

    issues.push_back(newIssue);

    // Add the book to the student's borrowed books list
    this->borrowedBooks.push_back(*bookIt);  // Add to borrowed books

    // Increment the student's borrowed book count
    this->borrowedBooksCount++;

    std::cout << "Book '" << bookIt->title << "' borrowed successfully!\n";
    std::cout << "Remaining copies: " << bookIt->numberOfCopies << "\n";

    
    return true;
}


void Student::returnBook(Book& book, std::vector<IssueRecord>& issues) {
    // Check if the book is in the student's borrowed books list
    auto it = std::find_if(borrowedBooks.begin(), borrowedBooks.end(), [&book](const Book& b) {
        return b.title == book.title;
    });

    if (it != borrowedBooks.end()) {
        // Remove the book from the borrowed list
        borrowedBooks.erase(it);
        borrowedBooksCount--;  // Decrement the borrowed books count
        std::cout << name << " returned " << book.title << "\n";
        
        // Update book availability
        book.available = true;
        book.numberOfCopies++;
        
        // Find the corresponding issue record
        auto issueIt = std::find_if(issues.begin(), issues.end(), [&](const IssueRecord& issue) {
            return issue.bookID == std::to_string(book.book_id) && issue.borrowerID == this->id && issue.returnDate == 0;
        });

        if (issueIt != issues.end()) {
            // Set the return date and overdue status
            issueIt->returnDate = std::time(nullptr);
            std::cout<<(issueIt->returnDate - issueIt->dueDate) / (24 * 60 * 60)<<"\n";
            if (issueIt->returnDate > issueIt->dueDate) {
                issueIt->overdueStatus = true;
                int daysOverdue = (issueIt->returnDate - issueIt->dueDate) / (24 * 60 * 60);
                issueIt->fineAmount = daysOverdue * 1.0;
                std::cout << "Book returned " << daysOverdue << " days late. Fine: $" << issueIt->fineAmount << "\n";
            } else {
                issueIt->overdueStatus = false;
                issueIt->fineAmount = 0.0;
            }
            std::cout << "Issue record for " << book.title << " (ID: " << issueIt->issueID << ") has been updated.\n";
        } else {
            std::cout << "Issue record not found for this book.\n";
        }
    } else {
        std::cout << "Book not found in borrowed list.\n";
    }

    // Save the updated issues to the CSV file
}



void Student::showBooks(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "No books are available in the library.\n";
    } else {
        std::cout << "Books available in the library:\n";
        for (const auto& book : books) {
            if (book.available) {
                std::cout << "- " << book.title << " by " << book.author << " (" << book.year << ")\n";
            }
        }
    }
}

void Student::sortBooks(std::vector<Book>& books) {
    quickSort(books, 0, books.size() - 1);
    std::cout << "Books have been sorted alphabetically by title.\n";
}

Book* Student::searchBook(std::vector<Book>& books, const std::string& bookTitle) {
    auto it = std::find_if(books.begin(), books.end(), 
        [&bookTitle](const Book& b) { return b.title == bookTitle; });
    if (it != books.end()) {
        std::cout << "The book '" << bookTitle << "' is " 
                  << (it->available ? "available" : "not available") << " in the library.\n";
        return &(*it);
    } else {
        std::cout << "The book '" << bookTitle << "' is not found in the library.\n";
        return nullptr;
    }
}

void Student::studentSignUp(std::vector<Student>& students) {
    std::string name, id;
    bool flag1;

    while(true) {
        std::cout<<"Enter your name (Name should be all characters and all letters should be capital): ";
        std::cin>> name;
        flag1 = false; 
        for (char c : name) {
            if (!std::isalpha(c) || !std::isupper(c)) {
                flag1 = true; // If character is not a letter or not uppercase, return false
            }
        }

        if (!flag1) {
            break;
        }
    }
    std::cout << "Enter your ID: ";
    std::cin >> id;

    for (const auto& student : students) {
        if (student.id == id) {
            std::cout << "Student already exists! Please try logging in instead.\n";
            return;
        }
    }
    bool flag;
    std::string passwords;

    while (true) {
        std::cout << "Enter your password: ";
        std::cin >> passwords;
        flag = false;  // Reset flag for each iteration

        for (const auto& student : students) {
            if (student.password == passwords) {
                std::cout << "This password is already taken. Try choosing another password.\n";
                flag = true;  // Indicate password was found in the list
                break;
            }
        }

    // If no existing password matches, break out of the loop
        if (!flag) {
            break;
        }
    }

std::cout << "Password is unique. Proceeding...\n";
   
    

    students.emplace_back(name, id, passwords);
    std::cout << "Signup successful!\n";
}

bool Student::studentLogin(const std::vector<Student>& students, int& studentIndex) {
    std::string name, password;

    std::cout << "Enter your name: ";
    std::getline(std::cin >> std::ws, name);  // Read full name, ignoring leading whitespace

    std::cout << "Enter your password: ";
    std::getline(std::cin >> std::ws, password);  // Read password, ignoring leading whitespace

    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].name == name && students[i].authenticate(password)) {
            studentIndex = i;
            return true;  // Login successful
        }
    }
    std::cout<<name << " " << password << "\n";
    return false;  // Login failed
}

void Student::saveStudentsToCSV(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Could not open the file for writing." << std::endl;
        return;
    }

    outFile << "Name,ID,Password,BorrowedBooks,ReturnDays\n";

    for (const auto& student : students) {
        outFile << student.name << "," << student.id << "," << student.password << ",";
        
        // Save borrowed books
        for (size_t i = 0; i < student.borrowedBooks.size(); ++i) {
            outFile << student.borrowedBooks[i].title;
            if (i < student.borrowedBooks.size() - 1) {
                outFile << "|";  // Use a separator between book titles
            }
        }

        outFile << "," << student.returnDays << "\n";
    }
    
    outFile.close();
    std::cout << "All student data saved to " << filename << " successfully!" << std::endl;
}


void Student::loadStudentsFromCSV(std::vector<Student>& students, const std::string& filename, const std::vector<Book>& allBooks) {
    students.clear();
    
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for reading." << std::endl;
        return;
    }

    std::string line;
    bool firstLine = true;  // To skip header

    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;  // Skip header
        }

        if (line.empty()) continue;  // Skip empty lines

        std::stringstream ss(line);
        std::string name, id, password, borrowedBooksStr, returnDaysStr;

        // Read CSV fields
        if (!std::getline(ss, name, ',') || 
            !std::getline(ss, id, ',') || 
            !std::getline(ss, password, ',') || 
            !std::getline(ss, borrowedBooksStr, ',') || 
            !std::getline(ss, returnDaysStr, ',')) {
            std::cerr << "Error: Invalid line format in CSV: " << line << std::endl;
            continue;  // Skip this line and continue with the next
        }

        // Create student
        Student student(name, id, password);

        // Handle return days
        try {
            if (!returnDaysStr.empty()) {
                student.returnDays = std::stoi(returnDaysStr);
            } else {
                student.returnDays = 0;  // Default value if empty
            }
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid return days value: " << returnDaysStr << std::endl;
            student.returnDays = 0;  // Use default value
        }

        // Handle borrowed books
        if (!borrowedBooksStr.empty()) {
            std::stringstream borrowedSS(borrowedBooksStr);
            std::string bookTitle;
            int borrowedBookCount = 0;
            while (std::getline(borrowedSS, bookTitle, '|')) {
                auto it = std::find_if(allBooks.begin(), allBooks.end(),
                    [&bookTitle](const Book& b) { return b.title == bookTitle; });
                if (it != allBooks.end()) {
                    student.borrowedBooks.push_back(*it);
                    borrowedBookCount++;  // Increase count for each borrowed book
                    const_cast<Book&>(*it).available = false;  // Mark book as unavailable
                }
            }
            student.borrowedBooksCount = borrowedBookCount;  // Update count from loaded books
        }

        students.push_back(student);  // Add student to the list
    }
    
    file.close();
    std::cout << "Students loaded from " << filename << " successfully!\n";
}



void Student::studentMenu(std::vector<Book>& books, std::vector<Student>& students, 
                              std::vector<IssueRecord>& issues, const std::string& issueFilename, 
                              const std::string& studentFilename) {

    std::string enteredPassword;
    std::cout << "Enter the master password to continue: ";
    std::cin >> enteredPassword;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer

    if (enteredPassword != MASTER_PASSWORD) {
        std::cout << "Invalid password. Exiting...\n";
        return;
    }

    int choice;
    int currentStudentIndex = -1;

    do {
        std::cout << "\n1. Signup\n2. Login\n3. Exit\nEnter your choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            studentSignUp(students);
            saveStudentsToCSV(students, studentFilename);
        } else if (choice == 2) {
            if (studentLogin(students, currentStudentIndex)) {
                std::cout << "Welcome, " << students[currentStudentIndex].name << "!\n";

                int studentChoice;
                do {
                    std::cout << "\nStudent Menu\n";
                    std::cout << "1. Display Books\n2. Search Book\n3. Sort Books\n";
                    std::cout << "4. Borrow Book\n5. Return Book\n6. View Borrowed Books\n7. Logout\n";
                    std::cout << "Enter your choice: ";
                    
                    if (!(std::cin >> studentChoice)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::string bookTitle;
                    Book* foundBook;
                    switch (studentChoice) {
                        case 1:
                            showBooks(books);
                            break;
                        case 2:
                            std::cout << "Enter the book title to search: ";
                            std::getline(std::cin, bookTitle);
                            searchBook(books, bookTitle);
                            break;
                        case 3:
                            sortBooks(books);
                            break;
                        case 4: {
                                    if (students[currentStudentIndex].borrowBook(books, issues)) {
                                        // Save students data to CSV after successfully borrowing a book
                                        saveStudentsToCSV(students, studentFilename);
                                        // Save the updated issue records to CSV
                                        Librarian::saveIssuesToCSV(issues, issueFilename);
                                        std::cout << "Book borrowed and records updated successfully.\n";
                                    } else {
                                        std::cout << "Unable to borrow the book.\n";
                                    }
                                    break;
                                }

                        case 5:
                            std::cout << "Enter the book title to return: ";
                            std::getline(std::cin, bookTitle);
                            foundBook = searchBook(books, bookTitle);
                            if (foundBook) {
                                students[currentStudentIndex].returnBook(*foundBook, issues);
                                saveStudentsToCSV(students, studentFilename);
                                Librarian::saveIssuesToCSV(issues, issueFilename);
                            }
                            break;
                        case 6:
                            students[currentStudentIndex].displayBooks();
                            break;
                        case 7:
                            std::cout << "Logging out...\n";
                            break;
                        default:
                            std::cout << "Invalid option.\n";
                    }
                } while (studentChoice != 7);  // Logout
            } else {

                std::cout << "Login failed. Try again.\n";
            }
        } else if (choice == 3) {
            std::cout << "Exiting student menu.\n";
        } else {
            std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 3);  // Exit option
}