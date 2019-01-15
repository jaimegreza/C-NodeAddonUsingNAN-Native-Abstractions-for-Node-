/** SecretKeeper.h **/

#ifndef SECRET_KEEPER_H_
#define SECRET_KEEPER_H_

#include <iostream>
#include "./GameManager.h"
#include "./game_message.h"

using namespace std;

/** Our Library is intended to be a stateless proxy between the api request and the Game Manager.
 *  Several instantiations of this class from various user calls into the api will all go to 
 * the same instance of the Game Manager so Game state is kept safe.
 * */

class SecretKeeper
{
private:
    GameManager *gameManager;

public:
    SecretKeeper();
    virtual ~SecretKeeper();


    game_message_w* startGame(int difficultyLevel); // initially, return a new Game. if Id = -1 not more words for that difficulty level
    game_message_w* guessGame(int gameId, char guess); // return the Game with state changes.	
    game_message_w* getGameInfo(int gameId); // return current state of the game

};

#endif /* SECRET_KEEPER_H */
