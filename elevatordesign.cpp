#include <bits/stdc++.h>
using namespace std;
class ElevatorManager;
// ElevatorObserver -> Panel
class ElevatorObserver
{
    public:
    virtual void update(int a, int b) = 0;
    virtual void requestElevator() = 0;
    virtual void setManager(ElevatorManager *m) = 0;
};
// OuterPanel
class OuterPanel: public ElevatorObserver
{
    ElevatorManager *obj;
    int currentFloor;
    public:
    OuterPanel(int floor)
    {
        currentFloor = floor;
    }
    void update(int elevatorId, int floor) override
    {
        cout<<"Elevator "<<elevatorId<<" at floor "<<floor<<endl;
    }
    void setManager(ElevatorManager *m) override;
    void requestElevator() override;
};
class Elevator;
class Strategy;
// State - movingUp, movingDown, Idle
class State
{
    public:
    virtual string getType() = 0;
};
class MoveUpState: public State
{
    public:
    string getType()
    {
        return "UP";
    }
};
class MoveDownState: public State
{
    public:
    string getType()
    {
        return "DOWN";
    }
};
class IdleState: public State
{
    public:
    string getType()
    {
        return "IDLE";
    }
};
// Shortest distance strategy, load balance strategy
// Elevator
class Elevator
{
    ElevatorManager *manager;
    int id;
    State *currentState;
    int currentFloor;
    queue<int>q; // Floor
    public:
    Elevator(int i, int floor)
    {
        id = i;
        currentFloor = floor;
        setState(new IdleState());
    }
    void addToQueue(int floor)
    {
        q.push(floor);
        cout<<"Elevator "<<id<<" received request to go to floor "<<floor<<endl;
        processQueue();
    }
    void processQueue()
    {
        if(q.empty())
            return;
        int targetFloor = q.front();
        cout<<"Processing req from floor "<<currentFloor<<" to floor "<<targetFloor<<endl;
        if(targetFloor > currentFloor)
        {
            setState(new MoveUpState());
            simulateMovement(targetFloor);
        }
        else if(targetFloor < currentFloor)
        {
            setState(new MoveDownState());
            simulateMovement(targetFloor);
        }
    }
    void setState(State *newState)
    {
        currentState = newState;
    }
    int getCurrentFloor()
    {
        return currentFloor;
    }
    void simulateMovement(int targetFloor)
    {
        while(currentFloor != targetFloor)
        {
            if(targetFloor > currentFloor)
                currentFloor++;
            else if(targetFloor < currentFloor)
                currentFloor--;
            cout<<"Elevator "<<id<<" now at "<<currentFloor<<endl;
        }
        q.pop();
        setState(new IdleState()); // Stop the elevator
    }
};
// Strategy
class Strategy
{
    public:
    virtual Elevator* getElevator(int floor, vector<Elevator*>elevators) = 0;
};
class ShortestDistanceStrategy: public Strategy
{
    public:
    Elevator* getElevator(int floorNum, vector<Elevator*>elevators) override
    {
        int dist = abs(floorNum - elevators[0]->getCurrentFloor()); 
        int elevatorNum = 0;
        for(int i=1; i<elevators.size(); i++)
        {
            int val = abs(floorNum - elevators[i]->getCurrentFloor());
            if(val < dist)
            {
                dist = val;
                elevatorNum = i;
            }
        }
        return elevators[elevatorNum];
    }
};
// Elevator Manager
class ElevatorManager
{
    vector<Elevator*>elevators;
    vector<ElevatorObserver*>panels;
    Strategy *obj;
    ElevatorObserver *ob;
    public:
    void addElevator(Elevator *a)
    {
        elevators.push_back(a);
    }
    void addPanel(ElevatorObserver *p)
    {
        panels.push_back(p);
    }
    void setStrategy(Strategy *a)
    {
        obj = a;
    }
    void addToQueue(int floor)
    {
        // Select any elevator using strategy and then push into elevator queue.
        setStrategy(new ShortestDistanceStrategy());
        Elevator *e = obj->getElevator(floor, elevators);
        if(e)
        {
            e->addToQueue(floor); 
        }
    }
    void notifyObservers()
    {
        int n = elevators.size();
        for(int i=0; i<n; i++)
        {
            panels[i]->update(i+1, elevators[i]->getCurrentFloor());
        }
    }
};
void OuterPanel::requestElevator()
{
    obj->addToQueue(currentFloor);
}
void OuterPanel::setManager(ElevatorManager *m)
{
    obj = m;
}

int main()
{
    Elevator *e1 = new Elevator(1,1);
    Elevator *e2 = new Elevator(2,1);
    Elevator *e3 = new Elevator(3,2);
    
    ElevatorObserver *p1 = new OuterPanel(1);
    ElevatorObserver *p2 = new OuterPanel(2);
    ElevatorObserver *p3 = new OuterPanel(3);
    ElevatorObserver *p4 = new OuterPanel(4);
    
    ElevatorManager *manager = new ElevatorManager();
    manager->addElevator(e1);
    manager->addElevator(e2);
    manager->addElevator(e3);
    
    manager->addPanel(p1);
    manager->addPanel(p2);
    manager->addPanel(p3);
    manager->addPanel(p4);
    p1->setManager(manager);
    p2->setManager(manager);
    p3->setManager(manager);
    p4->setManager(manager);
    
    manager->notifyObservers();
    
    // Request elevator at floor 4
    p4->requestElevator();
    
    delete e1; delete e2; delete e3;
    delete p1; delete p2; delete p3;
    delete manager;
}