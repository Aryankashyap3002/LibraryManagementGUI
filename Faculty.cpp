#include "Faculty.h"
#include "Sorting.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

bool Faculty::authenticate(const std::string& enteredPassword) const {
    return this->password == enteredPassword;
}

void Faculty::displayBooks() const {
    if (borrowedBooks.empty()) {
        std::cout << name << " has no borrowed books.\n";
    } else {
        std::cout << "Books borrowed by " << name << ":\n";
        for (const auto& book : borrowedBooks) {
            std::cout << "- " << book.title << " by " << book.author << "\n";
        }
    }
}

bool Faculty::borrowBook(Book& book) {
    borrowedBooks.push_back(book);
    std::cout << name << " borrowed " << book.title << "\n";
    book.available = false;
    return true;
}

void Faculty::returnBook(Book& book) {
    auto it = std::find_if(borrowedBooks.begin(), borrowedBooks.end(),
        [&book](const Book& b) { return b.title == book.title; });
    if (it != borrowedBooks.end()) {
        borrowedBooks.erase(it);
        std::cout << name << " returned " << book.title << "\n";
        book.available = true;
    } else {
        std::cout << "Book not found in borrowed list.\n";
    }
}

void Faculty::showBooks(const std::vector<Book>& books) {
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

void Faculty::sortBooks(std::vector<Book>& books) {
    quickSort(books, 0, books.size() - 1);
    std::cout << "Books have been sorted alphabetically by title.\n";
}

Book* Faculty::searchBook(std::vector<Book>& books, const std::string& bookTitle) {
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
    std::string name, id, password;
    std::cout << "Enter your name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Enter your ID: ";
    std::cin >> id;

    for (const auto& f : faculty) {
        if (f.name == name || f.id == id) {
            std::cout << "Faculty member already exists! Please try logging in instead.\n";
            return;
        }
    }

    std::cout << "Enter your password: ";
    std::cin >> password;

    faculty.emplace_back(name, id, password);
    std::cout << "Signup successful!\n";
}

bool Faculty::facultyLogin(const std::vector<Faculty>& faculty, int& facultyIndex) {
    std::string name, password;
    std::cout << "Enter your name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Enter your password: ";
    std::cin >> password;

    for (size_t i = 0; i < faculty.size(); ++i) {
        if (faculty[i].name == name && faculty[i].authenticate(password)) {
            facultyIndex = i;
            return true;
        }
    }
    return false;
}

void Faculty::saveFacultyToCSV(const std::vector<Faculty>& faculty, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Could not open the file for writing." << std::endl;
        return;
    }

    outFile << "Name,ID,Password,BorrowedBooks\n";

    for (const auto& f : faculty) {
        outFile << f.name << "," << f.id << "," << f.password << ",";
        
        // Save borrowed books
        for (size_t i = 0; i < f.borrowedBooks.size(); ++i) {
            outFile << f.borrowedBooks[i].title;
            if (i < f.borrowedBooks.size() - 1) {
                outFile << "|";  // Use a separator between book titles
            }
        }
        
        outFile << "\n";
    }

    outFile.close();
    std::cout << "All faculty data saved to " << filename << " successfully!" << std::endl;
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
        std::string name, id, password, borrowedBooksStr;

        // Read CSV fields
        if (!std::getline(ss, name, ',') || 
            !std::getline(ss, id, ',') || 
            !std::getline(ss, password, ',') || 
            !std::getline(ss, borrowedBooksStr, ',')) {
            std::cerr << "Error: Invalid line format in CSV: " << line << std::endl;
            continue;  // Skip this line and continue with the next
        }

        // Create faculty member
        Faculty f(name, id, password);

        // Handle borrowed books
        if (!borrowedBooksStr.empty()) {
            std::stringstream borrowedSS(borrowedBooksStr);
            std::string bookTitle;
            while (std::getline(borrowedSS, bookTitle, '|')) {
                auto it = std::find_if(allBooks.begin(), allBooks.end(),
                    [&bookTitle](const Book& b) { return b.title == bookTitle; });
                if (it != allBooks.end()) {
                    f.borrowedBooks.push_back(*it);
                    const_cast<Book&>(*it).available = false;
                }
            }
        }

        faculty.push_back(f);
    }

    file.close();
    std::cout << "Faculty loaded from " << filename << " successfully!\n";
}

void Faculty::facultyMenu(std::vector<Faculty>& faculty, std::vector<Book>& books, const std::string& facultyFilename) {
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
                        case 4:
                            std::cout << "Enter the book title to borrow: ";
                            std::getline(std::cin, bookTitle);
                            foundBook = searchBook(books, bookTitle);
                            if (foundBook && foundBook->available) {
                                faculty[currentFacultyIndex].borrowBook(*foundBook);
                                saveFacultyToCSV(faculty, facultyFilename);
                            } else if (foundBook) {
                                std::cout << "Sorry, this book is not available for borrowing.\n";
                            }
                            break;
                        case 5:
                            std::cout << "Enter the book title to return: ";
                            std::getline(std::cin, bookTitle);
                            foundBook = searchBook(books, bookTitle);
                            if (foundBook) {
                                faculty[currentFacultyIndex].returnBook(*foundBook);
                                saveFacultyToCSV(faculty, facultyFilename);
                            }
                            break;
                        case 6:
                            faculty[currentFacultyIndex].displayBooks();
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