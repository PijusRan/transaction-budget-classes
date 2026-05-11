#include "BudgetStrategy.h"

int Fund3Months::monthsNeeded(long long monthlySavings, long long monthlyExpense) {
    return (monthlyExpense * 3) / monthlySavings;
}

int Fund6Months::monthsNeeded(long long monthlySavings, long long monthlyExpense) {
    return (monthlyExpense * 6) / monthlySavings;
}
