import React, { Component } from 'react';
import './App.css';
import Game from './components/Game';
import {showMessageToast} from './ui.js'

 
class App extends Component {
  
  constructor(props) {
    super(props);
    this.state = {
      difficultyLevel: 5,
      showGame: false
    };


  }

  reset() {
//    this.state = initialState;
  }

  startNewGame(diffLevel)
  {

    showMessageToast("Are you ready for level: " + diffLevel + "?");

    var gameSelection = document.getElementById("gameSelection");
    gameSelection.className = "hidden";

    this.setState( { difficultyLevel : diffLevel } );
    const { showGame } = this.state.showGame;

    this.setState( { showGame : true } );
    
    
  }
  
  gameFinished = () => {
    this.setState( { showGame : false } );
  }

  componentDidMount()
  {
    showMessageToast("Would you like to play a game? Pick one of us!!!");
    
    var pokemon = document.getElementById("pokemon");
    var images = pokemon.getElementsByTagName("img");

    for (var i=0; i < images.length; i++) {
        var img = images[i];
        var boundStartNewGame = this.startNewGame.bind(this, img.getAttribute("difficultyLevel"))
        img.addEventListener("click", boundStartNewGame);
    }
  }
  render() {
    
    const diffLevel = this.state.difficultyLevel;
    
    return (
      <div className="App">
         { this.state.showGame && <Game difficultyLevel={diffLevel} gameFinished={this.gameFinished} /> }
         <div id="gameSelection">
          <table id="pokemon" align="center">
            <tr>
                <td>
                  <img src="./img/pokey_level_1.png" alt="1" difficultyLevel="1"></img>
                </td>
                <td>
                  <img src="./img/pokey_level_2.png" alt="2" difficultyLevel="2"></img>
                </td>
                <td>
                  <img src="./img/pokey_level_3.png" alt="3" difficultyLevel="3"></img>
                </td>
                <td>
                  <img src="./img/pokey_level_4.png" alt="4" difficultyLevel="4"></img>
                </td>
                <td>
                  <img src="./img/pokey_level_5.png" alt="5" difficultyLevel="5"></img>
                </td>
            </tr>
            <tr>
                <td>
                  <img src="./img/pokey_level_6.png" alt="6" difficultyLevel="6"></img>
                </td>
                <td>
                  <img src="./img/pokey_level_7.png" alt="7" difficultyLevel="7"></img>
                </td>
                <td>
                  <img src="./img/pokey_level_8.png" alt="8" difficultyLevel="8"></img>
                </td>
                <td>
                  <img src="./img/pokey_level_9.png" alt="9" difficultyLevel="9"></img>
                </td>
                <td>
                  <img src="./img/pokey_level_10.png" alt="10" difficultyLevel="10"></img>
                </td>
            </tr>
          </table>
          </div>
          <div id="messageToast">Some informational  message or error</div>
      </div>
      
    );
  }
}

export default App;
