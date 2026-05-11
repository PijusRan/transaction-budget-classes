#include <iostream>
#include <cassert>
#include <climits>
#include "Transaction.h"
#include "Budget.h"
#include "BudgetStrategy.h"

using namespace std;

// Helper: simple pass/fail reporter
static int testsPassed = 0;
static int testsFailed = 0;

void check(bool condition, const string& label) {
    if (condition) {
        cout << "[PASS] " << label << endl;
        ++testsPassed;
    } else {
        cout << "[FAIL] " << label << endl;
        ++testsFailed;
    }
}

// ============================================================
// Transaction / Income / Expense tests
// ============================================================

void testIncome() {
    cout << "\n--- Income ---" << endl;

    Income inc(150000LL, {2024, 3, 10}, IncomeType::SALARY);
    check(inc.getAmountInCents() == 150000LL, "Income: amountInCents from long long ctor");
    check(inc.getAmountInBase() == 1500.0,    "Income: amountInBase");
    check(inc.getType() == IncomeType::SALARY, "Income: type SALARY");

    Income incDouble(750.50, {2024, 4, 1}, IncomeType::FREELANCE);
    check(incDouble.getAmountInCents() == 75050LL, "Income: amountInCents from double ctor");
    check(incDouble.getType() == IncomeType::FREELANCE, "Income: type FREELANCE");

    // clone (Prototype pattern)
    Transaction* cloned = inc.clone();
    check(cloned != nullptr, "Income: clone not null");
    check(cloned->getAmountInCents() == inc.getAmountInCents(), "Income: clone amount matches");
    delete cloned;

    // toString
    string s = inc.toString();
    check(!s.empty(), "Income: toString not empty");

    // Invalid date throws
    bool threw = false;
    try { Income bad(100LL, {0, 1, 1}, IncomeType::OTHER); }
    catch (const invalid_argument&) { threw = true; }
    check(threw, "Income: invalid year throws invalid_argument");
}

void testExpense() {
    cout << "\n--- Expense ---" << endl;

    Expense exp(50000LL, {2024, 5, 20}, ExpenseType::FOOD);
    check(exp.getAmountInCents() == -50000LL, "Expense: amount stored as negative");
    check(exp.getType() == ExpenseType::FOOD, "Expense: type FOOD");

    Expense expDouble(200.00, {2024, 6, 1}, ExpenseType::RENT);
    check(expDouble.getAmountInCents() == -20000LL, "Expense: double ctor negative");

    // clone
    Transaction* cloned = exp.clone();
    check(cloned->getAmountInCents() == exp.getAmountInCents(), "Expense: clone amount matches");
    delete cloned;

    // toString
    check(!exp.toString().empty(), "Expense: toString not empty");
}

void testTransactionOperators() {
    cout << "\n--- Transaction operators ---" << endl;

    Income a(10000LL, {2024, 1, 1}, IncomeType::SALARY);
    Income b(10000LL, {2024, 1, 2}, IncomeType::BONUS);
    Income c(20000LL, {2024, 1, 3}, IncomeType::INVESTMENT);

    check(a == b,  "Transaction: == equal amounts");
    check(!(a == c), "Transaction: == different amounts false");
    check(a < c,   "Transaction: < smaller");
    check(c > a,   "Transaction: > larger");
}

// ============================================================
// Budget container tests
// ============================================================

void testBudgetBasic() {
    cout << "\n--- Budget basic ---" << endl;

    Budget b;
    check(b.getTotalInCents() == 0, "Budget: initial total is 0");
    check(b.getStrategy() == nullptr, "Budget: initial strategy is null");

    Income* inc = new Income(200000LL, {2024, 1, 1}, IncomeType::SALARY);
    b.completeTransaction(inc);
    check(b.getTotalInCents() == 200000LL, "Budget: total after income");

    Expense* exp = new Expense(50000LL, {2024, 2, 1}, ExpenseType::RENT);
    b.completeTransaction(exp);
    check(b.getTotalInCents() == 150000LL, "Budget: total after expense");
}

void testBudgetCalcMethods() {
    cout << "\n--- Budget calc methods ---" << endl;

    Budget b;
    // 3 months of data
    b.completeTransaction(new Income(300000LL,  {2024, 1, 1}, IncomeType::SALARY));
    b.completeTransaction(new Income(300000LL,  {2024, 2, 1}, IncomeType::SALARY));
    b.completeTransaction(new Income(300000LL,  {2024, 3, 1}, IncomeType::SALARY));
    b.completeTransaction(new Expense(90000LL,  {2024, 1, 5}, ExpenseType::RENT));
    b.completeTransaction(new Expense(90000LL,  {2024, 2, 5}, ExpenseType::RENT));
    b.completeTransaction(new Expense(90000LL,  {2024, 3, 5}, ExpenseType::RENT));

    check(b.calcTotalMonths() == 3,          "Budget: calcTotalMonths = 3");
    check(b.calcMonthlySavings() == 300000LL, "Budget: calcMonthlySavings");
    check(b.calcMonthlyExpense() == -90000LL, "Budget: calcMonthlyExpense (negative)");
}

void testBudgetDeepCopy() {
    cout << "\n--- Budget deep copy ---" << endl;

    Budget original;
    original.completeTransaction(new Income(100000LL, {2024, 1, 1}, IncomeType::GIFT));
    original.completeTransaction(new Expense(30000LL,  {2024, 1, 10}, ExpenseType::ENTERTAINMENT));

    Budget copy = original.copy();
    check(copy.getTotalInCents() == original.getTotalInCents(), "Budget: copy total matches");

    // Modifying copy must not affect original
    !copy; // clear copy
    check(original.getTotalInCents() != 0, "Budget: original unaffected after clearing copy");
}

void testBudgetStrategy() {
    cout << "\n--- Budget strategy ---" << endl;

    Budget b;
    // 1 month, savings 500, expense 200
    b.completeTransaction(new Income(50000LL,  {2024, 1, 1}, IncomeType::SALARY));
    b.completeTransaction(new Expense(20000LL, {2024, 1, 5}, ExpenseType::UTILITIES));

    // No strategy: should throw
    bool threw = false;
    try { b.monthsForStrategy(); }
    catch (const NoStrategyException&) { threw = true; }
    check(threw, "Budget: monthsForStrategy throws with no strategy");

    Fund3Months fund3;
    b.setStrategy(&fund3);
    check(b.getStrategy() == &fund3, "Budget: strategy pointer set");
    int months3 = b.monthsForStrategy();
    check(months3 >= 0, "Budget: Fund3Months returns non-negative months");

    Fund6Months fund6;
    b.setStrategy(&fund6);
    int months6 = b.monthsForStrategy();
    check(months6 >= months3, "Budget: Fund6Months >= Fund3Months months");
}

void testBudgetIteratorAndFilter() {
    cout << "\n--- Budget iterator & filter ---" << endl;

    Budget b;
    b.completeTransaction(new Income(100000LL,  {2024, 1, 1}, IncomeType::SALARY));
    b.completeTransaction(new Expense(40000LL,  {2024, 1, 5}, ExpenseType::FOOD));
    b.completeTransaction(new Income(50000LL,   {2024, 2, 1}, IncomeType::FREELANCE));

    int count = 0;
    for (Budget::ForwardIterator it = b.begin(); it != b.end(); ++it) {
        ++count;
    }
    check(count == 3, "Budget: iterator visits all 3 transactions");

    // filter callback: incomes only
    vector<Transaction*> incomes = b.filter([](Transaction* t) {
        return t->getAmountInCents() > 0;
    });
    check(incomes.size() == 2, "Budget: filter incomes count = 2");

    // filter callback: expenses only
    vector<Transaction*> exps = b.filter([](Transaction* t) {
        return t->getAmountInCents() < 0;
    });
    check(exps.size() == 1, "Budget: filter expenses count = 1");
}

void testBudgetOperators() {
    cout << "\n--- Budget operators ---" << endl;

    Budget b;
    Income* inc = new Income(200000LL, {2024, 1, 1}, IncomeType::SALARY);
    b += inc;

    // operator> and <
    Income bigInc(999999LL, {2024, 2, 1}, IncomeType::INVESTMENT);
    Income smallInc(100LL,  {2024, 3, 1}, IncomeType::OTHER);

    check(b > &smallInc, "Budget: operator> total > small transaction");
    check(b < &bigInc,   "Budget: operator< total < big transaction");

    // operator*= applies amount to total
    Income* bonus = new Income(10000LL, {2024, 4, 1}, IncomeType::BONUS);
    long long before = b.getTotalInCents();
    b *= bonus;
    check(b.getTotalInCents() == before + 10000LL, "Budget: operator*= adds amount");

    // operator-= removes transaction
    b -= inc;
    b.revalidateSum();
    // After removing inc the only contribution is bonus (10000) as *= doesn't push to array
    check(b.getTotalInCents() == 10000LL, "Budget: operator-= and revalidateSum");

    // operator! clears
    !b;
    check(b.getTotalInCents() == 0, "Budget: operator! clears total");
}

void testBudgetSort() {
    cout << "\n--- Budget sort ---" << endl;

    Budget b;
    b.completeTransaction(new Income(500000LL,  {2024, 1, 1}, IncomeType::SALARY));
    b.completeTransaction(new Expense(100000LL, {2024, 1, 2}, ExpenseType::RENT));
    b.completeTransaction(new Income(200000LL,  {2024, 1, 3}, IncomeType::FREELANCE));

    b.sort();

    // After sort amounts should be non-decreasing
    long long prev = LLONG_MIN;
    bool sorted = true;
    for (Budget::ForwardIterator it = b.begin(); it != b.end(); ++it) {
        if ((*it)->getAmountInCents() < prev) { sorted = false; break; }
        prev = (*it)->getAmountInCents();
    }
    check(sorted, "Budget: sort produces non-decreasing order");
}

// ============================================================
// Loan class tests (third class – not yet implemented)
// Tests show the intended contract so the implementor knows
// what a correct Loan implementation must satisfy.
// ============================================================

void testLoan() {
    cout << "\n--- Loan (third class – contract tests) ---" << endl;

    // --- Construction ---
    // A Loan of 12 000.00 over 12 months at 5 % interest,
    // starting on 2024-01-01.
    //
    // Loan loan(1200000LL, {2024, 1, 1}, LoanType::NOT_IMPLEMENTED, 12, 0.05);
    //
    // The constructor must call the Transaction base constructor (which
    // validates the date) and store periodInMonths and interestRate.

    // --- getAmountInCents ---
    // The raw principal stored in amountInCents must be positive
    // because completeTransaction() in Budget will split it into
    // Expense objects whose amounts are negated there.
    //
    // check(loan.getAmountInCents() == 1200000LL,
    //       "Loan: amountInCents equals principal");

    // --- toString ---
    // toString() must return a non-empty string describing the loan,
    // e.g. "Loan: 12000.00 over 12 months at 5.00%"
    //
    // check(!loan.toString().empty(), "Loan: toString not empty");

    // --- clone (Prototype pattern) ---
    // clone() must return a heap-allocated copy of the correct dynamic
    // type (Loan*), with matching amount, period, and interest rate.
    //
    // Transaction* cloned = loan.clone();
    // check(cloned != nullptr, "Loan: clone not null");
    // Loan* clonedLoan = dynamic_cast<Loan*>(cloned);
    // check(clonedLoan != nullptr, "Loan: clone dynamic type is Loan");
    // check(clonedLoan->getAmountInCents() == loan.getAmountInCents(),
    //       "Loan: clone amount matches");
    // delete cloned;

    // --- Budget integration ---
    // When completeTransaction() receives a Loan, it must split the
    // loan into `periodInMonths` separate Expense objects of type
    // ExpenseType::LOAN, each with amount = principal / periodInMonths,
    // spread across consecutive months starting from the loan date.
    //
    // Budget b;
    // Income* salary = new Income(500000LL, {2024, 1, 1}, IncomeType::SALARY);
    // b.completeTransaction(salary);
    //
    // Loan* loan2 = new Loan(120000LL, {2024, 1, 1},
    //                        LoanType::NOT_IMPLEMENTED, 3, 0.0);
    // b.completeTransaction(loan2);   // must add 3 Expense(40000) entries
    //
    // int expenseCount = 0;
    // for (auto it = b.begin(); it != b.end(); ++it) {
    //     if ((*it)->getAmountInCents() < 0) ++expenseCount;
    // }
    // check(expenseCount == 3, "Loan: completeTransaction creates 3 expense instalments");
    //
    // long long expectedTotal = 500000LL - 120000LL;   // salary - full principal
    // check(b.getTotalInCents() == expectedTotal,
    //       "Loan: budget total after loan matches salary minus principal");

    // All Loan tests are commented out because the class is not yet
    // implemented.  Once Loan.cpp is written and compiled, remove the
    // comment markers and run the tests to verify the implementation.
    cout << "(Loan tests intentionally commented out – class not yet implemented)" << endl;
    cout << "(Uncomment tests in testLoan() once Loan.cpp is provided)" << endl;
}

// ============================================================
// Entry point
// ============================================================

int main() {
    cout << "========== Budget Module Tests ==========" << endl;

    testIncome();
    testExpense();
    testTransactionOperators();
    testBudgetBasic();
    testBudgetCalcMethods();
    testBudgetDeepCopy();
    testBudgetStrategy();
    testBudgetIteratorAndFilter();
    testBudgetOperators();
    testBudgetSort();
    testLoan();

    cout << "\n========== Results ==========" << endl;
    cout << "Passed: " << testsPassed << endl;
    cout << "Failed: " << testsFailed << endl;

    return testsFailed == 0 ? 0 : 1;
}
