// IssueRecord.h
#ifndef ISSUE_RECORD_H
#define ISSUE_RECORD_H

#include <string>
#include <ctime>

class IssueRecord {
public:
    std::string issueID;
    std::string bookID;
    std::string borrowerID;
    std::string borrowerType;
    std::time_t borrowDate;
    std::time_t dueDate;
    std::time_t returnDate;
    bool overdueStatus;
    double fineAmount;
    std::string bookTitle;
    std::string borrowerName;

    IssueRecord(std::string issueID, std::string bookID, std::string borrowerID, 
                std::string borrowerType, std::time_t borrowDate, std::time_t dueDate,
                std::string bookTitle, std::string borrowerName)
        : issueID(std::move(issueID)), bookID(std::move(bookID)), borrowerID(std::move(borrowerID)),
          borrowerType(std::move(borrowerType)), borrowDate(borrowDate), dueDate(dueDate),
          returnDate(0), overdueStatus(false), fineAmount(0.0),
          bookTitle(std::move(bookTitle)), borrowerName(std::move(borrowerName)) {}

    // Add methods to update return date, check overdue status, calculate fine, etc.
};

#endif // ISSUE_RECORD_H