#include "Transaction.h"

#include <iostream>
#include <cmath>



// Constructors
Transaction::Transaction(long long amountInCents, Date date){
    // Durna logika vardan paprastumo
    if(date.y <= 0 || date.m <= 0 || date.m > 12 || date.d <= 0 || date.d > 31){
        throw invalid_argument("Invalid date.");
    }
    if(amountInCents <= 0){
        throw invalid_argument("Transaction cannot be a negative value.");
    }

    this->date = date;
}
Transaction::Transaction(double amount, Date date)
    : Transaction(static_cast<long long>(round(amount * 100.0)), date) {};
Transaction::Transaction(){}

// Getters / Setters
long long Transaction::getAmountInCents(){
    return this->amountInCents;
}
double Transaction::getAmountInBase(){
    return static_cast<double>(this->amountInCents/100.00);
}
Date Transaction::getDate(){
    return date;
}

void Transaction::setAmountInCents(long long amountInCents){
    this->amountInCents = amountInCents;
}
void Transaction::setAmountInBase(double amountInBase){
    this->amountInCents = static_cast<long long>(amountInBase*100.00);
}
void Transaction::setDate(Date date){
    this->date = date;
}

// Methods
Transaction* Transaction::clone() {
    throw NotImplementedException();
}

// Operators
bool Transaction::operator==(const Transaction& other) const {
    return (amountInCents == other.amountInCents);
}
bool Transaction::operator<(const Transaction& other) const {
    return (amountInCents < other.amountInCents);
}
bool Transaction::operator>(const Transaction& other) const {
    return (amountInCents > other.amountInCents);
}