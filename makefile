Budget.o: Budget.cpp Budget.h Transaction.o BudgetStrategy.o
	g++ -c Budget.cpp Budget.h Transaction.o BudgetStrategy.o -o Budget.o

BudgetStrategy.o: BudgetStrategy.cpp BudgetStrategy.h
	g++ -c BudgetStrategy.cpp -o BudgetStrategy.o

Transaction.o: Transaction.cpp Transaction.h
	g++ -c Transaction.cpp -o Transaction.o

Income.o: Income.cpp Transaction.h
	g++ -c Income.cpp -o Income.o

Expense.o: Expense.cpp Transaction.h
	g++ -c Expense.cpp -o Expense.o

demo: demo.cpp Budget.o Transaction.o Income.o Expense.o BudgetStrategy.o
	g++ demo.cpp Budget.o Transaction.o Income.o Expense.o BudgetStrategy.o -o demo

test: test.cpp Budget.o Transaction.o Income.o Expense.o BudgetStrategy.o
	g++ test.cpp Budget.o Transaction.o Income.o Expense.o BudgetStrategy.o -o test

run_demo: demo
	./demo

run_test: test
	./test

clean:
	rm -f *.o demo test

rebuild:
	make clean
	make Budget.o
	make demo
	make test