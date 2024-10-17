#include "Faculty.h"
#include "Sorting.h"
#include "Librarian.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <ctime>
#include <iomanip>

bool Faculty::facultyAuthenticate(const std::string& enteredPassword) const {
    return this->password == enteredPassword;
}

void Faculty::facultyDisplayBooks() const {
    if (borrowedBooks.empty()) {
        std::cout << name << " has no borrowed books.\n";
    } else {
        std::cout << "Books borrowed by " << name << ":\n";
        for (const auto& book : borrowedBooks) {
            std::cout << "- " << book.title << " by " << book.author << "\n";
        }
    }
}

bool Faculty::facultyBorrowBook(std::vector<Book>& books, std::vector<IssueRecord>& issues) {
    

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
        "Faculty",
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

void Faculty::facultyReturnBook(Book& book, std::vector<IssueRecord>& issues) {
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
    Librarian::saveIssuesToCSV(issues, "issues.csv");
}

void Faculty::facultyShowBooks(const std::vector<Book>& books) {
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

void Faculty::facultySortBooks(std::vector<Book>& books) {
    quickSort(books, 0, books.size() - 1);
    std::cout << "Books have been sorted alphabetically by title.\n";
}

Book* Faculty::facultySearchBook(std::vector<Book>& books, const std::string& bookTitle) {
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

void Faculty::facultySignUp(std::vector<Faculty>& faculty) {
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

    for (const auto& fac : faculty) {
        if (fac.name == name || fac.id == id) {
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

        for (const auto& fac : faculty) {
            if (fac.password == passwords) {
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

    faculty.emplace_back(name, id, passwords);
    std::cout << "Signup successful!\n";
}

bool Faculty::facultyLogin(const std::vector<Faculty>& faculty, int& facultyIndex) {
    std::string name, password;

    std::cout << "Enter your name: ";
    std::getline(std::cin >> std::ws, name);  // Read full name, ignoring leading whitespace

    std::cout << "Enter your password: ";
    std::getline(std::cin >> std::ws, password);  // Read password, ignoring leading whitespace

    for (size_t i = 0; i < faculty.size(); ++i) {
        if (faculty[i].name == name && faculty[i].facultyAuthenticate(password)) {
            facultyIndex = i;
            return true;  // Login successful
        }
    }
    std::cout<<name << " " << password << "\n";
    return false;  // Login failed
}

void Faculty::saveFacultyToCSV(const std::vector<Faculty>& faculty, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Could not open the file for writing." << std::endl;
        return;
    }

    outFile << "Name,ID,Password,BorrowedBooks,ReturnDays\n";

    for (const auto& fac : faculty) {
        outFile << fac.name << "," << fac.id << "," << fac.password << ",";
        
        // Save borrowed books
        for (size_t i = 0; i < fac.borrowedBooks.size(); ++i) {
            outFile << fac.borrowedBooks[i].title;
            if (i < fac.borrowedBooks.size() - 1) {
                outFile << "|";  // Use a separator between book titles
            }
        }

        outFile << "," << fac.returnDays << "\n";
    }
    
    outFile.close();
    std::cout << "All student data saved to " << filename << " successfully!" << std::endl;
}

void Faculty::loadFacultyFromCSV(std::vector<Faculty>& faculty, const std::string& filename, const std::vector<Book>& allBooks) {
    faculty.clear();
    
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

        // Create studentStudent
         Faculty fac(name, id, password);

        // Handle return days
        try {
            if (!returnDaysStr.empty()) {
                fac.returnDays = std::stoi(returnDaysStr);
            } else {
                fac.returnDays = 0;  // Default value if empty
            }
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid return days value: " << returnDaysStr << std::endl;
            fac.returnDays = 0;  // Use default value
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
                    fac.borrowedBooks.push_back(*it);
                    borrowedBookCount++;  // Increase count for each borrowed book
                    const_cast<Book&>(*it).available = false;  // Mark book as unavailable
                }
            }
            fac.borrowedBooksCount = borrowedBookCount;  // Update count from loaded books
        }

        faculty.push_back(fac);  // Add fac to the list
    }
    
    file.close();
    std::cout << "Faculty loaded from " << filename << " successfully!\n";
}


void Faculty::facultyMenu(std::vector<Book>& books, std::vector<Faculty>& faculty, 
                            std::vector<IssueRecord>& issues, const std::string& issueFilename, 
                            const std::string& facultyFilename) {
    std::string enteredPassword;
    std::cout << "Enter the faculty master password to continue: ";
    std::cin >> enteredPassword;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer

    if (enteredPassword != FACULTY_MASTER_PASSWORD) {
        std::cout << "Invalid password. Exiting...\n";
        return;
    }

    int choice;
    int currentFacultyIndex = -1;

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
            facultySignUp(faculty);
            saveFacultyToCSV(faculty, facultyFilename);
        } else if (choice == 2) {
            if (facultyLogin(faculty, currentFacultyIndex)) {
                std::cout << "Welcome, " << faculty[currentFacultyIndex].name << "!\n";

                int facultyChoice;
                do {
                    std::cout << "\nFaculty Menu\n";
                    std::cout << "1. Display Books\n2. Search Book\n3. Sort Books\n";
                    std::cout << "4. Borrow Book\n5. Return Book\n6. View Borrowed Books\n7. Logout\n";
                    std::cout << "Enter your choice: ";
                    
                    if (!(std::cin >> facultyChoice)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::string bookTitle;
                    Book* foundBook;
                    switch (facultyChoice) {
                        case 1:
                            facultyShowBooks(books);
                            break;
                        case 2:
                            std::cout << "Enter the book title to search: ";
                            std::getline(std::cin, bookTitle);
                            facultySearchBook(books, bookTitle);
                            break;
                        case 3:
                            facultySortBooks(books);
                            break;
                        case 4:
                            if (faculty[currentFacultyIndex].facultyBorrowBook(books, issues)) {
                                saveFacultyToCSV(faculty, facultyFilename);
                                Librarian::saveIssuesToCSV(issues, issueFilename);
                                std::cout << "Book borrowed and records updated successfully.\n";
                            } else {
                                std::cout << "Unable to borrow the book.\n";
                            }
                            break;
                        case 5:
                            std::cout << "Enter the book title to return: ";
                            std::getline(std::cin, bookTitle);
                            foundBook = facultySearchBook(books, bookTitle);
                            if (foundBook) {
                                faculty[currentFacultyIndex].facultyReturnBook(*foundBook, issues);
                                saveFacultyToCSV(faculty, facultyFilename);
                                Librarian::saveIssuesToCSV(issues, issueFilename);
                            }
                            break;
                        case 6:
                            faculty[currentFacultyIndex].facultyDisplayBooks();
                            break;
                        case 7:
                            std::cout << "Logging out...\n";
                            break;
                        default:
                            std::cout << "Invalid option.\n";
                    }
                } while (facultyChoice != 7);  // Logout
            } else {
                std::cout << "Login failed. Try again.\n";
            }
        } else if (choice == 3) {
            std::cout << "Exiting faculty menu.\n";
        } else {
            std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 3);  // Exit option
}