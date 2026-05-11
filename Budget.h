#pragma once
#include <vector>
#include <functional>
#include <memory>
#include "Transaction.h"
#include "BudgetStrategy.h"

using namespace std;

class BudgetImpl;

class Budget {
    private:
    unique_ptr<BudgetImpl> impl;

    public:
    Budget();
    ~Budget();
    Budget(const Budget& other);
    Budget(Budget&&) = default;
    Budget& operator=(Budget&&) = default;

    long long getTotalInCents();
    double getTotalInBase();
    BudgetStrategy* getStrategy();

    void setTotalInCents(long long totalInCents);
    void setTotalInBase(double totalInBase);
    void setStrategy(BudgetStrategy* strategy);

    void completeTransaction(Transaction* transaction);
    int calcTotalMonths();
    long long calcMonthlyExpense();
    long long calcMonthlySavings();
    int monthsForStrategy();
    Budget copy();

    Budget operator+=(Transaction* other);
    Budget operator-=(Transaction* other);
    Budget operator*=(Transaction* other);
    Budget operator!();
    bool operator<(Transaction* other);
    bool operator>(Transaction* other);

    class ForwardIterator {
        private:
        vector<Transaction*>::iterator it;

        public:
        using iterator_category = forward_iterator_tag;
        using value_type = Transaction*;
        using difference_type = ptrdiff_t;
        using pointer = Transaction**;
        using reference = Transaction*&;

        ForwardIterator(vector<Transaction*>::iterator iter) : it(iter) {}

        Transaction*& operator*() {
            return *it;
        }
        Transaction** operator->() {
            return &(*it);
        }
        ForwardIterator& operator++() {
            ++it;
            return *this;
        }
        ForwardIterator operator++(int) {
            ForwardIterator tmp = *this;
            ++it;
            return tmp;
        }
        bool operator==(const ForwardIterator& other) const {
            return it == other.it;
        }
        bool operator!=(const ForwardIterator& other) const {
            return it != other.it;
        }
    };

    ForwardIterator begin();
    ForwardIterator end();

    void sort();
    vector<Transaction*> filter(function<bool(Transaction*)> callback);
    void revalidateSum();
};
