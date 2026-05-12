#pragma once
#include <string>
#include <stdexcept>
using namespace std;

class NotImplementedException : public std::logic_error {
    public:
    NotImplementedException() : std::logic_error("Method not implemented.") {}
};

struct Date{
    unsigned int y, m, d;
};

class Transaction{
// Fields
    protected:
    long long amountInCents;
    Date date;

// Constructors / Destructors
    public:
    Transaction(long long amountInCents, Date date);
    Transaction(double amount, Date date);
    Transaction() = default;
    virtual ~Transaction() = default;

// Getters / Setters
    long long getAmountInCents();
    double getAmountInBase();
    Date getDate();

    virtual void setAmountInCents(long long amountInCents);
    virtual void setAmountInBase(double amountInBase);
    virtual void setDate(Date date);

// Methods
    virtual Transaction* clone() = 0;
    virtual string toString() = 0;

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

class Income : public Transaction{
// Fields
    private:
    IncomeType type;

// Constructors / Destructors
    public:
    Income(long long amountInCents, Date date, IncomeType type);
    Income(double amount, Date date, IncomeType type);
    ~Income() override = default;

// Getters / Setters
    IncomeType getType();
    void setType(IncomeType type);

// Methods
    Transaction* clone() override;
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
    LOAN,
    OTHER
};

class Expense : public Transaction{
// Fields
    private:
    ExpenseType type;

// Constructors / Destructors
    public:
    Expense(long long amountInCents, Date date, ExpenseType type);
    Expense(double amount, Date date, ExpenseType type);
    ~Expense() override = default;

// Getters / Setters
    ExpenseType getType();
    void setType(ExpenseType type);

// Methods
    Transaction* clone() override;
    string toString() override;
};

// === LOAN (not implemented) ===

enum class LoanType {
    NOT_IMPLEMENTED
};

class Loan : public Transaction{
// Fields
    private:
    int periodInMonths;
    double interestRate;
    LoanType type;

// Constructors / Destructors
    public:
    Loan(long long amountInCents, Date date, LoanType, int period, double interestRate);
    Loan(double amount, Date date, LoanType type, int period, double interestRate);
    ~Loan() override = default;

// Getters / Setters
    LoanType getType();
    void setType(LoanType);

// Methods
    Transaction* clone() override;
    string toString() override;
};