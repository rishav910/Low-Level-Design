#include<bits/stdc++.h>
using namespace std;
/*
    Dice - Normal Dice, Crooked Dice
    Methods:
    + rollDice()
*/
class Dice
{
    public:
    virtual int rollDice() = 0;
};
class NormalDice: public Dice
{
    public:
    int rollDice() override
    {
        return (rand()%6)+1;
    }
};
class CrookedDice: public Dice
{
    vector<int>v;
    public:
    CrookedDice()
    {
        v = {2,4,6};
    }
    int rollDice() override
    {
        return v[rand()%3];
    }
};
/*
    Board
    Properties;
    - snake, ladder
    Methods:
    + addSnake(start, end)
    + addLadder(start, end)
    + snaked(state)
    + laddered(state)
*/
class Board
{
    unordered_map<int,int>snake, ladder;
    public:
    void addSnake(int start, int end)
    {
        if(start <= end)
        {
            cout<<"Invalid snake"<<endl;
            return;
        }
        cout<<"Snake registered"<<endl;
        snake[start] = end;
    }
    void addLadder(int start, int end)
    {
        if(start >= end)
        {
            cout<<"Invalid ladder"<<endl;
            return;
        }
        cout<<"Ladder registered"<<endl;
        ladder[start] = end;
    }
    void snaked(int &state)
    {
        if(snake.find(state)!=snake.end())
        {
            cout<<"Snake found moved from "<<state<<" to "<<snake[state]<<endl;
            state = snake[state];
        }
    }
    void laddered(int &state)
    {
        if(ladder.find(state)!=ladder.end())
        {
            cout<<"Ladder found moved from "<<state<<" to "<<ladder[state]<<endl;
            state = ladder[state];
        }
    }
};
/*
    Player
    Properties:
    - name
    - currentState
    Methods:
    + getCurrentState()
*/
class Player
{
    string name;
    int currentState;
    public:
    Player(string a):name(a),currentState(1){}
    int getCurrentState()
    {
        return currentState;
    }
    void updateState(int newState)
    {
        currentState = newState;
    }
    string getName()
    {
        return name;
    }
};
class SnakeLadder
{
    vector<Player*>players;
    Board *board;
    Dice *dice;
    unordered_set<Player*>won;
    public:
    SnakeLadder(vector<Player*>players, Dice *d)
    {
        this->players = players;
        dice = d;
        board = new Board();
    }
    void addSnake(int start, int end)
    {
        board->addSnake(start,end);
    }
    void addLadder(int start, int end)
    {
        board->addLadder(start, end);
    }
    void singlePlayerPlay(Player *p)
    {
        cout<<"\nPlayer "<<p->getName()<<": \n\n";
        int steps = dice->rollDice();
        cout<<"Dice outcome: "<<steps<<endl;
        if(p->getCurrentState() + steps > 100)
            return;
        int state = p->getCurrentState() + steps;
        cout<<"Moved from "<<(p->getCurrentState())<<" to "<<state<<endl;
        if(state == 100)
        {
            cout<<(p->getName())<<" has won!!!"<<endl;
            p->updateState(state);
            won.insert(p);
        }
        board->snaked(state);
        board->laddered(state);
        p->updateState(state);
    }
    void allPlayersPlay()
    {
        for(auto p:players)
        {
            if(won.find(p)==won.end())
                singlePlayerPlay(p);
        }
    }
    bool anybodyWon()
    {
        return won.size()>0;
    }
};
int main()
{
    srand(time(NULL));
    Player *p1 = new Player("Rishav");
    Player *p2 = new Player("Shweta");
    Player *p3 = new Player("Ruma");
    Player *p4 = new Player("Barun");
    vector<Player*>players;
    players.push_back(p1);players.push_back(p2);
    players.push_back(p3);players.push_back(p4);
    Dice *d = new CrookedDice();
    
    SnakeLadder *game = new SnakeLadder(players, d);
    game->addSnake(14, 7);
    game->addSnake(53, 32);
    game->addSnake(97, 68);
    game->addSnake(89, 77);
    game->addLadder(11, 21);
    game->addLadder(88, 99);
    game->addLadder(43, 84);
    
    while(!game->anybodyWon())
    {
        game->allPlayersPlay();
    }
}