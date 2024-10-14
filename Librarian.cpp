#include "Librarian.h"
#include "Book.h"
#include "Sorting.h"
#include "Searching.h"
#include "IssueRecord.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <set>
#include <fstream>
#include <sstream>
#include <map>

const std::string Librarian::PASSWORD = "librarypass";


bool Librarian::authenticate(const std::string& password) {
    return password == PASSWORD;
}

void Librarian::addBook(std::vector<Book>& books, std::vector<int> book_ids) {
    Book book;
    std::srand(std::time(0));
    
    // Generate a unique book_id
    int new_book_id;
    bool is_unique;
    do {
        new_book_id = std::rand() % 10000 + 1; // Generates a random number between 1 and 10000
        is_unique = true;

        // Check if book_id is already present in book_ids
        for (int id : book_ids) {
            if (id == new_book_id) {
                is_unique = false;  // If ID is found in the vector, it's not unique
                break;
            }
        }
    } while (!is_unique);

    // Set the unique book_id
    book.book_id = new_book_id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the buffer

    std::cout << "Enter book title: ";
    std::getline(std::cin, book.title);

    std::cout << "Enter book author: ";
    std::getline(std::cin, book.author);

    std::cout << "Enter publication year: ";
    std::cin >> book.year;

    std::cout << "Number of copies: ";
    std::cin >> book.numberOfCopies;

    book.available = true;  // Set availability to true when adding a new book

    books.push_back(book);
    std::cout << book.book_id << book.title << book.numberOfCopies;
    std::cout << "Book added successfully!\n";

    
}



void Librarian::deleteBook(std::vector<Book>& books) {
    // Implementation from the existing deleteBook function
     if (books.empty()) {
        std::cout << "No books available to delete.\n";
        return;
    }

    std::string searchTerm;
    std::cout << "Enter the title or author of the book to delete: ";
    std::cin.ignore();
    std::getline(std::cin, searchTerm);

    auto it = std::find_if(books.begin(), books.end(), [&searchTerm](const Book& book) {
        return book.title == searchTerm || book.author == searchTerm;
    });

    if (it != books.end()) {
        std::cout << "Book found: \n";
        it->print();  // Display the book details before deletion

        // Confirm deletion
        char confirm;
        std::cout << "Are you sure you want to delete this book? (y/n): ";
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            books.erase(it);  // Remove the book from the vector
            std::cout << "Book deleted successfully!\n";
        } else {
            std::cout << "Deletion cancelled.\n";
        }
    } else {
        std::cout << "Book not found.\n";
    }
}

// Function to display books in the system
void Librarian::displayBooks(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "No books available.\n";
        return;
    }
    for (const auto& book : books) {
        book.print();
    }
}

void Librarian::modifyBook(std::vector<Book>& books) {
    // Implementation from the existing modifyBook function
    // ...
    if (books.empty()) {
        std::cout << "No books available to modify.\n";
        return;
    }

    std::string searchTerm;
    std::cout << "Enter the title or author of the book you want to modify: ";
    std::cin.ignore();
    std::getline(std::cin, searchTerm);

    // Search for the book
    bool found = false;
    for (auto& book : books) {
        if (book.title == searchTerm || book.author == searchTerm) {
            std::cout << "Book found:\n";
            book.print();

            std::cout << "What do you want to modify?\n";
            std::cout << "1. Title\n";
            std::cout << "2. Author\n";
            std::cout << "3. Year\n";
            std::cout << "4. All fields\n";
            std::cout << "Enter your choice: ";
            int choice;
            std::cin >> choice;

            std::cin.ignore();  // To ignore the newline character left by std::cin

            switch (choice) {
                case 1:
                    std::cout << "Enter new title: ";
                    std::getline(std::cin, book.title);
                    break;
                case 2:
                    std::cout << "Enter new author: ";
                    std::getline(std::cin, book.author);
                    break;
                case 3:
                    std::cout << "Enter new year: ";
                    std::cin >> book.year;
                    break;
                case 4:
                    std::cout << "Enter new title: ";
                    std::getline(std::cin, book.title);
                    std::cout << "Enter new author: ";
                    std::getline(std::cin, book.author);
                    std::cout << "Enter new year: ";
                    std::cin >> book.year;
                    break;
                default:
                    std::cout << "Invalid choice, no changes made.\n";
                    return;
            }

            std::cout << "Book updated successfully!\n";
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "No matching book found for the search term.\n";
    }
}

void Librarian::viewBorrowings(const std::vector<IssueRecord>& issues) {
    std::cout << "Current Borrowings:\n\n";
    
    std::time_t now = std::time(nullptr);
    
    for (const auto& issue : issues) {
        if (issue.returnDate == 0) {  // Book hasn't been returned yet
            std::cout << "Borrower: " << issue.borrowerName << " (ID: " << issue.borrowerID << ", Type: " << issue.borrowerType << ")\n";
            std::cout << "Book: " << issue.bookTitle << " (ID: " << issue.bookID << ")\n";
            std::cout << "Borrowed on: " << std::put_time(std::localtime(&issue.borrowDate), "%Y-%m-%d") << "\n";
            std::cout << "Due on: " << std::put_time(std::localtime(&issue.dueDate), "%Y-%m-%d") << "\n";
            
            int daysLeft = (issue.dueDate - now) / (24 * 60 * 60);
            if (daysLeft > 0) {
                std::cout << "Days left to return: " << daysLeft << "\n";
            } else {
                std::cout << "Overdue by " << -daysLeft << " days\n";
            }
            std::cout << "\n";
        }
    }
}




void Librarian::saveBooksToCSV(const std::vector<Book>& books, const std::string& filename) {
    // First, read all existing books into a map
    std::map<int, Book> existingBooks;
    std::ifstream inFile(filename);
    std::string line;
    
    if (inFile) {
        std::getline(inFile, line);  // Skip header
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string book_id_str, title, author, year_str, copies_str;
            
            std::getline(ss, book_id_str, ',');
            std::getline(ss, title, ',');
            std::getline(ss, author, ',');
            std::getline(ss, year_str, ',');
            std::getline(ss, copies_str, ',');
            
            
            
            try {
                int book_id = std::stoi(book_id_str);
                int year = std::stoi(year_str);
                int numberOfCopies = std::stoi(copies_str);
                existingBooks[book_id] = Book(book_id, title, author, year, numberOfCopies);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid number format in line: " << line << std::endl;
                continue;  // Skip this line
            }

            
        }
        inFile.close();
    }

    // Update existing books and add new ones
    for (const auto& book : books) {
        existingBooks[book.book_id] = book;  // This will update existing or add new
    }

    // Write all books back to the file
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Could not open the file for writing." << std::endl;
        return;
    }

    // Write header
    outFile << "book_id,title,author,year,numberOfCopies\n";

    // Write book data
    for (const auto& pair : existingBooks) {
        const Book& book = pair.second;
        outFile << book.book_id << "," << book.title << "," << book.author << "," 
                << book.year << "," << book.numberOfCopies << "\n";
    }

    outFile.close();
    std::cout << "Books saved to " << filename << " successfully!" << std::endl;
}



// Load books from CSV file
void Librarian::loadBooksFromCSV(std::vector<Book>& books, const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error: Could not open the file " << filename << " for reading." << std::endl;
        return;
    }

    std::string line;
    std::getline(inFile, line);  // Skip the header

    while (std::getline(inFile, line)) {
        if (line.empty()) continue;  // Skip empty lines

        std::stringstream ss(line);
        std::string book_id_str, title, author, year_str, copies_str;
        int book_id, year, numberOfCopies;

        // Parse each field separated by commas
        std::getline(ss, book_id_str, ',');
        std::getline(ss, title, ',');
        std::getline(ss, author, ',');
        std::getline(ss, year_str, ',');
        std::getline(ss, copies_str, ',');

        // Convert string fields to integers
        try {
            book_id = std::stoi(book_id_str);
            year = std::stoi(year_str);
            numberOfCopies = std::stoi(copies_str);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid number format in line: " << line << std::endl;
            continue;  // Skip this line
        }

        // Create a Book object and add it to the vector
        books.emplace_back(book_id, title, author, year, numberOfCopies);
    }

    inFile.close();
    std::cout << "Books loaded from " << filename << " successfully!" << std::endl;
}

// Add this helper function to the Librarian class
int Librarian::countCurrentBorrowings(const std::vector<IssueRecord>& issues, const std::string& borrowerID) {
    int count = 0;
    for (const auto& issue : issues) {
        if (issue.borrowerID == borrowerID && issue.returnDate == 0) {
            count++;
        }
    }
    return count;
}




void Librarian::issueBook(std::vector<IssueRecord>& issues, std::vector<Book>& books, 
                          const std::vector<Student>& students, const std::vector<Faculty>& faculty) {
    std::string bookID, borrowerID, borrowerType;

    std::cout << "Enter Book ID: ";
    std::cin >> bookID;

    auto bookIt = std::find_if(books.begin(), books.end(),
        [&bookID](const Book& b) { return b.book_id == std::stoi(bookID); });

    if (bookIt == books.end() || bookIt->numberOfCopies <= 0) {
        std::cout << "Book not available for issuing.\n";
        return;
    }

    std::cout << "Enter Borrower ID: ";
    std::cin >> borrowerID;
    std::cout << "Enter Borrower Type (Student/Faculty): ";
    std::cin >> borrowerType;

    std::string borrowerName;
    if (borrowerType == "Student") {
        auto studentIt = std::find_if(students.begin(), students.end(),
            [&borrowerID](const Student& s) { return s.id == borrowerID; });
        if (studentIt == students.end()) {
            std::cout << "Student not found.\n";
            return;
        }
        borrowerName = studentIt->name;
    } else if (borrowerType == "Faculty") {
        auto facultyIt = std::find_if(faculty.begin(), faculty.end(),
            [&borrowerID](const Faculty& f) { return f.id == borrowerID; });
        if (facultyIt == faculty.end()) {
            std::cout << "Faculty not found.\n";
            return;
        }
        borrowerName = facultyIt->name;
    } else {
        std::cout << "Invalid borrower type.\n";
        return;
    }

    std::time_t now = std::time(nullptr);
    std::time_t dueDate = now;

    if (borrowerType == "Student") {
        std::time_t dueDate = dueDate + (14 * 24 * 60 * 60); // 14 days from now
    } else if (borrowerType == "Faculty") {
        std::time_t dueDate = dueDate + (4 * 30 * 24 * 60 * 60); // 4 months
    }
    

    IssueRecord newIssue(generateIssueID(), bookID, borrowerID, borrowerType, 
                         now, dueDate, bookIt->title, borrowerName);
    issues.push_back(newIssue);

    bookIt->numberOfCopies--; // Decrement the number of copies
    if (bookIt->numberOfCopies == 0) {
        bookIt->available = false; // Mark unavailable only if no copies remain
    }

    std::cout << "Book issued successfully.\n";
}


void Librarian::returnBook(std::vector<IssueRecord>& issues, std::vector<Book>& books) {
    std::string issueID;
    std::cout << "Enter Issue ID: ";
    std::cin >> issueID;

    auto issueIt = std::find_if(issues.begin(), issues.end(),
        [&issueID](const IssueRecord& i) { return i.issueID == issueID; });

    if (issueIt == issues.end()) {
        std::cout << "Issue record not found.\n";
        return;
    }

    issueIt->returnDate = std::time(nullptr);

    if (issueIt->returnDate > issueIt->dueDate) {
        issueIt->overdueStatus = true;
        int daysOverdue = (issueIt->returnDate - issueIt->dueDate) / (24 * 60 * 60);
        issueIt->fineAmount = daysOverdue * 1.0; // $1 per day overdue
    }

    auto bookIt = std::find_if(books.begin(), books.end(),
        [&](const Book& b) { return b.book_id == std::stoi(issueIt->bookID); });

    if (bookIt != books.end()) {
        bookIt->numberOfCopies++; // Increment the number of copies
        bookIt->available = true; // Set available when copies are returned
    }

    std::cout << "Book returned successfully. ";
    if (issueIt->overdueStatus) {
        std::cout << "Fine amount: $" << issueIt->fineAmount;
    }
    std::cout << std::endl;
}


void Librarian::viewIssuedBooks(const std::vector<IssueRecord>& issues) {
    for (const auto& issue : issues) {
        std::cout << "Issue ID: " << issue.issueID << "\n"
                  << "Book ID: " << issue.bookID << "\n"
                  << "Book Title: " << issue.bookTitle << "\n"
                  << "Borrower ID: " << issue.borrowerID << "\n"
                  << "Borrower Name: " << issue.borrowerName << "\n"
                  << "Borrower Type: " << issue.borrowerType << "\n"
                  << "Borrow Date: " << std::put_time(std::localtime(&issue.borrowDate), "%Y-%m-%d") << "\n"
                  << "Due Date: " << std::put_time(std::localtime(&issue.dueDate), "%Y-%m-%d") << "\n";
        if (issue.returnDate != 0) {
            std::cout << "Return Date: " << std::put_time(std::localtime(&issue.returnDate), "%Y-%m-%d") << "\n";
        }
        std::cout << "Overdue: " << (issue.overdueStatus ? "Yes" : "No") << "\n"
                  << "Fine: $" << issue.fineAmount << "\n\n";
    }
}

std::string Librarian::convertUnixToDate(time_t timestamp) {
    if (timestamp == 0) {
        return "";  // Return empty string for unset dates
    }
    struct tm *timeinfo = std::localtime(&timestamp);
    std::stringstream ss;
    ss << std::put_time(timeinfo, "%d/%m/%Y");
    return ss.str();
}


void Librarian::saveIssuesToCSV(const std::vector<IssueRecord>& issues, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    file << "IssueID,BookID,BorrowerID,BorrowerType,BorrowDate,DueDate,ReturnDate,OverdueStatus,FineAmount,BookTitle,BorrowerName\n";

    for (const auto& issue : issues) {
        // Skip the row if mandatory fields are empty
        if (issue.issueID.empty() || issue.bookID.empty() || issue.borrowerID.empty()) {
            continue; // Skip incomplete records
        }

        // Write valid data to the CSV file
        file << issue.issueID << ","
             << issue.bookID << ","
             << issue.borrowerID << ","
             << issue.borrowerType << ","
             << convertUnixToDate(issue.borrowDate) << ","
             << convertUnixToDate(issue.dueDate) << ","
             << (issue.returnDate != 0 ? convertUnixToDate(issue.returnDate) : "") << ","
             << (issue.overdueStatus ? "1" : "0") << ","
             << std::fixed << std::setprecision(2) << issue.fineAmount << ","
             << (issue.bookTitle.empty() ? "" : issue.bookTitle) << ","
             << (issue.borrowerName.empty() ? "" : issue.borrowerName) << "\n";
    }

    file.close();
    std::cout << "Issues saved to " << filename << " successfully.\n";
}

// Helper function to convert date string to Unix timestamp
time_t convertDateToUnix(const std::string& dateStr) {
    if (dateStr.empty()) {
        return 0;
    }

    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%d/%m/%Y");
    
    if (ss.fail()) {
        std::cerr << "Invalid date format in CSV: " << dateStr << "\n";
        return 0;
    }

    return std::mktime(&tm);
}



void Librarian::loadIssuesFromCSV(std::vector<IssueRecord>& issues, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return;
    }

    issues.clear();
    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string issueID, bookID, borrowerID, borrowerType, borrowDateStr, dueDateStr, returnDateStr, overdueStatusStr, fineAmountStr, bookTitle, borrowerName;

        std::getline(ss, issueID, ',');
        std::getline(ss, bookID, ',');
        std::getline(ss, borrowerID, ',');
        std::getline(ss, borrowerType, ',');
        std::getline(ss, borrowDateStr, ',');
        std::getline(ss, dueDateStr, ',');
        std::getline(ss, returnDateStr, ',');
        std::getline(ss, overdueStatusStr, ',');
        std::getline(ss, fineAmountStr, ',');
        std::getline(ss, bookTitle, ',');
        std::getline(ss, borrowerName);

        // Initialize default values
        time_t borrowDate = 0, dueDate = 0, returnDate = 0;
        bool overdueStatus = false;
        double fineAmount = 0.0;

        // Convert date strings to time_t
        borrowDate = convertDateToUnix(borrowDateStr);
        dueDate = convertDateToUnix(dueDateStr);
        returnDate = convertDateToUnix(returnDateStr);

        // Convert overdue status
        overdueStatus = (overdueStatusStr == "1");

        // Convert fine amount
        try {
            fineAmount = std::stod(fineAmountStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid fine amount in CSV: " << fineAmountStr << "\n";
        }

        // Create and add the issue record
        IssueRecord issue(issueID, bookID, borrowerID, borrowerType, borrowDate, dueDate, bookTitle, borrowerName);
        issue.returnDate = returnDate;
        issue.overdueStatus = overdueStatus;
        issue.fineAmount = fineAmount;

        issues.push_back(issue);
    }

    file.close();
    std::cout << "Issues loaded from " << filename << " successfully.\n";
    std::cout << "Total issues loaded: " << issues.size() << "\n";
}


std::string Librarian::generateIssueID() {
    static int counter = 0;
    return "ISS" + std::to_string(++counter);
}


void Librarian::showLibrarianMenu() {
    std::cout << "\nLibrarian Menu\n"
              << "1. Add Book\n"
              << "2. Delete Book\n"
              << "3. Modify Book\n"
              << "4. View Student Borrowings\n"
              << "5. Display Books\n"
              << "6. Search Book\n"
              << "7. Sort Books\n"
              << "8. Exit\n";
}


// Function to handle librarian menu
void Librarian::librarianMenu(std::vector<Book>& books, std::vector<Student>& students,
                              std::vector<IssueRecord>& issues, std::vector<int> book_ids, const std::string& issueFilename, 
                              const std::string& bookFilename) {
    std::string password;
    std::cout << "Enter librarian password: ";
    std::cin >> password;

   if (Librarian::authenticate(password)) {
    int choice;
    do {
        showLibrarianMenu();  // Call the function to display the menu
        std::cout << "Enter your choice: ";

        // Check if the input is valid
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();  // Clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
            continue;  // Restart the loop
        }

        switch (choice) {
            case 1:
                Librarian::addBook(books, book_ids);
                loadBooksFromCSV(books, bookFilename);
                break;
            case 2:
                Librarian::deleteBook(books);
                loadBooksFromCSV(books, bookFilename);
                break;
            case 3:
                Librarian::modifyBook(books);
                loadBooksFromCSV(books, bookFilename);
                break;
            case 4:
                viewBorrowings(issues);
                break;
            case 5:
                displayBooks(books);
                break;
            case 6: {
                std::string searchTerm;
                std::cout << "Enter title or author to search: ";
                std::cin.ignore();
                std::getline(std::cin, searchTerm);
                linearSearch(books, searchTerm); 
                break;
            }
            case 7:
                quickSort(books, 0, books.size() - 1);
                loadBooksFromCSV(books, bookFilename);
                break;
            case 8:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }

        saveBooksToCSV(books, bookFilename);  // Save the books after modification
        }   while (choice != 8);
        } else {
            std::cout << "Authentication failed. Exiting program.\n";
        }
 
}


