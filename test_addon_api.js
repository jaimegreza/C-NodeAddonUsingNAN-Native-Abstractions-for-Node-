const secretKeeper = require('./build/Release/secret_keeper_lib.node');

module.exports = secretKeeper;

console.log(secretKeeper.helloWorld());

var json = JSON.stringify(secretKeeper.helloWorld());

console.log(json);

var game_1_Message = secretKeeper.createGame(3);

console.log(JSON.stringify(game_1_Message));

var game_2_Message = secretKeeper.createGame(3);

console.log(JSON.stringify(game_2_Message));

var game_1_MessageInfo = secretKeeper.getGameCurrentInfo(game_1_Message.gameId);

console.log(JSON.stringify(game_1_MessageInfo));


var game_1_MessageGuess = secretKeeper.guessGame(1, 'a');
console.log(JSON.stringify(game_1_MessageGuess));

game_1_MessageGuess = secretKeeper.guessGame(1, 'b');
console.log(JSON.stringify(game_1_MessageGuess));
