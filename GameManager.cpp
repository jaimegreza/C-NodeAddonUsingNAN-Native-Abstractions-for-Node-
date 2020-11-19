/** GameManager.cpp **/


#include <iostream>
#include <vector>
#include <sstream>
#include <curl/curl.h>
#include <string>
#include "./GameManager.h"
#include <cstdlib>
#include <ctime>
using namespace std;

/** CLASS VARIABLES **/
GameManager *GameManager::game_Manager_Instance = nullptr;


/** TO DO: make following into a printGameInfo utility functino **/
void printGameStatusInManager(Game *game)
{
    cout << " ----- " << endl;

    cout << "Game ID: " << game->gameId() << endl;
    
    bool bGameFinished = game->isGameFinished();
    cout << "game finished? " << bGameFinished << endl;

    bool bWon = game->isGameWon();
    cout << "game won? " << bWon << endl;

    int difficultyLevel = game->difficultyLevel();
    cout << "difficulty: " << difficultyLevel << endl;

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

    list<char> lettersGuessed = game->lettersGuessed();
    cout << "letters guessed: ";
    
    for (char c : lettersGuessed)
    {
        cout << c << " ";
    }

    cout << endl;

} 

/*** TO DO: make following into a printGameMessage utility function **/
void printGameMessageInManager(game_message_w *gameMessage)
{
    cout << " ----- " << endl;

    cout << "Game ID: " << gameMessage->gameId << endl;
    
    bool bGameFinished = gameMessage->bGameFinished;
    cout << "game finished? " << bGameFinished << endl;

    bool bWon = gameMessage->bWon;
    cout << "game won? " << bWon << endl;

    int difficultyLevel = gameMessage->difficultyLevel;
    cout << "difficulty: " << difficultyLevel << endl;

    int numGuessLeft = gameMessage->numGuessRemaining;
    cout << "num guesses remaining: " << numGuessLeft << endl;

    int secretLength = gameMessage->secretLength;
    cout << "secret length: " << secretLength << endl;

    char lastGuess = gameMessage->lastGuess;
    cout << "last guess: " << lastGuess << endl;

   bool bLastGuessMatched = gameMessage->bLastGuessMatched;
    cout << "last guess matched? " << bLastGuessMatched << endl;

    list<int> pos = gameMessage->positions;
    cout << "last pos: ";
    
    for (int i : pos)
    {
        cout << i << " ";
    }

    list<char> lettersGuessed = gameMessage->lettersGuessed;
    cout << "letters guessed: ";
    
    for (char c : lettersGuessed)
    {
        cout << c << " ";
    }
    cout << endl;

}




size_t write(void* ptr, size_t size, size_t nmemb, string* data)
{
    data->append((char* ) ptr , size * nmemb);
    return size * nmemb;
} 

vector<string> processReachAPI(const string &data)
{
    stringstream stream(data);
    vector<string> result;
    while (1){
        string line;
        getline(stream,line);
        result.push_back(line);
        if (!stream.good())
            break;

    }
    result.push_back("pokemon");
    return result;
}

vector<string> callReachAPI(int difficulty, int start)
{

    auto curl = curl_easy_init();
    string URL =  "http://app.linkedin-reach.io/words?";
    string queryString = "";//= "count=" ;
   // queryString.append(to_string(API_COUNT));

    queryString.append("&minLength=" + to_string(MIN_LENGTH));
    queryString.append("&maxLength=" + to_string(MAX_LENGTH));
    queryString.append("&difficulty=" + to_string(difficulty));
    queryString.append("&start=" + to_string(start));

    string URI = URL + queryString;

    cout << "URI: " << URI << endl;

    curl_easy_setopt(curl, CURLOPT_URL, URI.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
 //   curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, queryString.c_str());

    string response_text;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_text);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl = NULL;

    vector<string> result = processReachAPI(response_text);
    return result;
}


GameManager::GameManager()
{
    cout << "GameManager constructor" << endl;

    m_bExhausted_level1 = false;
    m_bExhausted_level2 = false;
    m_bExhausted_level3 = false;
    m_bExhausted_level4 = false;
    m_bExhausted_level5 = false;
    m_bExhausted_level6 = false;
    m_bExhausted_level7 = false;
    m_bExhausted_level8 = false;
    m_bExhausted_level9 = false;
    m_bExhausted_level10 = false;

    m_level1_start = 0;
    m_level2_start = 0;
    m_level3_start = 0;
    m_level4_start = 0;
    m_level5_start = 0;
    m_level6_start = 0;
    m_level7_start = 0;
    m_level8_start = 0;
    m_level9_start = 0;    
    m_level10_start = 0;

    initialize();
}

GameManager::~GameManager()
{
    cout << "GameManager destructor" << endl;
}


GameManager* GameManager::getInstance()
{
    if (GameManager::game_Manager_Instance == nullptr)
    {
        game_Manager_Instance = new GameManager();
    }

    return game_Manager_Instance;


}

void GameManager::initialize()
{
    cout << "GameManager initialize" << endl;

    /** set up initial cache for each difficulty level **/

    // init rand seed
    srand( time( NULL ) );
    
    vector<string> result;

    for (int i = 1 ; i <= 10 ; i++)
    {
        result = callReachAPI(i,0);
        m_level_cache[i] = result;
    }
    return;

    /* level 1 */
    result = callReachAPI(1,m_level1_start);
    m_level1_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level1 = true;

    for (string ss : result)
    {
        m_level1_cache.push_front(ss);
    }

    /* level 2 */     
    result = callReachAPI(2,m_level2_start);
    m_level2_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level2 = true;

    for (string ss : result)
    {
        m_level2_cache.push_front(ss);
    }

    /* level 3 */
    result = callReachAPI(3,m_level3_start);
    m_level3_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level3 = true;

    for (string ss : result)
    {
        m_level3_cache.push_front(ss);
    }

    /* level 4 */
    result = callReachAPI(4,m_level4_start);
    m_level4_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level4 = true;

    for (string ss : result)
    {
        m_level4_cache.push_front(ss);
    }

    /* level 5 */
    result = callReachAPI(5,m_level5_start);
    m_level5_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level5 = true;

    for (string ss : result)
    {
        m_level5_cache.push_front(ss);
    }

    /* level 6 */
    result = callReachAPI(6,m_level6_start);
    m_level6_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level6 = true;

    for (string ss : result)
    {
        m_level6_cache.push_front(ss);
    }

    /* level 7 */
    result = callReachAPI(7,m_level7_start);
    m_level7_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level7 = true;

    for (string ss : result)
    {
        m_level7_cache.push_front(ss);
    }

    /* leve 8 */
    result = callReachAPI(8,m_level8_start);
    m_level8_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level8 = true;

    for (string ss : result)
    {
        m_level8_cache.push_front(ss);
    }

    /* level 9 */
    result = callReachAPI(9,m_level9_start);
    m_level9_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level9 = true;

    for (string ss : result)
    {
        m_level9_cache.push_front(ss);
    }

    /* level 10 */
    result = callReachAPI(10,m_level10_start);
    m_level10_start += result.size();

    if (result.size() < API_COUNT)
        m_bExhausted_level10 = true;

    for (string ss : result)
    {
        m_level10_cache.push_front(ss);
    }

    
}



bool GameManager::getWordForDifficltyLevel(int difficultyLevel, string &secret)
{
    // get a secret from our cache

    // new code: using random access and map, quickly return for now.
 
    vector<string> result = m_level_cache[difficultyLevel];

    int len = result.size();


    int secretIndex = rand() % len;
    cout << "Manager - get Word - random index: " << secretIndex << endl;
    secret = result[secretIndex];

    // new code: using random access and map, quickly return for now.
    /// refill logic to do later with new map
    return true;

    //m_used_secrets 



    switch (difficultyLevel)
    {
            case 1:
            {
                if ((m_level1_cache.size() == 0) && !m_bExhausted_level1)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level1_cache.size() > 0)
                {
                    secret = m_level1_cache.front(); // get from head of list
                    m_level1_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            case 2:
            {
                if ((m_level2_cache.size() == 0) && !m_bExhausted_level2)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level2_cache.size() > 0)
                {
                    secret = m_level2_cache.front(); // get from head of list
                    m_level2_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            case 3:
            {
                if ((m_level3_cache.size() == 0) && !m_bExhausted_level3)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level3_cache.size() > 0)
                {
                    secret = m_level3_cache.front(); // get from head of list
                    m_level3_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            case 4:
            {
                if ((m_level4_cache.size() == 0) && !m_bExhausted_level4)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level4_cache.size() > 0)
                {
                    secret = m_level4_cache.front(); // get from head of list
                    m_level4_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            case 5:
            {
                if ((m_level5_cache.size() == 0) && !m_bExhausted_level5)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level5_cache.size() > 0)
                {
                    secret = m_level5_cache.front(); // get from head of list
                    m_level5_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            case 6:
            {
                if ((m_level6_cache.size() == 0) && !m_bExhausted_level6)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level6_cache.size() > 0)
                {
                    secret = m_level6_cache.front(); // get from head of list
                    m_level6_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            case 7:
            {
                if ((m_level7_cache.size() == 0) && !m_bExhausted_level7)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level7_cache.size() > 0)
                {
                    secret = m_level7_cache.front(); // get from head of list
                    m_level7_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            case 8:
            {
                if ((m_level8_cache.size() == 0) && !m_bExhausted_level8)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level8_cache.size() > 0)
                {
                    secret = m_level8_cache.front(); // get from head of list
                    m_level8_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            case 9:
            {
                if ((m_level9_cache.size() == 0) && !m_bExhausted_level9)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level9_cache.size() > 0)
                {
                    secret = m_level9_cache.front(); // get from head of list
                    m_level9_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            case 10:
            {
                if ((m_level10_cache.size() == 0) && !m_bExhausted_level10)
                {
                    refillWordsForDifficultyLevel(difficultyLevel);
                }

                if (m_level10_cache.size() > 0)
                {
                    secret = m_level10_cache.front(); // get from head of list
                    m_level10_cache.pop_front();  // remove from list
                    return true;
                }
                else // we exhausted the resource for this level
                    return false;
                break;
            }
            default:
                break;
    }

    return false; // got nuttin
} 


/***
 *   Do not call below if Exhausted_Level is true and level_cache size is 0
 *   resource is exhausted so no need to call API
 ***/
void GameManager::refillWordsForDifficultyLevel(int difficultyLevel)
{
    /** if a cache for a level runs out, refill it with an api call **/

    vector<string> result;
    
    switch (difficultyLevel)
    {
        case 1: // level 1
        {
            result = callReachAPI(difficultyLevel,m_level1_start);
            m_level1_start += result.size();
    
            if (result.size() < API_COUNT)
                m_bExhausted_level1 = true;

            for (string ss : result)
            {
                m_level1_cache.push_front(ss);
            }
            break;
        }

        case 2: // level 2
        {
            result = callReachAPI(difficultyLevel,m_level2_start);
            m_level2_start += result.size();

            if (result.size() < API_COUNT)
                m_bExhausted_level2 = true;

            for (string ss : result)
            {
                m_level2_cache.push_front(ss);
            }
            break;
        }

        case 3: // level 3
        {
            result = callReachAPI(difficultyLevel,m_level3_start);
            m_level3_start += result.size();

            if (result.size() < API_COUNT)
                m_bExhausted_level3 = true;

            if (result.size() < API_COUNT)
                m_bExhausted_level3 = true;

            for (string ss : result)
            {
                m_level3_cache.push_front(ss);
            }
            break;
        }
        case 4: // level 4
        {
            result = callReachAPI(difficultyLevel,m_level4_start);
            m_level4_start += result.size();

            if (result.size() < API_COUNT)
                m_bExhausted_level4 = true;

            for (string ss : result)
            {
                m_level4_cache.push_front(ss);
            }
            break;
        }
        case 5: // level 5
        {
            result = callReachAPI(difficultyLevel,m_level5_start);
            m_level5_start += result.size();

            if (result.size() < API_COUNT)
                m_bExhausted_level5 = true;

            for (string ss : result)
            {
                m_level5_cache.push_front(ss);
            }
            break;
        }
        case 6: // level 6
        {
            result = callReachAPI(difficultyLevel,m_level6_start);
            m_level6_start += result.size();

            if (result.size() < API_COUNT)
                m_bExhausted_level6 = true;

            for (string ss : result)
            {
                m_level6_cache.push_front(ss);
            }
            break;
        }
        case 7: // level 7
        {
            result = callReachAPI(difficultyLevel,m_level7_start);
            m_level7_start += result.size();

            if (result.size() < API_COUNT)
                m_bExhausted_level7 = true;

            for (string ss : result)
            {
                m_level7_cache.push_front(ss);
            }
            break;
        }
        case 8: // level 8
        {
            result = callReachAPI(difficultyLevel,m_level8_start);
            m_level8_start += result.size();

            if (result.size() < API_COUNT)
                m_bExhausted_level8 = true;

            for (string ss : result)
            {
                m_level8_cache.push_front(ss);
            }
            break;
        }
        case 9: // level 9
        {
            result = callReachAPI(difficultyLevel,m_level9_start);
            m_level9_start += result.size();

            if (result.size() < API_COUNT)
                m_bExhausted_level9 = true;

            for (string ss : result)
            {
                m_level9_cache.push_front(ss);
            }
            break;
        }
        case 10: // level 10
        {
            result = callReachAPI(difficultyLevel,m_level10_start);
            m_level10_start += result.size();

            if (result.size() < API_COUNT)
                m_bExhausted_level10 = true;

            for (string ss : result)
            {
                m_level10_cache.push_front(ss);
            }
            break;
        }
        default:
            break;
    }


}

game_message_w* GameManager::makeGameMessage(Game *game, bool bIsGood)
{
    game_message_w * gameMessage = new game_message_w();

    if (!bIsGood || (game == nullptr))
    {
        // use -1 for GameId which is default in struct game_message_w
        // rest of game message is meaningless when game id -1
        return gameMessage;
    }

    gameMessage->gameId = game->gameId();
    gameMessage->difficultyLevel = game->difficultyLevel();
    gameMessage->bGameFinished = game->isGameFinished();
    gameMessage->bWon = game->isGameWon();
    gameMessage->numGuessRemaining = game->numGuessRemain();
    gameMessage->secretLength = game->secretLength();
    gameMessage->lastGuess = game->lastGuess();
    gameMessage->bLastGuessMatched = game->bLastGuessMatched();

    list<int> positions = game->lastPositions();
    gameMessage->positions = positions;

    list<char> lettersGuessed = game->lettersGuessed();
    gameMessage->lettersGuessed = lettersGuessed;

    /** TODO: comment out printing of game message **/
    printGameMessageInManager(gameMessage);
 
    return gameMessage;

}


/**** pubically exposed interface to SecretKeeper ***/


game_message_w* GameManager::generateGame(int difficultyLevel)
{
    string secret;
    bool bSecretFound = getWordForDifficltyLevel(difficultyLevel, secret); // returns false if exhausted

    cout << "GameManager::generateGame:  secret: " << secret << endl;

    cout << "GameManager::generateGame:  bSecretFound: " << bSecretFound << endl;

    if (!bSecretFound)
        return makeGameMessage(nullptr, false);

    // insert secret into used secrets
    // it shouldn't be in there if the caching mechanism is working 
    // but check to be safe
    if (m_used_secrets.find(secret) == m_used_secrets.end())
    {
        m_used_secrets.insert(secret);
    }

    // make a new Game
    cout << "difficulty-level-in-game-manager: " << difficultyLevel << endl;
    Game *game = new Game(secret, difficultyLevel);

    //printGameStatusInManager(game);
    
    // put it into the map
    // check if it's in there , shouldn't be since autoincrement id
    //
    if (m_game_map.find(game->gameId()) == m_game_map.end())
    {
        //m_game_map[game->gameId()] = game;
        m_game_map.insert(pair<int, Game*>(game->gameId(), game));
    }

    // return our game message, should be in initial state
    // and game in storage

    return makeGameMessage(game, true);

}

game_message_w* GameManager::guessGame(int gameId, char guess)
{
    if (gameId < 0)
        return makeGameMessage(nullptr, false);

    Game *game = nullptr;
    // check if it's in the map ,  should be there
    //
    if (m_game_map.find(gameId) != m_game_map.end())
    {
        game = m_game_map[gameId];

        cout << "guess game: null ptr? " << (game == nullptr) << endl;
    }
   
    if (game == nullptr)
        return makeGameMessage(nullptr, false);

    // change the state of the game by guessing the letter
    game->guessLetter(guess);

    // stuff it back - not sure if needed since game pointer
    // and the Game Manager is the only one with access to the pointers
    m_game_map.insert(pair<int, Game*>(game->gameId(), game));
    //m_game_map[game->gameId()] = game;

    // return our game message, should be in a changed state
    // and game in storage

    return makeGameMessage(game, true);

}


game_message_w* GameManager::getGameInfo(int gameId)
{

    if (gameId < 0)
        return makeGameMessage(nullptr, false);

    Game *game = nullptr;
    // check if it's in the map ,  should be there
    //
    if (m_game_map.find(gameId) != m_game_map.end())
    {
        game = m_game_map[gameId];
    }
   
    if (game == nullptr)
        return makeGameMessage(nullptr, false);

    return makeGameMessage(game, true);

}
