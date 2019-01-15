#include <iostream>
#include <list>

#include "./SecretKeeper.h"
#include "./game_message.h"

using namespace std;


    // uncomment below for testng of creating games directly without using service //
/*
void printGameStatus(Game *game)
{
    cout << " ----- " << endl;

    cout << "Game ID: " << game->gameId() << endl;
    
    bool bGameFinished = game->isGameFinished();
    cout << "game finished? " << bGameFinished << endl;

    bool bWon = game->isGameWon();
    cout << "game won? " << bWon << endl;

    int numGuessLeft = game->numGuessRemain();
    cout << "num guesses remaining: " << numGuessLeft << endl;

    int secretLength = game->secretLength();
    cout << "secret length: " << secretLength << endl;

    char lastGuess = game->lastGuess();
    cout << "last guess: " << lastGuess << endl;

    bool bLastGuessMatched = game->bLastGuessMatched();
    cout << "last guess matched? " << bLastGuessMatched << endl;


    list<int> pos = game->lastPositions();
    cout << "last pos: ";
    
    for (int i : pos)
    {
        cout << i << " ";
    }

    cout << endl;

} */

int main()
{

    SecretKeeper keeper;

    game_message_w *gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    gameMessage = keeper.startGame(3);
    
    gameMessage = keeper.guessGame(gameMessage->gameId,'a');
    gameMessage = keeper.guessGame(gameMessage->gameId,'b');
    gameMessage = keeper.guessGame(gameMessage->gameId,'b');

    

        
    // uncomment below for testng of creating games directly without using service //
    /*
    Game game("tet", 3);
    int numLeft=0;
    printGameStatus(&game);
    numLeft = game.guessLetter('a');
    printGameStatus(&game);
    numLeft = game.guessLetter('a');
    printGameStatus(&game);
    numLeft = game.guessLetter('b');
    printGameStatus(&game);
    numLeft = game.guessLetter('f');
    printGameStatus(&game);
    numLeft = game.guessLetter('e');
    printGameStatus(&game);
    numLeft = game.guessLetter('e');
    printGameStatus(&game);
    numLeft = game.guessLetter('x');
    printGameStatus(&game);
    numLeft = game.guessLetter('y');
    printGameStatus(&game);

    numLeft = game.guessLetter('z');
    printGameStatus(&game);

    Game game2("hiya", 3);
    numLeft=0;
    printGameStatus(&game2);
    numLeft = game2.guessLetter('h');
    printGameStatus(&game2);
    numLeft = game2.guessLetter('i');
    printGameStatus(&game2);
    numLeft = game2.guessLetter('a');
    printGameStatus(&game2);
    numLeft = game2.guessLetter('y');
    printGameStatus(&game2);
    */

}