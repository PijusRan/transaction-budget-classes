#pragma once
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

struct Date{
    unsigned int y, m, d;
};

class Transaction{
// Fields
    protected:
    long long amountInCents;
    Date date;

// Constructors
    public:
    Transaction(long long amountInCents, Date date);
    Transaction(double amount, Date date);
    Transaction();

// Getters / Setters
    long long getAmountInCents();
    double getAmountInBase();
    Date getDate();

    void setAmountInCents(long long amountInCents);
    void setAmountInBase(double amountInBase);
    void setDate(Date date);

// Methods
    Transaction* clone();

    virtual string toString();

// Operators
    bool operator==(const Transaction& other) const;
    bool operator<(const Transaction& other) const;
    bool operator>(const Transaction& other) const;
};

//  === INCOME ===

enum class IncomeType {
    SALARY,
    FREELANCE,
    INVESTMENT,
    BONUS,
    GIFT,
    OTHER
};

class Income : Transaction{
// Fields
    private:
    IncomeType type;

// Constructors
    public:
    Income(long long amountInCents, Date date, IncomeType type);
    Income(double amount, Date date, IncomeType type);

// Getters / Setters
    IncomeType getType();
    void setType(IncomeType type);

// Methods
    string toString() override;
};

// === EXPENSE ===

enum class ExpenseType {
    FOOD,
    RENT,
    UTILITIES,
    TRANSPORTATION,
    ENTERTAINMENT,
    HEALTHCARE,
    INSURANCE,
    EDUCATION,
    OTHER
};

class Expense : Transaction{
// Fields
    private:
    ExpenseType type;

// Constructors
    public:
    Expense(long long amountInCents, Date date, ExpenseType type);
    Expense(double amount, Date date, ExpenseType type);

// Getters / Setters
    ExpenseType getType();
    void setType(ExpenseType type);

// Methods
    string toString() override;
};