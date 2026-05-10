#include "Transaction.h"

#include <cmath>

// Constructors
Income::Income(long long amountInCents, Date date, IncomeType type) : Transaction(amountInCents, date){
    this->amountInCents = +amountInCents;
    this->type = type;
}
Income::Income(double amount, Date date, IncomeType type)
    : Income(static_cast<long long>(round(amount * 100.0)), date, type) {};


// Getters / Setters
IncomeType Income::getType(){
    return type;
}
void Income::setType(IncomeType type){
    this->type = type;
}

// Methods
string Income::toString(){
    return "Income: " + to_string(amountInCents / 100.0) + " (" + to_string(static_cast<int>(type)) + ")";
}

Transaction* Income::clone() {
    return new Income(*this);
}