/** Game.h **/ 

#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <string>
#include <list>
#include <set>
#include <map>


#define MAX_GUESSES 6;

enum GAME_STATUS
{
    INPROGRESS = 0,
    WON = 1,
    LOST = 2
};

using namespace std;

class Game
{

private:
    static int current_id; // increment for unique id generation

    int game_id;  // unique Id of game
    int m_difficulty_level; // difficulty level

    string m_secret;  // secret to guess
    int m_secret_length;     // secret word length
    GAME_STATUS m_status;  // current status of game

    map<char, list<int>> m_secret_letters; // map of all letters and their positions in secret word.

    int m_remain_guesses; // number of guesses remaining. game ends when hits 0

    set<char> m_letters_guessed;   // set of all letters user guessed

    /* last user interaction results */
    int m_num_letters_guessed; // when num letters guessed is length of secret game is over and won

    bool m_bAlreadyGuessed = false; // see if already guessed
    char m_last_guess;         // last letter user guest

    bool m_last_guess_matched = false; // read positions if last guess matched
    list<int> m_last_positions;   // positions of letter last user guessed.



public:
    Game(){};
    Game(string secret, int diff_level);
    int guessLetter(char guess); //  return if letter guessed aleady
    bool isGameWon();
    bool isGameFinished();
    char lastGuess();
    bool bLastGuessMatched();
    list<int> lastPositions();
    list<char> lettersGuessed();
    int numGuessRemain();
    int secretLength();
    int gameId();
    int difficultyLevel();



};


#endif /* GAME_H_ */