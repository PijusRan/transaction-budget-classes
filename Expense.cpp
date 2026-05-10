#include "Transaction.h"

#include <cmath>

// Constructors
Expense::Expense(long long amountInCents, Date date, ExpenseType type) : Transaction(amountInCents, date){
    this->amountInCents = -amountInCents;
    this->type = type;
}
Expense::Expense(double amount, Date date, ExpenseType type)
    : Expense(static_cast<long long>(round(amount * 100.0)), date, type) {};

// Getters / Setters
ExpenseType Expense::getType(){
    return type;
}
void Expense::setType(ExpenseType type){
    this->type = type;
}

// Methods
string Expense::toString(){
    return "Expense: " + to_string(amountInCents / 100.0) + " (" + to_string(static_cast<int>(type)) + ")";
}

Transaction* Expense::clone() {
    return new Expense(*this);
}