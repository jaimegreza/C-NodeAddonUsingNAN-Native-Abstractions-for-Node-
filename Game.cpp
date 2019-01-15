/** Game.cpp **/

#include "./Game.h"

int Game::current_id;

Game::Game(string secret, int diff_level) : game_id(++current_id), m_difficulty_level(diff_level), m_secret(secret)
{
    cout << "difficulty-level-in-game: " << m_difficulty_level << endl;
    
    m_status = GAME_STATUS::INPROGRESS;
    m_remain_guesses = (int) MAX_GUESSES;

    m_last_guess = '#'; // no A-Z a-z 0-9 - ie: not alphanumeric
    m_num_letters_guessed = 0; // beginning of game so no guesses yet

    m_secret_length = m_secret.length();  // when  number of correct letters guessed equals length of word = WIN

    // process secret and store letters and their positions into map
    for (int i = 0;i < m_secret_length;i++)
    {
        list<int> positions;
        char letter = m_secret[i];

        if (m_secret_letters.find(letter) != m_secret_letters.end())
        {
            positions =  m_secret_letters[letter];
            positions.push_back(i);
            m_secret_letters[letter] = positions;

        }
        else
        {
            positions.push_back(i);
            m_secret_letters[letter] = positions;
        }        
    }

/*    for (pair<char, list<int>> p : m_secret_letters)
    {
        for(int p : p.second)
        {
            cout << p << endl;
        }
    }
*/

    cout << "difficulty-level-in-game: " << m_difficulty_level << endl;

}

bool Game::isGameWon()
{
    if (m_status == GAME_STATUS::WON)
        return true;
    else
        return false;
}

bool Game::isGameFinished()
{
    if (m_status == GAME_STATUS::INPROGRESS)
        return false;
    else
        return true;
}


char Game::lastGuess()
{
    return m_last_guess;
}

bool Game::bLastGuessMatched()
{
    return m_last_guess_matched;
}


list<char> Game::lettersGuessed()
{
    list<char> guessedLetters(m_letters_guessed.begin(), m_letters_guessed.end());

    return guessedLetters;
    
}


list<int> Game::lastPositions()
{
    return m_last_positions;
}

int Game::numGuessRemain()
{
    return m_remain_guesses;
}

int Game::secretLength()
{
    return m_secret_length;
}

int Game::gameId()
{
    return game_id;
}

int Game::difficultyLevel()
{
    return m_difficulty_level;
}


int Game::guessLetter(char guess) //  return number of guess left
{
    //if already won just return remaining guesses
    if (m_status == GAME_STATUS::WON)
    {
        return m_remain_guesses;
    }

    // if for some reason called after game ends
    // do not set status since should be set already
    if (m_remain_guesses == 0)  
    {
        return 0;
    }

    // see if letter was guessed already
    if (m_letters_guessed.find(guess) == m_letters_guessed.end())
    {
        m_bAlreadyGuessed = false;
        m_letters_guessed.insert(guess);

    }
    else
    {
        m_bAlreadyGuessed = true;
    }

    m_last_guess = guess;

    if (!m_bAlreadyGuessed)
    {
    
        // check if guess is in the secret
        if (m_secret_letters.find(guess) != m_secret_letters.end())
        {
            m_last_guess_matched = true;
            m_last_positions =  m_secret_letters[guess];
            m_num_letters_guessed += m_last_positions.size();
        }
        else
        {
            m_remain_guesses--; // penalize for bad guess
            m_last_guess_matched = false;
            m_last_positions.clear();
        }
    }

    // check winning or losing conditions

    if (m_remain_guesses == 0) // subtract this current guess
    {
        if (m_num_letters_guessed == m_secret_length)
        {
            m_status = GAME_STATUS::WON;
        }
        else
        {
            m_status  = GAME_STATUS::LOST;
        }
    }
    else // check if won with guesses still available
    {
        if (m_num_letters_guessed == m_secret_length)
        {
            m_status = GAME_STATUS::WON;
        }

    }

    return m_remain_guesses;   // if Game is won this will always be same value so need to check game status
}
 