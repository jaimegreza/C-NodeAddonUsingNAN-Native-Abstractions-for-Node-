/** GameManager.h **/

#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_


#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "./Game.h"
#include "./game_message.h"

using namespace std;

#define API_COUNT 10
#define MIN_LENGTH 3
#define MAX_LENGTH 6

class GameManager
{

private:

    GameManager();
    virtual ~GameManager();
    static GameManager* game_Manager_Instance; /// Singleton pattern, only 1 instance of this gaming service

/**** caches to store Secrets per difficulty level after calls to Reach API 
 *   One cache per difficulty level. 
 *  Initally store 10 secrets per difficulty level then replinish as needed
 *  REACH rest api provides a START position and a COUNT , we will just get 10 secrets at a time - API_COUNT
 *  we will store the current position of the last accessed secret in a member variables per difficulty level
 * ***/ 

/***  I use lists for easy removal of a secret and refill when size = 0
 *  If i used vector would have to use extra int ptr to see last index removed or available
 *  and maintain the ptr. Using list size to track when it gets to 0 size.
 ***/

    list<string> m_level1_cache;   
    list<string> m_level2_cache;   
    list<string> m_level3_cache;
    list<string> m_level4_cache;
    list<string> m_level5_cache;
    list<string> m_level6_cache;
    list<string> m_level7_cache;
    list<string> m_level8_cache;
    list<string> m_level9_cache;
    list<string> m_level10_cache;

/**  I will start calling the REACH api at 0 start and count of 10
 *   I will keep track of the new start depending on if all 10 secrets were returned
 *   If the API suddenly returns less than 10 I know that difficult level has been exhausted of secrets
 *   (since it is a REST api i verified it by calling with start of 0 and count of 10 for each difficulty level)
 *   verified get same results over and over so that I can trust the caching mechanism I am using.
 *   since a REST Get is only provided it should be idempotent per URI api?difficulty=&start=&count=10
 *   i am not using minLength and maxLength for this caching mechanism 
 *   but may set the minLength to 3 and maxLength to 6 for more challenging games so don't have to
 *  keep exhausting easy games to get through more difficult ones. 
 *  (maybe use randomisation later or user preference when users are added)
 * */
    int m_level1_start;
    int m_level2_start;
    int m_level3_start;
    int m_level4_start;
    int m_level5_start;
    int m_level6_start;
    int m_level7_start;
    int m_level8_start;
    int m_level9_start;    
    int m_level10_start;

/** whenever the number of returned API secrets is less than 10 (API_COUNT) no need to call api anymore
 * means we exhausted that resource 
 * we can set minLength to maxLength and try again but for demo is ok for now
 * */

    bool m_bExhausted_level1;
    bool m_bExhausted_level2;
    bool m_bExhausted_level3;
    bool m_bExhausted_level4;
    bool m_bExhausted_level5;
    bool m_bExhausted_level6;
    bool m_bExhausted_level7;
    bool m_bExhausted_level8;
    bool m_bExhausted_level9;
    bool m_bExhausted_level10;

/** use hashset to store all the secrets that are currently used or previously used
 *  all unused secrets will be in the cache until they are fetched
 *  I then put them in the 
 * */

    unordered_set<string> m_used_secrets;

/**** use hashmap to store current games and finished games 
 *  later I can have 2 hashmaps: one for current games with state still changing (Game objects)
 *  and another hashmap with some kind of memo of the last state of a finished game
 *  this can be stored later or retrieved from a database but for now put all games in one hashmap
 *  I know memory footprint will increase but for this demo i thin it is fine
 * ***/
    unordered_map<int, Game*> m_game_map;

/*** Private internal methods for this Game Service Manger ****/

/**
* build up tables of secrets per difficulty level via the provided Reach API
**/
    void initialize(); 

    /*  get a secret for the specified difficulty level
    *   returns false there are no more secrets for that difficulty level
    *   return true if there was a secret found
    *   use it to specify gameId -1, game not available
    */
    bool getWordForDifficltyLevel(int difficultyLevel, string &secret); 
	
/**
*  if no word in cache need to refill from API
**/
    void refillWordsForDifficultyLevel(int difficultyLevel); 


/***
 *  create a game_message from a Game,
 *   if the game is not good, no secret availale, use -1 for gameId in message
 ***/
    game_message_w* makeGameMessage(Game * game, bool bIsGood);


public:
    /** Public methods to provide an interface to obtain and to call into this Game Service 
     *  game_message_w Messages are used for communication. 
     *  later on , can serialize this message for RPC if need to separate SecretKeeper application
     *  from this Game Manager service.
     * */

    static GameManager *getInstance();

    game_message_w* generateGame(int difficultyLevel);  // return nullptr for a Game with -1 id , exhaust difficulty 
    game_message_w* guessGame(int gameId, char guess);  // change the state of the specified game with gameId
    game_message_w* getGameInfo(int gameId);  // get current state of game

};

#endif /* GAME_MANAGER_H_ */
