#include<bits/stdc++.h>
using namespace std;
/*
    User
    Properties:
    + username
    + password
    + loggedIn
    Methods:
    + login()
    + logout()
*/
class User {
    public:
    string username;
    string password;
    bool loggedIn;
    
    User(string user, string pass):username(user),password(pass),loggedIn(false){}
    
    void login() {
        loggedIn = true;
        cout<<"Logged in successfully"<<endl;
    }
    void logout() {
        loggedIn = false;
        cout<<"Logged out sucessfully"<<endl;
    }
};
/*
    AuthenticationService
    - map users (username -> User)
    + signup()
    + login()
    + logout()
*/
class AuthenticationService {
    unordered_map<string, User*>users;
    public:
    bool signup(string username, string password) {
        if(users.find(username) == users.end()) {
            users[username] = new User(username, password);
            cout<<"Signing up successful"<<endl;
            return true;
        }
        cout<<"User already exists"<<endl;
        return false;
    }
    User* login(string user, string pass) {
        if(users.find(user)!=users.end()) {
            if(users[user]->password == pass) {
                users[user]->login();
                return users[user];
            } else {
                cout<<"Invalid credentials"<<endl;
                return NULL;
            }
        }
        cout<<"User does not exist"<<endl;
        return NULL;
    }
    void logout(User *user) {
        if(user && user->loggedIn) {
            user->logout();
        }
    }
};
/*
    Product
    Properties:
    - totalCountOfRaings
    + name
    + price
    + avgRating
    + vector<string> reviews
*/
class Product {
    int totalCountOfRatings;
    public:
    string name;
    int price;
    double avgRating;
    vector<string>reviews;
    
    Product(string a, int b) {
        name = a;
        price = b;
        avgRating = 0.0;
        totalCountOfRatings = 0;
    }
    
    void addReview(string review, int rating) {
        reviews.push_back(review);
        addRating(rating);
    }
    
    void addRating(int rating) {
        avgRating = (double)(avgRating + rating)/(totalCountOfRatings+1);
        totalCountOfRatings++;
    }
};
/* 
    Payment <- CardPayment, UPIPayment
    Properties:  
    + getType()
    + processPayment()
*/
class Payment {
    public:
    virtual string getType() = 0;
    virtual void processPayment(Product *p, int quantity) = 0;
};
class CardPayment: public Payment {
    public:
    string getType() override {
        return "Card";
    }
    void processPayment(Product *p, int quantity) override {
        double price = p->price * quantity;
        cout<<p->name<<": Processing payment of Rs."<<price<<" via "<<getType()<<endl;
    }
};
class UPIPayment: public Payment {
    public:
    string getType() override {
        return "UPI";
    }
    void processPayment(Product *p, int quantity) override {
        double price = p->price * quantity;
        cout<<p->name<<": Processing payment of Rs."<<price<<" via "<<getType()<<endl;
    }
};
/*
    Cart
    Properties:
    - unordered_map<Product*, int> items
    - totalAmount
    Methods:
    + addToCart()
    + removeFromCart()
    + decreaseProductCount()
    + increaseProductCount()
    + getItems()
*/
class Cart {
    unordered_map<Product*,int>items;
    int totalAmount;
    public:
    Cart() {
        totalAmount = 0;
    }
    void addToCart(Product *p) {
        items[p]++;
        totalAmount += p->price;
    }
    bool removeFromCart(Product *p) {
        if(items.find(p) != items.end()) {
            totalAmount -= (p->price * items[p]);
            items.erase(items.find(p));
            return true;
        }
        return false;
    }
    void decreaseProductCount(Product *p) {
        items[p]--;
        totalAmount -= p->price;
        if(items[p] == 0) {
            items.erase(items.find(p));
        }
    }
    void increaseProductCount(Product *p) {
        items[p]++;
        totalAmount += p->price;
    }
    unordered_map<Product*,int> getItems() {
        return items;
    }
};
/*
    Order
    Properties:
    - orderID
    - vector<Item> items
    - totalAmount
    - orderTime
    Methods:
    + track()
    + getOrderItems()
*/
class Order {
    int orderID;
    unordered_map<Product*,int>items;
    int totalAmount;
    time_t orderTime;
    public:
    Order(int id, Cart *cart) {
        orderID = id;
        items = cart->getItems();
    }
    Order(int id, Product* p) {
        orderID = id;
        items[p] = 1;
    }
    void trackOrder() {
        cout<<"Order being tracked";
    }
    unordered_map<Product*,int> getOrderItems() {
        cout<<"Order placed successfully";
        cout<<"Product\t\tCount";
        for(auto x:items) {
            Product *p = x.first;
            int count = x.second;
            cout<<(p->name)<<"\t"<<count<<endl;
        }
        return items;
    }
};
/*
    Inventory
    Properties:
    - unordered_map<Product*, int>inventory
    Methods:
    + orderedItems(Order)
*/
class Inventory {
    unordered_map<Product*, int>inventory;
    unordered_map<string, Product*>items;
    public:
    Inventory(unordered_map<Product*, int>obj) {
        inventory = obj;
        for(auto x:obj) {
            Product *p = x.first;
            items[p->name] = p;
        }
    }
    void orderedItems(Order *order) {
        unordered_map<Product*,int>items = order->getOrderItems();
        for(auto x: items) {
            Product *p = x.first;
            int cnt = x.second;
            inventory[p]-=cnt;
            if(inventory[p] <= 0) {
                inventory.erase(inventory.find(p));
            }
        }
    }
    Product* searchProduct(string productName) {
        if(items.find(productName)!=items.end()) {
            return items[productName];
        }
        return NULL;
    }
};
/*
    Amazon
    Properties:
    - unordered_map<productName, Product*>
    - Cart
    - Order
    Methods:
    + searchProduct()
    + addToCart()
    + removeFromCart()
    + orderFromCart()
    + orderDirect()
    Furthermore, addProductReview(), addProductRating()
*/
class Amazon {
    Inventory *inventory;
    Cart *cart;
    Order *order;
    public:
    static int orderID;
    Amazon(unordered_map<Product*, int>invObj) {
        cart = new Cart();
        inventory = new Inventory(invObj);
    }
    Product* searchProduct(string prod) {
        if(inventory->searchProduct(prod)) {
            cout<<"Product found"<<endl;
            return inventory->searchProduct(prod);
        }
        cout<<"Product not found"<<endl;
        return NULL;
    }
    
    bool addToCart(Product *p) {
        cart->addToCart(p);
        cout<<"Added to cart successfully";
        return true;
    }
    bool removeFromCart(Product *p) {
        cart->removeFromCart(p);
        return true;
    }
    void orderFromCart() {
        order = new Order(orderID, cart);
        orderID++;
        order->getOrderItems();
        inventory->orderedItems(order);
    }
    void orderDirect(Product *p) {
        order = new Order(orderID, p);
        orderID++;
        order->getOrderItems();
        inventory->orderedItems(order);
    }
};
int Amazon::orderID = 1;
int main() {
    AuthenticationService *auth = new AuthenticationService;
    auth->signup("user1","pass1");
    
    User *user = auth->login("user1","pass1");
    
    vector<Product*>products;
    
    // Amazon amazon = new Amazon()
    // Payment service
}