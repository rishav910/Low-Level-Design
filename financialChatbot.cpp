#include <bits/stdc++.h>
using namespace std;
/*
    Learnings:
    When you feel multiple classes are having same methods
    You might be wrong, instead get the methods where they are originating from, not from some other class which will call them
*/
// Forward class declaration since user needs Account class
class Account;
/*
    User
    Properties:
    - userID
    - name
    - Account*
    Methods:
    + getAccount()
    + getName()
*/
class User {
    string name;
    Account* account;
    int userID;
    public:
    User(string pName, Account *a, int id):name(pName),account(a),userID(id){}
    Account* getAccount() {
        return account;
    }
    string getName() {
        return name;
    }
};
/*
    Transaction
    Properties:
    - txnID
    - txnTime
    - amount (double)
    - category (string)
    Methods:
    + showTransaction()
*/
class Transaction {
    int txnID;
    time_t txnTime;
    double amount;
    string category;
    public:
    Transaction(int id, time_t timeT, double amt, string cat) {
        txnID = id;
        txnTime = timeT;
        amount = amt;
        category = cat;
    }
    void showTransaction() {
        cout<<"Transaction of "<<amount<<" done at "<<txnTime<<endl;
    }    
};
/*
    Account
    Properties:
    - accountID
    - balance (double)
    Methods:
    + getBalance()
    + transferMoney()
    + showTransactions()
    + monthlyTotalExpense()
    + categoryWiseSplit()
*/
class Account {
    int accountID;
    double balance;
    vector<Transaction*>transactions;
    unordered_map<string, int>category;
    public:
    static int txnID;
    Account(int id, double bal): accountID(id), balance(bal){}
    
    double getBalance() {
        return balance;
    }
    bool addMoney(double money) {
        cout<<"Money added successfully"<<endl;
        balance += money;
        return true;
    }
    bool transferMoney(User *user, double amt) {
        if(balance < amt) {
            cout<<"Insufficient balance"<<endl;
            return false;
        }
        cout<<"Transferred "<<amt<<" to "<<user->getName()<<endl;
        balance -= amt;
        user->getAccount()->addMoney(amt);
        Transaction *tx = new Transaction(txnID, time(NULL),amt,"Money transfer");
        transactions.push_back(tx);
        category["Money transfer"]++;
        return true;
    }
    void transactionHistory(int latestCnt=5) {
        int n = transactions.size();
        if(n-1-latestCnt < 0) {
            for(int i=n-1; i>=0; i--) {
                transactions[i]->showTransaction();
            }
            return;
        }
        for(int i=n-1; i>=n-1-latestCnt; i--) {
            transactions[i]->showTransaction();
        }
    }
    void showMonthlyExpense() {
        cout<<"Showing transaction history for last month";
        // Do some modifications with txnTime;
    }
    void categoryWiseSplit() {
        cout<<"Category count:"<<endl;
        for(auto x: category) {
            cout<<x.first<<" : "<<x.second;
        }
    }
};
int Account::txnID = 1;
/*
    FinancialChatbot
    Methods:
    + command() - based on command, we call account's methods
*/
class FinancialChatbot {
    public:
    void command(User*user, string com) {
        if(com.compare("balance")==0) {
            cout<<user->getName()<<"'s Balance: "<<user->getAccount()->getBalance()<<endl;
        } else if(com.compare("transactions")) {
            cout<<"Retreiving transaction history for "<<user->getName()<<":"<<endl;
            user->getAccount()->transactionHistory();
        } else if(com.compare("monthly expense")) {
            // user->getAccount()->showMonthlyExpense();
        } else if(com.compare("category split")) {
            user->getAccount()->categoryWiseSplit();
        } else {
            cout<<"Wrong command entered"<<endl;
        }
    }
    void transferMoney(User *user1, User *user2, double amt) {
        user1->getAccount()->transferMoney(user2, amt);
    } 
};
int main() {
    int accID = 1, userID = 1;
    User *user1 = new User("Rishav", new Account(accID++, 10000.00), userID++);
    User *user2 = new User("Barun", new Account(accID++, 70500.00), userID++);
    
    FinancialChatbot *bot = new FinancialChatbot;
    // Balance query - user1
    bot->command(user1, "balance");
    // Transfer money from user1 -> user2
    bot->transferMoney(user1, user2, 2000);
    bot->transferMoney(user1, user2, 12000);
    bot->transferMoney(user1, user2, 5000);
    // Balance query - user2
    bot->command(user2, "balance");
    bot->command(user1, "balance");
    
    // Transaction history - user1
    bot->command(user1, "transactions");
    
    // Category split - user1
    bot->command(user1, "category split");
}