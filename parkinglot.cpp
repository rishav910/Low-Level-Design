#include <bits/stdc++.h>
using namespace std;
/*
    User
    Properties: -username, -password, -loggedIn
    Methods: +login(), +logout()
*/
class User {
    public:
    string username;
    string password;
    bool loggedIn;
    
    User(string user, string pass): username(user), password(pass), loggedIn(false){}
    
    void login() {
        loggedIn = true;
        cout<<"User logged in successfully"<<endl;
    }
    void logout() {
        loggedIn = false;
        cout<<"User logged out successfully"<<endl;
    }
};
/*
    Authentication service
    Checks username, password, helps in login, logout, signup
    Properties:
    - map<username, User> users
    Methods:
    + signup(user,pass)
    + login(user,pass)
    + logout(User)
*/
class AuthenticationService {
    unordered_map<string, User*> users;
    
    public:
    void signup(string username, string password) {
        if(users.find(username) == users.end()) {
            users[username] = new User(username, password);
            cout<<"Signed up successfully"<<endl;
        } else {
            cout<<"User already exists"<<endl;
        }
    }
    
    User* login(string username, string password) {
        if(users.find(username)!=users.end() && users[username]->password == password) {
            users[username]->login();
            return users[username];
        } else {
            cout<<"Invalid credentials"<<endl;
            return NULL;
        }
    }
    
    void logout(User *user) {
        if(user) {
            user->logout();
        }
    }
};
/*
    Vehicle - Car, MotorCycle, ElectricCar
    Methods:
    + getType()
    + getCharges(hours)
*/
class Vehicle {
    public:
    virtual string getType() = 0;
    virtual double getCharges(int hours) = 0;
};

class Car: public Vehicle {
    public:
    string getType() override {
        return "Car";
    }
    double getCharges(int hours) override {
        return hours*20.0;
    }
};
class MotorCycle: public Vehicle {
    public:
    string getType() override {
        return "MotorCycle";
    }
    double getCharges(int hours) override {
        return hours*10.0;
    }
};
class ElectricCar: public Vehicle {
    public:
    string getType() override {
        return "ElectricCar";
    }
    double getCharges(int hours) override {
        return hours*30.0;
    }
};
/*
    Parking Space/Spot
    Properties:
    - id
    - isOccupied
    - isReserved
    - Vehicle* vehicle
    - reservationTime
    - User* reservedBy
    Methods:
    + parkVehicle(Vehicle)
    + vacateSpot()
    + reserveSpot(User, reservationTime)
    + cancelReservation()
*/
class ParkingSpot {
    public:
    int spotId;
    time_t reservationTime;
    Vehicle* vehicle;
    User* reservedBy;
    bool isOccupied;
    bool isReserved;
    
    ParkingSpot(int id) {
        spotId = id;
        isOccupied = isReserved = false;
        reservationTime = 0;
        vehicle = NULL;
        reservedBy = NULL;
    }
    bool parkVehicle(Vehicle *v) {
        if(!isOccupied && !isReserved) {
            vehicle = v;
            isOccupied = true;
            return true;
        }
        return false;
    }
    void vacateSpot() {
        isOccupied = isReserved = false;
        vehicle =  NULL;
        reservedBy = NULL;
    }
    bool reserveSpot(User *user, time_t resTime) {
        if(!isOccupied && !isReserved) {
            isReserved = true;
            reservedBy = user;
            reservationTime = resTime;
            return true;
        }
        return false;
    }
    void cancelReservation() {
        isReserved = false;
        reservedBy = NULL;
        reservationTime = 0;
    }
};
/*
    Payment
    Methods:
    + processPayment(Vehicle, hours)
*/
class Payment {
    public:
    double processPayment(Vehicle *v, int hours) {
        double cost = v->getCharges(hours);
        cout<<"Processing payment of Rs."<<cost<<" for parking "<<v->getType()<<" for "<<hours<<" hours"<<endl;
        return cost;
    }
};
/*
    3D Navigation
    Methods:
    + show3DView(ParkingSpot)
*/
class Navigation {
    public:
    void show3DView() {
        cout<<"Showing 3D view of the parking spot"<<endl;
    }
};
/*
    Parking Lot
    Properties:
    - vector<ParkingSpot> spots
    - unordered_map<Vehicle, time_t> vehicleEntryTime;
    - unordered_map<username, ParkingSpot>reservedSpots;
    Methods:
    + findAvailableSlot()
    + parkVehicle(Vehicle)
    + reserveSpot(User, spotId, reservationTime)
    + cancelReservation(User)
*/
class ParkingLot {
    vector<ParkingSpot*>spots;
    unordered_map<Vehicle*, time_t> vehicleEntryTime;
    unordered_map<string, ParkingSpot*>reservedSpots;
    
    public:
    ParkingLot(int n) {
        for(int i=0; i<n; i++) {
            spots.push_back(new ParkingSpot(i));
        }
    }
    int findAvailableSlot() {
        int n = spots.size();
        for(int i=0; i<n; i++) {
            if(!spots[i]->isOccupied && !spots[i]->isReserved) {
                return i;
            }
        }
        return -1;
    }
    bool parkVehicle(Vehicle *v) {
        int id = findAvailableSlot();
        if(id == -1) {
            cout<<"No parking spots available"<<endl;
            return false;
        } 
        if(spots[id]->parkVehicle(v)) {
            vehicleEntryTime[v] = time(NULL);
            // Ask for payment here or by client at their usage
            cout<<"Vehicle parked at "<<id<<" spot"<<endl;
            return true;
        }
        return false;
    }
    bool reserveSpot(User *user, int spotId, time_t resTime) {
        if(spotId < 0 || spotId >= spots.size()) {
            cout<<"Invalid reservation spot"<<endl;
            return false;
        }
        if(spots[spotId]->reserveSpot(user, resTime)) {
            reservedSpots[user->username] = spots[spotId];
            cout<<"Vehicle spot reserved at "<<spotId<<" spot"<<endl;
            return true;
        }
        cout<<"Spot not available at the moment"<<endl;
        return false;
    }
    bool cancelReservation(User *user) {
        if(reservedSpots.find(user->username)!=reservedSpots.end()) {
            ParkingSpot *p = reservedSpots[user->username];
            p->cancelReservation();
            reservedSpots.erase(user->username);
            return true;
        }
        cout<<"No parking spot reserved to unreserve";
        return false;
    }
};
int main()
{
    AuthenticationService *auth = new AuthenticationService;
    ParkingLot *parking = new ParkingLot(5);
    Navigation *navigation = new Navigation;
    Payment *paymentService = new Payment;
    
    auth->signup("user1", "pass1");
    User *user = auth->login("user1", "pass1");
    
    if(user) {
        time_t reservationTime = time(NULL);
        parking->reserveSpot(user, 1, reservationTime);
        
        Vehicle *v1 = new Car(), *v2 = new MotorCycle();
        parking->parkVehicle(v1);
        navigation->show3DView();
        paymentService->processPayment(v1, 2);
        
        parking->parkVehicle(v2);
        navigation->show3DView();
        paymentService->processPayment(v2, 3);
        
    }
}