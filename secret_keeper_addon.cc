/** secret_keeperaddon.cc **/

#include "secret_keeper_addon.h"

using namespace std;

/* helper for checking passing integers to NAN methods */

bool isValidDifficultyLevel(string diffLevelStr)
{
    std::string::size_type sz;
    int diffLevel = std::stoi (diffLevelStr,&sz);

    if ((diffLevel >= 1) && (diffLevel <= 10))
        return true;
    
    return false;

}

bool isValidGameId(string gameIdStr)
{
    std::string::size_type sz;
    int gameId = std::stoi (gameIdStr,&sz);

    if (gameId > 0)
        return true;
    
    return false;
}

/** valid guesses are lower case letters a - z  **/

bool isValidGuess(string guess)
{
    cout << "valid guess: (str) " + guess << endl;
    if (guess.length() > 1)
        return false;

    const char *c;
    c = guess.c_str();

    cout << "valid guess: (char) " << c << endl;

    if (((*c - 'a') >= 0) && ((*c - 'a') <= 25) )
    {
        return true;
    }

    return false;
}

/** convert a Game Message to a string */

string convertGameMessageToJsonString(game_message_w * gameMessage)
{

    string gameMessageString("{ ");

    gameMessageString.append("\"gameId\": " + std::to_string(gameMessage->gameId));
    gameMessageString.append(", ");

//    gameMessageString.append("\"bGameFinished\": " + std::to_string(gameMessage->bGameFinished));
//    gameMessageString.append(", ");

    gameMessageString.append("\"bGameFinished\": ");
    gameMessage->bGameFinished ? gameMessageString.append("true") : gameMessageString.append("false");
    gameMessageString.append(", ");


//    gameMessageString.append("\"bWon\": " + std::to_string(gameMessage->bWon));
//    gameMessageString.append(", ");

    gameMessageString.append("\"bWon\": ");
    gameMessage->bWon ? gameMessageString.append("true") : gameMessageString.append("false");
    gameMessageString.append(", ");


    gameMessageString.append("\"difficultyLevel\": " + std::to_string(gameMessage->difficultyLevel));
    gameMessageString.append(", ");

    gameMessageString.append("\"numGuessRemaining\": " + std::to_string(gameMessage->numGuessRemaining));
    gameMessageString.append(", ");

    gameMessageString.append("\"secretLength\": " + std::to_string(gameMessage->secretLength));
    gameMessageString.append(", ");


    gameMessageString.append("\"lastGuess\": \"");
    gameMessageString.push_back(gameMessage->lastGuess);
    gameMessageString.append("\", ");

//    gameMessageString.append("\"bLastGuessMatched\": " + std::to_string(gameMessage->bLastGuessMatched));
//    gameMessageString.append(", ");

    gameMessageString.append("\"bLastGuessMatched\": ");
    gameMessage->bLastGuessMatched ? gameMessageString.append("true") : gameMessageString.append("false");
    gameMessageString.append(", ");



    gameMessageString.append("\"positions\": [");

    list<int> pos = gameMessage->positions;
    
    list<int>::iterator itr = pos.begin();

    for (int i = 0; i < pos.size() ; i++)
    {
        gameMessageString.append(std::to_string((int) *itr));
        itr++;

        if (i < pos.size() -1)
            gameMessageString.append(" , ");
    }
    gameMessageString.append(" ] ");
    gameMessageString.append(",");

    gameMessageString.append("\"lettersGuessed\": [");

    list<char> lettersGuessed = gameMessage->lettersGuessed;

    list<char>::iterator citr = lettersGuessed.begin();

    for (int i = 0; i < lettersGuessed.size() ; i++)
    {
        gameMessageString.append("\"");
        gameMessageString.push_back((char) *citr);
        gameMessageString.append("\"");
        citr++;

        if (i < lettersGuessed.size() -1)
            gameMessageString.append(" , ");
    }
    gameMessageString.append(" ] ");
    gameMessageString.append("}");

    return gameMessageString;
}



NAN_METHOD(HelloWorld) {
    Nan::HandleScope scope;
    //if (info.Length() < 3) {
     //   Nan::ThrowTypeError("3 arguments expected");
      //  return;
   // }
        
    game_message_w * gameMessage = new game_message_w();

    gameMessage->gameId = 1;
    gameMessage->difficultyLevel = 3;
    gameMessage->bGameFinished = false;
    gameMessage->bWon = true;
    gameMessage->numGuessRemaining = 3;
    gameMessage->secretLength = 5;
    gameMessage->lastGuess = 'a';
    gameMessage->bLastGuessMatched = false;

    list<int> positions = {1, 2, 4};
    gameMessage->positions = positions;

    list<char> lettersGuessed = {'a','b','c'};
    gameMessage->lettersGuessed = lettersGuessed;

    string gameMessageStr = convertGameMessageToJsonString(gameMessage);
    
    
    Nan::JSON NanJSON;
    v8::Local<v8::String> json_string = Nan::New(gameMessageStr).ToLocalChecked();
    Nan::MaybeLocal<v8::Value> result = NanJSON.Parse(json_string);
    
    v8::Local<v8::Value> val;

    if (!result.IsEmpty()) { 
        val = result.ToLocalChecked();
    }
    info.GetReturnValue().Set(val);

    //auto output = Nan::New(gameMessageStr).ToLocalChecked();
    //info.GetReturnValue().Set(output);

}

NAN_METHOD(CreateGame) {
    Nan::HandleScope scope;
    if (info.Length() < 1) {
        Nan::ThrowTypeError("1 arguments expected");
        return;
    }
    
    Nan::Utf8String difficultyLevel(info[0]->ToString());

    if (!(isValidDifficultyLevel(*difficultyLevel))) {
        Nan::ThrowError("Difficulty Level must be between 1 and 10");
        info.GetReturnValue().Set(Nan::Undefined());
        return;
    }

    std::string::size_type sz;
    int diffLevel = std::stoi (*difficultyLevel,&sz);

    game_message_w * gameMessage = new game_message_w();

    /** our library Secret Keeper **/
    SecretKeeper secretKeeper;

    gameMessage = secretKeeper.startGame(diffLevel);

    string gameMessageStr = convertGameMessageToJsonString(gameMessage);
    
    Nan::JSON NanJSON;
    v8::Local<v8::String> json_string = Nan::New(gameMessageStr).ToLocalChecked();
    Nan::MaybeLocal<v8::Value> result = NanJSON.Parse(json_string);
    
    v8::Local<v8::Value> val;

    if (!result.IsEmpty()) { 
        val = result.ToLocalChecked();
    }
    info.GetReturnValue().Set(val);

}


NAN_METHOD(GetGameCurrentInfo) {
    Nan::HandleScope scope;
    if (info.Length() < 1) {
        Nan::ThrowTypeError("1 arguments expected");
        return;
    }
    
    Nan::Utf8String gameId(info[0]->ToString());

    if (!(isValidGameId(*gameId))) {
        Nan::ThrowError("please enter valid Game Id");
        info.GetReturnValue().Set(Nan::Undefined());
        return;
    }

    std::string::size_type sz;
    int gid = std::stoi (*gameId,&sz);

    game_message_w * gameMessage = new game_message_w();

    /** our library Secret Keeper **/
    SecretKeeper secretKeeper;

    gameMessage = secretKeeper.getGameInfo(gid);

    string gameMessageStr = convertGameMessageToJsonString(gameMessage);
    
    Nan::JSON NanJSON;
    v8::Local<v8::String> json_string = Nan::New(gameMessageStr).ToLocalChecked();
    Nan::MaybeLocal<v8::Value> result = NanJSON.Parse(json_string);
    
    v8::Local<v8::Value> val;

    if (!result.IsEmpty()) { 
        val = result.ToLocalChecked();
    }
    info.GetReturnValue().Set(val);

}

NAN_METHOD(GuessGame) {
    Nan::HandleScope scope;
    if (info.Length() < 2) {
        Nan::ThrowTypeError("2 arguments expected");
        return;
    }
    
    Nan::Utf8String gameId(info[0]->ToString());
    Nan::Utf8String guess(info[1]->ToString());

    if (!(isValidGameId(*gameId))) {
        Nan::ThrowError("please enter valid Game Id");
        info.GetReturnValue().Set(Nan::Undefined());
        return;
    }

    if (!(isValidGuess(*guess))) {
        Nan::ThrowError("please enter valid guess: a - z ");
        info.GetReturnValue().Set(Nan::Undefined());
        return;
    }

    std::string::size_type sz;
    int gid = std::stoi (*gameId,&sz);
    
    /* need to pass a non const char to guessGame */
    string guessStr = *guess;
    char *cstr = new char[guessStr.length() + 1];
    strcpy(cstr, guessStr.c_str());

    game_message_w * gameMessage = new game_message_w();

    /** our library Secret Keeper **/
    SecretKeeper secretKeeper;

    gameMessage = secretKeeper.guessGame(gid, *cstr);

    delete [] cstr;

    string gameMessageStr = convertGameMessageToJsonString(gameMessage);
    
    Nan::JSON NanJSON;
    v8::Local<v8::String> json_string = Nan::New(gameMessageStr).ToLocalChecked();
    Nan::MaybeLocal<v8::Value> result = NanJSON.Parse(json_string);
    
    v8::Local<v8::Value> val;

    if (!result.IsEmpty()) { 
        val = result.ToLocalChecked();
    }
    info.GetReturnValue().Set(val);

}

NAN_MODULE_INIT(init) {
    Nan::Export(target, "helloWorld", HelloWorld);
    Nan::Export(target, "createGame", CreateGame);
    Nan::Export(target, "getGameCurrentInfo", GetGameCurrentInfo);
    Nan::Export(target, "guessGame", GuessGame);

};

NODE_MODULE(secret_keeper_lib, init);
