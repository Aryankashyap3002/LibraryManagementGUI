#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include "Book.h"

void quickSort(std::vector<Book>& books, int low, int high);
int partition(std::vector<Book>& books, int low, int high);

#endif
