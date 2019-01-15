/** secret_keeperaddon.h **/

#ifndef SECRET_KEEPER_ADDON_H_
#define SECRET_KEEPER_ADDON_ H_

#include <iostream>
#include <nan.h>
#include <string>

#include "./game_message.h"
#include "./SecretKeeper.h"

using namespace std;

NAN_METHOD(HelloWorld);
NAN_METHOD(CreateGame);
NAN_METHOD(GetGameCurrentInfo);
NAN_METHOD(GuessGame);

#endif /* secret_keeperaddon.h */