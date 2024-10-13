#include "Sorting.h"

int partition(std::vector<Book>& books, int low, int high) {
    std::string pivot = books[high].title;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (books[j].title < pivot) {
            i++;
            std::swap(books[i], books[j]);
        }
    }
    std::swap(books[i + 1], books[high]);
    return i + 1;
}

void quickSort(std::vector<Book>& books, int low, int high) {
    if (low < high) {
        int pi = partition(books, low, high);

        quickSort(books, low, pi - 1);
        quickSort(books, pi + 1, high);
    }
}
