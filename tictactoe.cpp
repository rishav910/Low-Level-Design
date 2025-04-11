#include<bits/stdc++.h>
using namespace std;
// Design Tic Tac Toe
/*
    Tic Tac Toe
    Properties:
    - board
    - currentPlayer
    Methods:
    + isBoardFull()
    + printBoard()
    + checkWinner()
    + getCurrentPlayer()
    + makeMove()
*/
class TicTacToe
{
    vector<vector<char>>board; // 3*3 board
    int currentPlayer;
    public:
    TicTacToe()
    {
        board.resize(3, vector<char>(3, ' '));
        currentPlayer = 1;
    }
    void printBoard()
    {
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                cout<<board[i][j];
                if(j<2)
                    cout<<"|";
            }
            if(i<2)
                cout<<"\n______"<<endl;
        }
    }
    bool isBoardFull()
    {
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
                if(board[i][j] == ' ')
                    return false;
        }
        return true;
    }
    bool makeMove(int row, int col)
    {
        if(row<0 || row>=3 || col<0 || col>=3 || board[row][col]!=' ')
        {
            return false;
        }
        board[row][col] = (currentPlayer==1) ? 'X':'0';
        currentPlayer = (currentPlayer==1) ? 2:1;
        return true;
    }
    bool checkWinner()
    {
        // Check rows & columns
        for(int i=0; i<3; i++)
        {
            if(board[i][0]!=' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
                return true;
            if(board[0][i]!=' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
                return true;
        }
        // Check for 2 diagonals
        if(board[0][0]!=' ' && board[0][0]==board[1][1] && board[1][1] == board[2][2])
            return true;
        if(board[2][0]!=' ' && board[2][0]==board[1][1] && board[1][1] == board[0][2])
            return true;
        return false;
    }
    int getCurrentPlayer()
    {
        return currentPlayer;
    }
};
int main()
{
    /*
        Client code:
        1. Create a class object
        2. Run a loop until board is full or winner is determined
            {
                Get inputs from player1/2 -> row, col
                Check successful/invalid move
            }
        3. Check winner if makeWinner()==true
        4. Else Draw
    */
    TicTacToe game;
    while(!game.isBoardFull() && !game.checkWinner())
    {
        game.printBoard();
        cout<<"\nPlayer "<<game.getCurrentPlayer()<<" enter row,col: ";
        int row,col;
        cin>>row>>col;
        if(game.makeMove(row,col))
            cout<<"Move successful"<<endl;
        else
            cout<<"Invalid move, try again"<<endl;
    }
    
    if(game.checkWinner())
    {
        int winner = (game.getCurrentPlayer()==1)?2:1;
        cout<<"Player "<<winner<<" wins!!!";
    }
    else
    {
        cout<<"Draw !!!";
    }
}