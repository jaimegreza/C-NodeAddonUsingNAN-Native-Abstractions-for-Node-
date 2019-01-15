
/** SecretKeeper.cpp **/


#include "./SecretKeeper.h"

SecretKeeper::~SecretKeeper()
{

}

SecretKeeper::SecretKeeper()
{

    gameManager = GameManager::getInstance();

    cout << "SecretKeeper " << endl;
}

game_message_w* SecretKeeper::startGame(int difficultyLevel)
{
    game_message_w* gameMessage =  gameManager->generateGame(difficultyLevel);

    return gameMessage;
} 

game_message_w* SecretKeeper::guessGame(int gameId, char guess)
{
    game_message_w* gameMessage =  gameManager->guessGame(gameId, guess);

    return gameMessage;
}

game_message_w* SecretKeeper::getGameInfo(int gameId)
{
    game_message_w* gameMessage =  gameManager->getGameInfo(gameId);

    return gameMessage;
}