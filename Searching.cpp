#include "Searching.h"
#include <iostream>

void linearSearch(const std::vector<Book>& books, const std::string& searchTerm) {
    bool found = false;
    for (const auto& book : books) {
        if (book.title == searchTerm || book.author == searchTerm) {
            book.print();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No book found with that title or author.\n";
    }
}

int binarySearch(const std::vector<Book>& books, const std::string& searchTerm) {
    int left = 0, right = books.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (books[mid].title == searchTerm || books[mid].author == searchTerm) {
            return mid;  // Book found
        } else if (books[mid].title < searchTerm) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;  // Book not found
}
