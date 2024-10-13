#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include "Book.h"
#include "Student.h"
#include "Faculty.h"
#include "Librarian.h"
#include "IssueRecord.h"

int main() {
    std::vector<Book> books;
    std::vector<int> book_ids;
    std::vector<IssueRecord> issues;
    std::vector<Student> students;
    std::vector<Faculty> faculty;
    std::string bookFilename = "books.csv";
    std::string studentFilename = "students.csv";
    std::string facultyFilename = "faculty.csv";
    std::string issueFilename = "issue.csv";

    // Load books, students, and faculty from CSV files
    // Librarian::loadIssueFromCSV(books, students, faculty, issueFilename);
    Librarian::loadIssuesFromCSV(issues, issueFilename);
    Librarian::loadBooksFromCSV(books, bookFilename);
    Student::loadStudentsFromCSV(students, studentFilename, books);
    Faculty::loadFacultyFromCSV(faculty, facultyFilename, books);

    Librarian::loadBooksFromCSV(books, bookFilename);
    Librarian::loadIssuesFromCSV(issues, issueFilename);

    for (const auto& book : books) {
        book_ids.push_back(book.book_id);
    }
for (const auto& book : books) {
        book_ids.push_back(book.book_id);
    }
    int userType;
    do {
        std::cout << "\nWelcome to the Library Management System\n";
        std::cout << "1. Librarian\n2. Student\n3. Faculty\n4. Exit\n";
        std::cout << "Enter your user type (1-4): ";
        
        if (!(std::cin >> userType)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (userType) {
            case 1:
                Librarian::librarianMenu(books, students, issues, book_ids, issueFilename, bookFilename);
                break;
            case 2:
                Student::studentMenu(books, students, issues, issueFilename, studentFilename);
                break;
            case 3:
                Faculty::facultyMenu(faculty, books, facultyFilename);
                break;
            case 4:
                std::cout << "Thank you for using the Library Management System. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }

        Librarian::saveBooksToCSV(books, bookFilename);
        Librarian::saveIssuesToCSV(issues, issueFilename);
    } while (userType != 4);

    // Save all data before exiting
    // Librarian::saveBooksToCSV(books, bookFilename);
    Librarian::saveIssuesToCSV(issues, issueFilename);
    Student::saveStudentsToCSV(students, studentFilename);
    Faculty::saveFacultyToCSV(faculty, facultyFilename);

    return 0;
}

// #include <QApplication>
// #include <QMainWindow>
// #include <QPushButton>
// #include <QVBoxLayout>

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//     QMainWindow window;
    
//     QWidget *centralWidget = new QWidget(&window);
//     QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
//     QPushButton *librarianButton = new QPushButton("Librarian", centralWidget);
//     QPushButton *studentButton = new QPushButton("Student", centralWidget);
//     QPushButton *facultyButton = new QPushButton("Faculty", centralWidget);
    
//     layout->addWidget(librarianButton);
//     layout->addWidget(studentButton);
//     layout->addWidget(facultyButton);
    
//     window.setCentralWidget(centralWidget);
//     window.setWindowTitle("Library Management System");
//     window.show();
    
//     return app.exec();
// }



// #include <catch2/catch.hpp>
// #include "Book.h"

// TEST_CASE("Book creation and accessors", "[book]") {
//     Book book("The Great Gatsby", "F. Scott Fitzgerald", 1925);
    
//     REQUIRE(book.getTitle() == "The Great Gatsby");
//     REQUIRE(book.getAuthor() == "F. Scott Fitzgerald");
//     REQUIRE(book.getYear() == 1925);
//     REQUIRE(book.isAvailable() == true);
// }