import React, { Component } from 'react';
import './Game.css';
import {showMessageToast} from '../ui.js'


const initialState = {
    /* the Secret Word is not given to us from API 
    ** we only know which letter and the positions for it when 
    ** it is guessed correctly and all letter guesses
    **/
    secretWord: ['a', '#', '#'],
    pokemonChallenger: '/img/pokey_level_3.png',
    gameId: 1,
    bGameFinished: false,
    bWon: true,
    difficultyLevel: 3,
    numGuessRemaining: 6,
    secretLength: 3,
    lastGuess: 'a',
    bLastGuessMatched: false,
    positions: [ 1, 2, 4 ],
    lettersGuessed: [ 'a' ]     
};

class Game extends Component {
  constructor(props) {
    super(props);

    this.state = initialState;
    this.state.pokemonChallenger = '/img/pokey_level_' + this.props.difficultyLevel + '.png';

    this.processGameMessageJson =  this.processGameMessageJson.bind(this);
    this.guessGame = this.guessGame.bind(this);
    
    /*** create a new Game and populate state */
    fetch("/games", {
      method: 'POST',
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({ 
          "difficultyLevel": this.props.difficultyLevel
      })
    })
    .then((res) => {
      if(res.ok) {
        //console.log('create game from UI', res.json())
        return res.json();
      } else {
        console.log('ack failed', res.statusText)
      }
    })
    .then((gameMessageJson) => {
     
      console.log(JSON.stringify(gameMessageJson));

      this.processGameMessageJson(gameMessageJson, true);

    })
    .catch((ex) => {
      console.log('Your Pokemon feinted!',ex)
      showMessageToast("Someething went wrong and Your Pokemon feinted!! refresh page! ");
      //throw new Error('fetch failed' + ex)
    });
    
  }

  processGameMessageJson = (gameMessageJson, gameStarted) => {

      this.setState( { gameId : gameMessageJson.gameId } );
      this.setState( { difficultyLevel : gameMessageJson.difficultyLevel } );
      this.setState( { bGameFinished : gameMessageJson.bGameFinished } );
      this.setState( { bWon : gameMessageJson.bWon } );
      this.setState( { secretLength : gameMessageJson.secretLength } );
      this.setState( { numGuessRemaining : gameMessageJson.numGuessRemaining } );
      this.setState( { lastGuess : gameMessageJson.lastGuess } );
      this.setState( { bLastGuessMatched : gameMessageJson.bLastGuessMatched } );
      this.setState( { positions : gameMessageJson.positions } );
      this.setState( { lettersGuessed : gameMessageJson.lettersGuessed } );

      var letters = [];

      if (gameStarted) 
      {
          for (var i = 0; i < this.state.secretLength ; i++)
          {
            letters.push("#");
          }

          this.setState( { secretWord : letters } );
      }

      if (this.state.bLastGuessMatched && !gameStarted)
      {
          letters = this.state.secretWord;
          for (var i = 0; i < this.state.positions.length ; i++)
          {
            letters[this.state.positions[i]] = this.state.lastGuess;
          }

          this.setState( { secretWord : letters } );
      }


      if (this.state.bGameFinished)
      {

        if (this.state.bWon)
          {
            
            showMessageToast("YOU WON!!!!!!!");
            var msg = "SECRET UNLOCKED!: " + this.state.secretWord.toString();
            showMessageToast(msg);

          }
          else
          {
            showMessageToast("YOU LOST!!!!!!!");
            var msg = "SECRET STILL HIDDEN: " + this.state.secretWord.toString();
            showMessageToast(msg);
          }

          this.gameDidFinish();
      }
       
   


  }

  guessGame = (guess, buttonKey) => {
      /*** create a new Game and populate state */
      var uri = '/games/' + this.state.gameId ;
      fetch(uri, {
          method: 'PUT',
          headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({ 
            "guess": guess
          })
      })
      .then((res) => {
          if(res.ok) {
            //console.log('guessed game from UI', res.json())
            return res.json();
          } else {
            console.log('ack failed', res.statusText)
          }
      })
      .then((gameMessageJson) => {
 
          console.log(JSON.stringify(gameMessageJson));
          this.processGameMessageJson(gameMessageJson, false);

      })
      .catch((ex) => {
          console.log('Your Pokemon feinted!',ex)
          showMessageToast("Someething went wrong or Your Pokemon feinted!! Keep guessing!!! ");
         // throw new Error('fetch failed' + ex)
      });

      //showMessageToast(guess);

      // set when guess is verified 
      buttonKey.className = "ui-keyboard-start-active" ;


  }

  gameDidFinish = () => {
 
    var gameSelection = document.getElementById("gameSelection");
    gameSelection.className = "visibile";

    this.props.gameFinished();
  }

  reset() {
    this.state = initialState;
  }
  componentDidMount() {

    var keyboard = document.getElementById("keyboard");

    var buttonKeys = keyboard.getElementsByTagName("button");

    for (var i=0; i < buttonKeys.length; i++) {
        var buttonKey = buttonKeys[i];
        var boundGuessGame = this.guessGame.bind(this, buttonKey.value, buttonKey)
        buttonKey.addEventListener("click", boundGuessGame);

        //change the style
        buttonKey.className = "ui-keyboard-start" ;

    }
//    fetch('/api/customers')
//      .then(res => res.json())
//      .then(customers => this.setState({customers}, () => console.log('Customers fetched...', customers)));
  }

  render() {
    return (
      <div id="pokemon_challenge">
          <div id="pokemon_opponent">
          <table align="center" height="25">
              <tr><td>
                {
                  <div>
                    <img src={this.state.pokemonChallenger}></img>
                    </div>
                }
                </td><td>
                {
                  <div class="pokeyCard shadow rounded">
                  <h6><b>{this.state.numGuessRemaining} tosses left</b></h6></div>
                }
                </td>
                <td>
                  <div><span>Tosses so far!</span></div>
                {
                  this.state.lettersGuessed.map(letterGuessed =>
                  <div class="pokeyCardSmall shadow rounded">
                      <h7><b>{letterGuessed}</b></h7></div>
                  )}
                </td>
                </tr>
              </table>
          </div>
          <div id="secretWord">
            <table align="center">
                <tr><td>
                {
                this.state.secretWord.map(letter =>
                <div class="pokeyCard shadow rounded">
                  <h7><b>{letter}</b></h7></div>
              )}
                </td></tr>
              </table>
          </div>
          <div id="keyboard">
            <table align="center">
                <tr>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="q" name="key_Q" title=""><span>Q</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="w" name="key_W" title=""><span>W</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="e" name="key_E" title=""><span>E</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="r" name="key_R" title=""><span>R</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="t" name="key_T" title=""><span>T</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="y" name="key_Y" title=""><span>Y</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="u" name="key_U" title=""><span>U</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="i" name="key_I" title=""><span>I</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="o" name="key_O" title=""><span>O</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="p" name="key_P" title=""><span>P</span></button>
                    </td>
                </tr>
                <tr>
                    <td>
                      <span></span>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="a" name="key_A" title=""><span>A</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="s" name="key_S" title=""><span>S</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="d" name="key_D" title=""><span>D</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="f" name="key_F" title=""><span>F</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="g" name="key_G" title=""><span>G</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="h" name="key_H" title=""><span>H</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="j" name="key_J" title=""><span>J</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="k" name="key_K" title=""><span>K</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="l" name="key_L" title=""><span>L</span></button>
                    </td>
                </tr>
                <tr>
                    <td>
                      <span></span>
                    </td>
                    <td>
                      <span></span>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="z" name="key_Z" title=""><span>Z</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="x" name="key_X" title=""><span>X</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="c" name="key_C" title=""><span>C</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="v" name="key_V" title=""><span>V</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="b" name="key_B" title=""><span>B</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="n" name="key_N" title=""><span>N</span></button>
                    </td>
                    <td>
                        <button class="ui-keyboard-button ui-corner-all" value="M" name="key_M" title=""><span>M</span></button>
                    </td>
                </tr>
            </table>
          </div>
      </div>
    );
  }
}

export default Game;