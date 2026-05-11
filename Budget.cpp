#include <algorithm>
#include "Budget.h"

using namespace std;

struct BudgetImpl {
    vector<Transaction*> transactionArray;
    long long totalInCents;
    BudgetStrategy* strategy;
};

Budget::Budget() : impl(make_unique<BudgetImpl>()) {
    impl->totalInCents = 0;
    impl->strategy = nullptr;
}

Budget::~Budget() = default;

Budget::Budget(const Budget& other) : impl(make_unique<BudgetImpl>()) {
    impl->totalInCents = other.impl->totalInCents;
    impl->strategy = other.impl->strategy;
    for (Transaction* transaction : other.impl->transactionArray) {
        impl->transactionArray.push_back(transaction->clone());
    }
}

long long Budget::getTotalInCents() { return impl->totalInCents; }
double Budget::getTotalInBase() { return impl->totalInCents / 100.0; }
BudgetStrategy* Budget::getStrategy() { return impl->strategy; }

void Budget::setTotalInCents(long long totalInCents) {
    impl->totalInCents = totalInCents;
}
void Budget::setTotalInBase(double totalInBase) {
    impl->totalInCents = static_cast<long long>(totalInBase * 100);
}
void Budget::setStrategy(BudgetStrategy* strategy) {
    impl->strategy = strategy;
}

void Budget::completeTransaction(Transaction* transaction) {
    impl->totalInCents += transaction->getAmountInCents();
    impl->transactionArray.push_back(transaction);
}

int Budget::calcTotalMonths() {
    if (impl->transactionArray.empty()) return 0;

    int earliestYear = impl->transactionArray[0]->getDate().y;
    int earliestMonth = impl->transactionArray[0]->getDate().m;
    int latestYear = impl->transactionArray[0]->getDate().y;
    int latestMonth = impl->transactionArray[0]->getDate().m;

    for (Transaction* transaction : impl->transactionArray) {
        if (transaction->getDate().y < earliestYear ||
            (transaction->getDate().y == earliestYear && transaction->getDate().m < earliestMonth)) {
            earliestYear = transaction->getDate().y;
            earliestMonth = transaction->getDate().m;
        }
        if (transaction->getDate().y > latestYear ||
            (transaction->getDate().y == latestYear && transaction->getDate().m > latestMonth)) {
            latestYear = transaction->getDate().y;
            latestMonth = transaction->getDate().m;
        }
    }

    return (latestYear - earliestYear) * 12 + (latestMonth - earliestMonth) + 1;
}

long long Budget::calcMonthlyExpense() {
    if (impl->transactionArray.empty()) return 0;
    long long totalInCents = 0;

    for (Transaction* transaction : impl->transactionArray) {
        if (transaction->getAmountInCents() < 0) {
            totalInCents += transaction->getAmountInCents();
        }
    }

    return totalInCents / calcTotalMonths();
}

long long Budget::calcMonthlySavings() {
    if (impl->transactionArray.empty()) return 0;
    long long totalInCents = 0;

    for (Transaction* transaction : impl->transactionArray) {
        if (transaction->getAmountInCents() > 0) {
            totalInCents += transaction->getAmountInCents();
        }
    }

    return totalInCents / calcTotalMonths();
}

int Budget::monthsForStrategy() {
    if (impl->strategy == nullptr) throw NoStrategyException();
    return impl->strategy->monthsNeeded(calcMonthlySavings(), calcMonthlyExpense());
}

Budget Budget::copy() {
    Budget newBudget;
    newBudget.impl->totalInCents = this->impl->totalInCents;
    for (Transaction* transaction : this->impl->transactionArray) {
        newBudget.impl->transactionArray.push_back(transaction->clone());
    }
    return newBudget;
}

Budget Budget::operator+=(Transaction* other) {
    impl->transactionArray.push_back(other);
    return *this;
}
Budget Budget::operator-=(Transaction* other) {
    impl->transactionArray.erase(find(impl->transactionArray.begin(), impl->transactionArray.end(), other));
    return *this;
}
Budget Budget::operator*=(Transaction* other) {
    impl->totalInCents += other->getAmountInCents();
    return *this;
}
Budget Budget::operator!() {
    impl->transactionArray.clear();
    impl->totalInCents = 0;
    return *this;
}

bool Budget::operator<(Transaction* other) {
    return impl->totalInCents < other->getAmountInCents();
}
bool Budget::operator>(Transaction* other) {
    return impl->totalInCents > other->getAmountInCents();
}

Budget::ForwardIterator Budget::begin() { return ForwardIterator(impl->transactionArray.begin()); }
Budget::ForwardIterator Budget::end() { return ForwardIterator(impl->transactionArray.end()); }

void Budget::sort() {
    struct Comparator {
        bool operator()(Transaction* a, Transaction* b) {
            return a->getAmountInCents() < b->getAmountInCents();
        }
    };

    std::sort(impl->transactionArray.begin(), impl->transactionArray.end(), Comparator());
}

vector<Transaction*> Budget::filter(function<bool(Transaction*)> callback) {
    vector<Transaction*> filtered;
    for (Transaction* transaction : impl->transactionArray) {
        if (callback(transaction)) {
            filtered.push_back(transaction);
        }
    }
    return filtered;
}

void Budget::revalidateSum() {
    impl->totalInCents = 0;
    for (Transaction* transaction : impl->transactionArray) {
        impl->totalInCents += transaction->getAmountInCents();
    }
}
