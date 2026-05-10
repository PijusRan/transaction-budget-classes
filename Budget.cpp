class Budget{
    private:
    vector<Transaction> transactionArray;
    long long totalInCents;

    public:
    Budget(){
        totalInCents = 0;
    };
    
    void completeTransaction(Transaction transaction){
        totalInCents += transaction.getAmountInCents();
        transactionArray.push_back(transaction);
    }

    
};