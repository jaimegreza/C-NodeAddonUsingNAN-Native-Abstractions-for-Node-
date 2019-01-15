/* secret keeper library */
const secretKeeper = require('./build/Release/secret_keeper_lib.node');
var express = require('express')

module.exports = secretKeeper;

var app = express()

var bodyParser = require('body-parser');

// configure app to use bodyParser() to get POST data
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// ROUTES FOR OUR REST API
var router = express.Router();   // get  express Router

router.use(function(req, res, next) {
    // log requests
    console.log(req.url);
    next(); // go to next routes
});

router.get('/', function(req, res) {
    res.json(secretKeeper.helloWorld())   
});

// REST API routes

router.route('/games')

// start a game via POST http://localhost:8080/games)
.post(function(req, res) {

    console.log("request body: " + req.body);

    // header: application/x-www-form-urlencoded
    // body : { difficultyLevel: '3' }
    var difficultyLevel = req.body.difficultyLevel;  // get the difficulty level
    console.log(difficultyLevel);
    var gameMessage = secretKeeper.createGame(difficultyLevel);  
    res.json(gameMessage);
});

// get game info

router.route('/games/:gameId')

// get current game info for gameId (accessed at GET http://localhost:8080/games/:gameId)
.get(function(req, res) {

    var gameId = req.params.gameId;
    var gameMessage = secretKeeper.getGameCurrentInfo(gameId);  
    res.json(gameMessage);

})

// guess a letter for this game id (accessed at PUT http://localhost:8080/games/:gameId)
.put(function(req, res) {
 
    var gameId = req.params.gameId;
    var guess = req.body.guess;

    console.log(guess);
    var gameMessage = secretKeeper.guessGame(gameId,guess);  
    res.json(gameMessage);

});

// REGISTER ROUTES 
app.use('/', router);

var port = 8080; 
app.listen(port)

console.log('Firing up engines on port ' + port);