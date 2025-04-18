#include <bits/stdc++.h>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;
/*
    Popular algorithms for RateLimiter:
    1. Token Bucket - tokens are used to serve a request. If there are enough tokens in the bucket, requests are accepted, otherwise rejected.
    Each bucket has a 'capacity' of storing tokens.
    Each second, tokens are filled with 'refillPerSecond' rate.
    Has a method 'allowRequest()' which returns true if request is accepted (> tokens), or rejected (if lesser tokens)
    2. Sliding window
*/
/*
    How to make C++ program wait for x seconds?
    using thread & chrono. 
    this_thread::sleep_for(chrono::seconds(x));
*/
/*
    User
    Properties:
    - userID
    Methods:
    + getUserID()
*/
class User {
    int userID;
    public:
    User(int x) {
        userID = x;
    }
    int getUserID() {
        return userID;
    }
};
/*
    IAlgorithm <- TokenBuket
    Methods:
    + allowRequest(User) - boolean
*/
class IAlgorithm
{
    public:
    virtual bool allowRequest(User *user) = 0;
};
class TokenBucket: public IAlgorithm {
    int capacity;
    double refillPerSecond;
    double tokens;
    time_t lastRefillTime;
    mutex mtx;
    public:
    TokenBucket(int cap, double rps, double token, time_t lastRefill) {
        capacity = cap;
        refillPerSecond = rps;
        tokens = token;
        lastRefillTime = lastRefill;
    }
    bool allowRequest(User *user) override {
        lock_guard<mutex>lock(mtx);
        
        time_t now = time(NULL);
        double secondsPassed = (now-lastRefillTime);
        
        // Refill tokens
        tokens = min((double)capacity, tokens+secondsPassed*refillPerSecond);
        lastRefillTime = now; // Important to reset the lastRefillTime
        
        cout<<"Seconds passed: "<<secondsPassed<<endl;
        cout<<"Available tokens: "<<tokens<<endl;
        cout<<"Last refill time: "<<ctime(&lastRefillTime);
        cout<<"Now: "<<ctime(&now);
        
        
        // Assuming a request takes 1 token
        if(tokens >= 1.0) {
            tokens -= 1.0;
            cout<<"Request served\n"<<endl;
            return true;
        }
        cout<<"Enough tokens not available. Please wait\n"<<endl;
        return false;
    }
};
/*
    RateLimiter
    Properties:
    - unordered_map<User, TokenBucket>userBucket
    
    Methods:
    + registerUser()
    + isAllowed(User)
*/
class RateLimiter {
    unordered_map<User*, IAlgorithm*>userBucket;
    static mutex mtx;
    static RateLimiter* obj;
    RateLimiter() {
    }
    public:
    static RateLimiter* getInstance() {
        if(obj == NULL) {
            mtx.lock();
            if(obj == NULL) {
                obj = new RateLimiter();
            }
            mtx.unlock();
        }
        return obj;
    }
    void registerUser(User *user, int cap, double refillRate) {
        lock_guard<mutex>lock(mtx);
        if(userBucket.find(user)==userBucket.end()) {
            userBucket[user] = new TokenBucket(cap, refillRate, 0.0, time(NULL));
            cout<<"User successfully registered"<<endl;
        }
    }
    bool isAllowed(User *user) {
        lock_guard<mutex>lock(mtx);
        if(userBucket.find(user)!=userBucket.end()) {
            return userBucket[user]->allowRequest(user);
        }
        cout<<"User not registered"<<endl;
        return false;
    }
    // Disable copy constructor and operator overloading
    RateLimiter(const RateLimiter&) = delete;
    void operator=(const RateLimiter&) = delete;
};
RateLimiter* RateLimiter::obj = NULL;
mutex RateLimiter::mtx;
int main() {
    RateLimiter *a = RateLimiter::getInstance();
    
    User *user1 = new User(1);
    User *user2 = new User(2);
    
    a->registerUser(user1, 3, 2.0);
    
    a->isAllowed(user1);
    
    cout<<"Waiting for 2 seconds"<<endl;
    this_thread::sleep_for(chrono::seconds(2));
    a->isAllowed(user1);
    a->isAllowed(user1);
    
    cout<<"Waiting for 3 seconds"<<endl;
    this_thread::sleep_for(chrono::seconds(3));
    a->isAllowed(user2);
}