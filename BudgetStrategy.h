#pragma once
#include <stdexcept>

using namespace std;

class NoStrategyException : public logic_error {
    public:
    NoStrategyException() : logic_error("No strategy set.") {}
};

class BudgetStrategy {
    public:
    virtual int monthsNeeded(long long monthlySavings, long long monthlyExpense) = 0;
    virtual ~BudgetStrategy() = default;
};

class Fund3Months : public BudgetStrategy {
    public:
    int monthsNeeded(long long monthlySavings, long long monthlyExpense) override;
};

class Fund6Months : public BudgetStrategy {
    public:
    int monthsNeeded(long long monthlySavings, long long monthlyExpense) override;
};
