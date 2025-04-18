#include <bits/stdc++.h>
#include <mutex>
/*
    Mutexes:
    1. Define mutex: mutex mtx;
    2. lock_guard<mutex>lock(mtx) -> used to lock execution of that piece of block until the scope ends.
    3. Similar to mtx.lock(), mtx.unlock() -> Because we shouldn't use this as exception can be thrown between lock() and unlock()
*/

using namespace std;
/*
    Base62Encoder
    Properties:
    - charset
    Methods:
    + encode()
    + decode()
*/
class Base62Encoder {
    string charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    public:
    string encode(int n) {
        string encoded = "";
        do {
            encoded += charset[n%62];
            n/=62;
        } while(n);
        return encoded;
    }
    int decode(string s) {
        int n=0;
        for(char c:s) {
            if(c>='0' && c<='9')
                n = n*62 + (c-'0');
            else if(c>='a' && c<='z')
                n = n*62 + (c-'a' + 10);
            else if(c>='A' && c<='Z')
                n = n*62 + (c-'A' + 36);
        }
        return n;
    }
};
/*
    IKeyGenerator <- SequentialKeyGenerator (Strategy)
    Methods:
    + generateKey()
*/
class IKeyGenerator {
    public:
    virtual string generateKey() =0;
};
class SequentialKeyGenerator: public IKeyGenerator {
    int counter;
    Base62Encoder *encoder;
    mutex mtx;
    public:
    SequentialKeyGenerator() {
        counter = 1;
        encoder = new Base62Encoder;
    }
    string generateKey() override {
        lock_guard<mutex>lock(mtx);
        return encoder->encode(counter++);
    }
};
/*
    URLRepository
    Properties:
    - shortToLong
    - longToShort
    Methods:
    + existsShortURL()
    + existsLongURL()
    + getLongURL()
    + getShortURL()
    + save(short, long)
*/
class URLRepository {
    unordered_map<string,string>shortToLong;
    unordered_map<string,string>longToShort;
    public:
    bool existsShortURL(string shortURL) {
        if(shortToLong.find(shortURL)!=shortToLong.end())
            return true;
        return false;
    }
    bool existsLongURL(string longURL) {
        if(longToShort.find(longURL)!=longToShort.end())    
            return true;
        return false;
    }
    string getLongURL(string shortURL) {
        return shortToLong[shortURL];
    }
    string getShortURL(string longURL) {
        return longToShort[longURL];
    }
    void save(string shortURL, string longURL) {
        shortToLong[shortURL] = longURL;
        longToShort[longURL] = shortURL;
    }
};
/*
    URLShorteningService (Singleton) -> Disable copy cons, = operator
    Properties:
    - KeyGenerator
    - URLRepository
    Methods:
    + getInstance()
    + shortenURL()
    + retreiveURL()
*/
class URLShorteningService {
    IKeyGenerator *keyGen;
    URLRepository *repo;
    static URLShorteningService *obj;
    static mutex mtx;
    // Private constructor
    URLShorteningService() {
        keyGen = new SequentialKeyGenerator();
        repo = new URLRepository();
    }
    public:
    static URLShorteningService* getInstance() {
        lock_guard<mutex>lock(mtx);
        if(obj == NULL) {
            obj = new URLShorteningService();
        }
        return obj;
    }
    string shortenURL(string longURL) {
        if(repo->existsLongURL(longURL)) {
            return repo->getShortURL(longURL);
        }
        // Generate a new key, save it in repo & return
        string shortKey = keyGen->generateKey();
        shortKey = "www.bit.ly/"+shortKey;
        repo->save(shortKey, longURL);
        return shortKey;
    }
    string retrieveURL(string shortURL) {
        if(repo->existsShortURL(shortURL)) {
            return repo->getLongURL(shortURL);
        }
        return "Not yet converted to shorten URLs";
    }
    // Make copy constructor disabled
    URLShorteningService(const URLShorteningService&) = delete;
    // Make operator overload = diabled
    void operator = (const URLShorteningService&) = delete;
    // We can also make these two private instead of disabling
};
URLShorteningService* URLShorteningService::obj = NULL;
mutex URLShorteningService::mtx;
int main() {
    URLShorteningService *a = URLShorteningService::getInstance();
    // Get shortURLs
    string x1 = a->shortenURL("https://www.programiz.com/cpp-programming/online-compiler/");
    string x2 = a->shortenURL("https://www.programiz.com/java-programming/online-compiler/");
    cout<<x1<<endl;
    cout<<x2<<endl;
    
    // Get longURL for shortKeys
    cout<<(a->retrieveURL(x1))<<endl;
    cout<<(a->retrieveURL(x2))<<endl;
}