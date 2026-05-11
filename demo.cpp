#include <iostream>
#include <algorithm>
#include "Transaction.h"
#include "Budget.h"
#include "BudgetStrategy.h"

using namespace std;

int main() {
    // setup
    Income* salary    = new Income(3000.00, {2024, 1, 1},  IncomeType::SALARY);
    Income* freelance = new Income(800.00,  {2024, 2, 1},  IncomeType::FREELANCE);
    Income* bonus     = new Income(500.00,  {2024, 3, 1},  IncomeType::BONUS);
    Expense* rent     = new Expense(900.00, {2024, 1, 5},  ExpenseType::RENT);
    Expense* food     = new Expense(300.00, {2024, 2, 10}, ExpenseType::FOOD);
    Expense* utils    = new Expense(150.00, {2024, 3, 15}, ExpenseType::UTILITIES);
    Budget budget;
    budget.completeTransaction(salary);
    budget.completeTransaction(freelance);
    budget.completeTransaction(bonus);
    budget.completeTransaction(rent);
    budget.completeTransaction(food);
    budget.completeTransaction(utils);

    // polymorphism
    cout << "=== Transactions (polymorphic toString) ===" << endl;
    for (Budget::ForwardIterator it = budget.begin(); it != budget.end(); ++it) {
        // polymorphism
        cout << (*it)->toString() << endl;
    }

    // dynamic_cast
    cout << "\n=== Income type via dynamic_cast ===" << endl;
    for (Budget::ForwardIterator it = budget.begin(); it != budget.end(); ++it) {
        Income* inc = dynamic_cast<Income*>(*it);
        if (inc != nullptr) {
            cout << "Income type id: " << static_cast<int>(inc->getType()) << endl;
        }
    }

    // deep copy
    Budget budgetCopy = budget.copy();
    cout << "\n=== Deep copy total (cents): " << budgetCopy.getTotalInCents() << " ===" << endl;

    // iterator
    cout << "\n=== Iterator + algorithm (find_if first negative) ===" << endl;
    // iterator
    auto found = find_if(budget.begin(), budget.end(), [](Transaction* t) {
        return t->getAmountInCents() < 0;
    });
    if (found != budget.end()) {
        cout << "First expense found: " << (*found)->toString() << endl;
    }

    // sorting
    budget.sort();
    cout << "\n=== Sorted transactions ===" << endl;
    for (Budget::ForwardIterator it = budget.begin(); it != budget.end(); ++it) {
        cout << (*it)->toString() << endl;
    }

    // callback
    cout << "\n=== Callback: filter expenses only ===" << endl;
    // callback
    vector<Transaction*> expenses = budget.filter([](Transaction* t) {
        return t->getAmountInCents() < 0;
    });
    for (Transaction* t : expenses) {
        cout << t->toString() << endl;
    }

    // callback
    cout << "\n=== Callback: filter incomes only ===" << endl;
    vector<Transaction*> incomes = budget.filter([](Transaction* t) {
        return t->getAmountInCents() > 0;
    });
    for (Transaction* t : incomes) {
        cout << t->toString() << endl;
    }

    // strategy
    Fund3Months fund3;
    budget.setStrategy(&fund3);
    cout << "\n=== Strategy (Fund3Months): months needed = "
         << budget.monthsForStrategy() << " ===" << endl;

    Fund6Months fund6;
    budget.setStrategy(&fund6);
    cout << "=== Strategy (Fund6Months): months needed = "
         << budget.monthsForStrategy() << " ===" << endl;

    cout << "\n=== NoStrategyException demo ===" << endl;
    Budget emptyStrategyBudget;
    emptyStrategyBudget.completeTransaction(new Income(1000.00, {2024, 1, 1}, IncomeType::OTHER));
    emptyStrategyBudget.completeTransaction(new Expense(400.00, {2024, 2, 1}, ExpenseType::OTHER));
    try {
        emptyStrategyBudget.monthsForStrategy();
    } catch (const NoStrategyException& e) {
        cout << "Caught: " << e.what() << endl;
    }

    // operator!
    cout << "\n=== Operator! (clear) ===" << endl;
    !budgetCopy;
    cout << "Cleared copy total (cents): " << budgetCopy.getTotalInCents() << endl;

    // calling every transaction
    cout << "\n=== revalidateSum ===" << endl;
    budget.revalidateSum();
    cout << "Revalidated total (cents): " << budget.getTotalInCents() << endl;

    cout << "\n=== Demo complete ===" << endl;
    return 0;
}
