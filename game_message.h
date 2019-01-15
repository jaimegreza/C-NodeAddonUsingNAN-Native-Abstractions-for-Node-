/** game_message.h **/

#ifndef GAME_MESSAGE_H_
#define GAME_MESSAGE_H_

#include <list>

using namespace std;

/** struct for using to pass messages between GameManger service and the Secret Keeper 
 *  later on they can be a separate process and would have to marshal this message over the wire
 *  initially thought to just use Game objects to pass between but gameId is static and 
 *  difficult to set to -1 to indicate game unvailable  so just went for messages instead
 *  could have used a boolean for gameUnvailable but can serialize a struct,
 *  if GameManger gave out pointer to Game object someone might make calls into it and change its state
 *  so better to use messages and let GameManager alter the state of the Games.
 * */

struct game_message_w
{
    int gameId;  // gameId -1 for unavailable/exhausted game
    int difficultyLevel; 
    bool bGameFinished; // is game not in progress?
    bool bWon;     // is the game over and won?

    int numGuessRemaining;  // num guesses remaining
    int secretLength; // length of secret word
    char lastGuess;   // last guessed character
    bool bLastGuessMatched; // did last character guess match any letter in secret

    list<int> positions;  // positions in secret of letters that matched guess

    list<char> lettersGuessed; // all chars of letters that were guessed for the game
    
    game_message_w() : gameId(-1), 
    bGameFinished(false),
    bWon(false),
    bLastGuessMatched(false){}
};

#endif /* game_message.h */