#include "GameEngine.h"
#include "MapLoader.h"
#include "CommandProcessing.h"
#include <filesystem>
#include <sstream>
#include <random>
#include <ctime>
#include <algorithm>

Player *GameEngine::nPlayer = nullptr;
Deck *GameEngine::deck = nullptr;
GameEngine::GameState *GameEngine::state = nullptr;
vector<Player *> *GameEngine::playerList = nullptr;
Map *GameEngine::map = nullptr;
CommandProcessor *GameEngine::comProc = nullptr;

GameEngine::GameEngine()
{
    state = new GameState(GameState::START);
    playerList = new vector<Player *>();
    comProc = new CommandProcessor();
    deck = new Deck(100);
    mode = nullptr;
}

GameEngine::GameEngine(GameState state, vector<Player *> *playerList, Map *map, CommandProcessor *comProc, Deck *deck)
{
    GameEngine::state = new GameState(state);
    this->playerList = playerList;
    GameEngine::map = map;
    GameEngine::comProc = comProc;
    GameEngine::deck = deck;
}
GameEngine::GameEngine(const GameEngine &engine)
{
    this->state = new GameState(*engine.state);
    this->playerList = new vector<Player *>(*engine.playerList);
    this->map = new Map(*map);
}
GameEngine &GameEngine::operator=(const GameEngine &engine)
{
    *this->state = *engine.state;
    delete this->playerList;
    this->playerList = new vector<Player *>(*engine.playerList);
    *this->map = *engine.map;
    return *this;
}
Deck *GameEngine::getDeck()
{
    return deck;
}
void GameEngine::setDeck(Deck *deck)
{
    if (deck != nullptr)
        delete deck;
    GameEngine::deck = deck;
}
// Add a function to set the game state
void GameEngine::setGameState(GameState newState)
{
    *state = newState;
}

// Add a function to get the current game state
GameEngine::GameState GameEngine::getGameState()
{
    return *state;
}
vector<Player *> *GameEngine::getPlayerList()
{
    return playerList;
}
void GameEngine::setPlayerList(vector<Player *> *list)
{
    playerList = list;
}
Player *GameEngine::getPlayerByName(string name)
{
    for (auto player : *playerList)
    {
        if (player->getName() == name)
            return player;
    }
    return nullptr;
}
Map *GameEngine::getMap()
{
    return map;
}
void GameEngine::setMap(Map *map)
{
    map = map;
}
CommandProcessor *GameEngine::getComProc()
{
    return comProc;
}
void GameEngine::setComProc(CommandProcessor *comProc)
{
    comProc = comProc;
}
// Add a function to execute a command and transition to the next state
void GameEngine::executeStateChange(const std::string &command)
{
    if (isValidTransition(command))
    {
        transitionToNextState(command);
        std::cout << "You are now in the state: " << stateToString() << "\n";
    }
    else
    {
        std::cout << "Error: Invalid command\n";
    }
}

// Add a function to convert game state to a string
std::string GameEngine::stateToString() const
{
    switch (*state)
    {
    case GameState::START:
        return "START";
    case GameState::MAP_LOADED:
        return "MAP_LOADED";
    case GameState::MAP_VALIDATED:
        return "MAP_VALIDATED";
    case GameState::PLAYERS_ADDED:
        return "PLAYERS_ADDED";
    case GameState::ASSIGN_REINFORCEMENT:
        return "ASSIGN_REINFORCEMENT";
    case GameState::ISSUE_ORDERS:
        return "ISSUE_ORDERS";
    case GameState::EXECUTE_ORDERS:
        return "EXECUTE_ORDERS";
    case GameState::WIN:
        return "WIN";
    }
    return "Error reading state.\n";
}

bool GameEngine::isValidTransition(const std::string &command)
{
    switch (*state)
    {
    case GameState::START:

        if (command == "loadmap")
            return true;
    case GameState::MAP_LOADED:
        if (command == "loadmap" || command == "validatemap")
            return true;
    case GameState::MAP_VALIDATED:
        if (command == "addplayer")
            return true;
    case GameState::PLAYERS_ADDED:
        if (command == "addplayer" || command == "assigncountries")
            return true;
    case GameState::ASSIGN_REINFORCEMENT:
        if (command == "issueorder")
            return true;
    case GameState::ISSUE_ORDERS:
        if (command == "issueorder" || command == "issueorders")
            return true;
    case GameState::EXECUTE_ORDERS:
        if (command == "exeorder" || command == "endexeorders" || command == "win")
            return true;
    case GameState::WIN:
        if (command == "end" || command == "play")
            return true;
    default:
        return false;
    }
}
void GameEngine::startupPhase()
{
    while (*(GameEngine::state) != GameState::ASSIGN_REINFORCEMENT)
    {
        switch (*(GameEngine::state))
        {
        case GameState::START:
        {
            if(this->mode == nullptr)
            {
                // Prompt the player to select a map
                // Establish a connection to the MapFiles folder
                std::filesystem::path path("./MapFiles");
                if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
                {
                    cout << "MapFiles folder could not be found" << endl;
                    cout << "Exiting game..." << endl;
                    exit(1);
                    break;
                }
                // Checking if any map files exist

                bool mapsExist = false;
                for (const auto &file : std::filesystem::directory_iterator{path})
                {
                    if (file.path().extension() == ".map")
                    {
                        mapsExist = true;
                    }
                }

                if (!mapsExist)
                {
                    cout << "MapFiles folder does not contain any .map files" << endl;
                    cout << "Exiting game..." << endl;
                    exit(1);
                    break;
                }

                // Clear the system

                // Prompt the user to enter a map
                cout << "Please select a map to load" << endl;

                for (const auto &file : std::filesystem::directory_iterator{path})
                {
                    if (file.path().extension() == ".map")
                    {
                        cout << "-" << file.path().stem() << endl;
                    }
                }

                cout << "//////////////////////////////////////////////" << endl;

                Command *cmd = this->comProc->getCommand();

                while (!cmd->isValid())
                {
                    cout << (cmd->getCommandEffect());
                    cmd = this->comProc->getCommand();
                }

                // Extract and check type of command
                std::stringstream ss;
                std::string holder;
                ss << (cmd->getCommandEffect());

                getline(ss, holder, ' '); //First string of the command

                if(holder == "loadmap")
                {
                    getline(ss, holder);

                    std::ifstream file("./MapFiles/" + holder + ".map");

                    MapLoader *loader = new MapLoader(holder);
                    loader->addContinents(file);
                    loader->addTerritories(file);
                    loader->getMap()->printMap();
                    GameEngine::map = loader->getMap();

                    delete loader;

                    setGameState(GameState::MAP_LOADED);

                    cout << "\nMap loaded!";
                }
                else if (holder == "tournament")
                {
                    this->mode = cmd->getMode();

                    TournamentMode* tm = dynamic_cast<TournamentMode*>(this->mode);

                    const string* maps = tm->getMaps();

                    const string* computerPlayers = tm->getComputerPlayers();

                    cout << "\nRunning tournament mode with\n Maps: ";

                    for(int i = 0; i < SIZE1; i++)
                    {
                        if(maps[i] != "")
                        {
                            cout << maps[i] << " ";
                        }
                    }

                    cout << "\nComputer player strategies: ";

                    for(int i = 0; i < SIZE2; i++)
                    {
                        if(computerPlayers[i] != "")
                        {
                            cout << computerPlayers[i] << " ";
                        }
                    }

                    cout << "\nNumber of games to be played on each map: " << tm->getNumberOfGames() << endl;
                    cout << "Number of turns until automatic draw: " << tm->getNumberOfTurns() << endl;

                    setGameState(GameState::START);
                }
            }
            else if(this->mode->getType() == modeTypes::TOURNAMENTMODE)
            {
                TournamentMode* tm = dynamic_cast<TournamentMode*>(this->mode);
                
                //Load the next map for the tournament mode (or the same map if map counter did not change)
                string newMap = tm->getMaps()[tm->getMapCounter()];

                std::ifstream file("./MapFiles/" + newMap + ".map");

                MapLoader *loader = new MapLoader(newMap);
                loader->addContinents(file);
                loader->addTerritories(file);
                GameEngine::map = loader->getMap();

                cout << "\nMap " + newMap + " loaded!\n" << endl;

                delete loader;

                //Change GameState to MAP_LOADED
                setGameState(GameState::MAP_LOADED);
            }
        }
        break;
        case GameState::MAP_LOADED:
        {
            if(this->mode == nullptr)
            {
                // Prompt the player to select a map
                // Establish a connection to the MapFiles folder
                std::filesystem::path path("./MapFiles");
                if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
                {
                    cout << "MapFiles folder could not be found" << endl;
                    cout << "Exiting game..." << endl;
                    exit(1);
                }
                // Checking if any map files exist

                bool mapsExist = false;
                for (const auto &file : std::filesystem::directory_iterator{path})
                {
                    if (file.path().extension() == ".map")
                    {
                        mapsExist = true;
                    }
                }

                if (!mapsExist)
                {
                    cout << "MapFiles folder does not contain any .map files" << endl;
                    cout << "Exiting game..." << endl;
                    exit(1);
                }

                // Notify the user that they can enter validatemap to validate the map or they can load another map
                cout << "Please continue with validatemap, or select another map to load" << endl;

                // Prompt the user to enter a map
                cout << "//Please select a map to load or validate map/" << endl;

                for (const auto &file : std::filesystem::directory_iterator{path})
                {
                    if (file.path().extension() == ".map")
                    {
                        cout << "-" << file.path().stem() << endl;
                    }
                }

                cout << "//////////////////////////////////////////////" << endl;

                Command *cmd = this->comProc->getCommand();

                while (!cmd->isValid())
                {
                    cout << (cmd->getCommandEffect());
                    cmd = this->comProc->getCommand();
                }
                // Delete the previous line

                // Extract the command
                std::stringstream ss;
                std::string fileName;
                ss << (cmd->getCommandEffect());

                getline(ss, fileName, ' ');

                // Control of flow:
                if (fileName == "loadmap")
                {
                    // Extract the map name
                    getline(ss, fileName);

                    std::ifstream file("./MapFiles/" + fileName + ".map");

                    MapLoader loader(fileName);
                    loader.addContinents(file);
                    loader.addTerritories(file);

                    GameEngine::map = loader.getMap();

                    setGameState(GameState::MAP_LOADED);

                    cout << "\nMap loaded!";
                }
                else
                {
                    // Validate map
                    if (!GameEngine::map->validate())
                    {
                        cout << "Invalid map file. Please select another map file.";
                        break;
                    }
                    else
                    {
                        setGameState(GameState::MAP_VALIDATED);
                        cout << "Map validated!" << endl;
                    }
                }
            }
            else if (this->mode->getType() == modeTypes::TOURNAMENTMODE)
            {
                //Validate the map
                if (!GameEngine::map->validate())
                {
                    cout << "Exiting tournament mode..." << endl;
                    delete this->mode;
                    this->mode = nullptr;

                    //Set game state back to start
                    setGameState(GameState::START);

                    cout << "Going back to main menu..." << endl;
                }
                else
                {
                    setGameState(GameState::MAP_VALIDATED);
                    cout << "Map validated!" << endl;
                }
            }
        }
        break;
        case GameState::MAP_VALIDATED:
        {
            if(this->mode == nullptr)
            {
                // Notify the user that they now have to add a player to the game
                cout << "Please add a player to the game by entering the command \"addplayer\" followed by a space and the player's name" << endl;

                // Prompt the user to enter a player
                cout << "//Please add a player to the game//////////////" << endl;
                // Prompt the user to add a player to the game

                cout << "///////////////////////////////////////////////" << endl;

                Command *cmd = this->comProc->getCommand();

                while (!cmd->isValid())
                {

                    cout << (cmd->getCommandEffect());
                    cmd = this->comProc->getCommand();
                }

                // Extract the player name
                std::stringstream ss;
                std::string playerStrategy;
                std::string playerName;
                ss << (cmd->getCommandEffect());
                getline(ss, playerStrategy, ' ');
                getline(ss, playerStrategy, ' ');
                getline(ss, playerName, ' ');

                Player *p = new Player();
                p->setPlayerId(1);
                p->setName(new string(playerName));
                if (p->setPlayerStrategy(playerStrategy))
                {
                    GameEngine::playerList->push_back(p);
                    GameEngine::setGameState(GameState::PLAYERS_ADDED);
                    cout << "Player " << playerList->back()->getName() << " added" << endl;
                }
                else
                {
                    cout << "Invalid player Strategy! Player not added.\n";
                    cmd->saveEffect("Invalid player Strategy! Player not added.");
                }
            }
            else if (this->mode->getType() == modeTypes::TOURNAMENTMODE)
            {   
                //Add computer players to the game
                TournamentMode* tm = dynamic_cast<TournamentMode*>(this->mode);
                
                for(int i = 0; i < tm->getNumberOfPlayers(); i++)
                {
                    Player* p = new Player();
                    p->setPlayerId(i);
                    p->setName(new string(tm->getComputerPlayers()[i]));
                    p->setPlayerStrategy(tm->getComputerPlayers()[i]);
                    GameEngine::playerList->push_back(p);
                    cout << "Player " << playerList->back()->getName() << " added" << endl;
                }
                GameEngine::setGameState(GameState::PLAYERS_ADDED);
            }
        }
        break;
        case GameState::PLAYERS_ADDED:
        {
            if(this->mode == nullptr)
            {
                // Notify the user of their options
                if (playerList->size() == 6)
                {
                    cout << "Please enter gamestart to start the game" << endl;
                }
                else
                {
                    cout << "Please continue adding players by entering the command \"addplayer\" followed by a space and the player's name" << endl;
                    cout << "or enter \"gamestart\" to start the game" << endl;
                }

                cout << "///////////////////////////////////////////////" << endl;

                Command *cmd = this->comProc->getCommand();

                while (!cmd->isValid())
                {
                    cout << (cmd->getCommandEffect());
                    cmd = this->comProc->getCommand();
                }
                // Delete the previous line

                // Extract the command
                std::stringstream ss;
                std::string input;
                ss << (cmd->getCommandEffect());

                getline(ss, input, ' ');

                // Flow of control
                if (input == "addplayer")
                {
                    if (playerList->size() == 6)
                    {
                        cout << "Cannot add any more players to the game" << endl;
                        GameEngine::setGameState(GameState::PLAYERS_ADDED);
                    }
                    else
                    {
                        // get player name
                        getline(ss, input);

                        playerList->push_back(new Player());
                        playerList->back()->setName(new string(input));
                        GameEngine::setGameState(GameState::PLAYERS_ADDED);

                        cout << "Player " << GameEngine::playerList->back()->getName() << " added" << endl;
                    }
                }
                else
                {
                    GameEngine::setGameState(GameState::ASSIGN_REINFORCEMENT);
                    cout << "\nStarting game..." << endl;
                }
            }
            else if(this->mode->getType() == modeTypes::TOURNAMENTMODE)
            {
                GameEngine::setGameState(GameState::ASSIGN_REINFORCEMENT);
                cout << "\nStarting game..." << endl;
            }
        }
        break;
        }
    }

    // Distribute all territories to the players
    int t = 0;
    int tsz = map->territories->size();
    int psz = playerList->size();
    int num = tsz / psz;
    for (auto player : *playerList)
    {
        player->addToPoolOfArmies(50 / psz);
        for (int j = 0; j < num; j++)
        {
            player->getTerritoryList().push_back(map->territories->at(t));
            map->territories->at(t)->setPlayer(player);
            ++t;
        }
    }
    while (t < tsz)
    {
        playerList->at(rand() % psz)->getTerritoryList().push_back(map->territories->at(t));
        ++t;
    }

    // Switch to the play phase (return from function)
}

void GameEngine::transitionToNextState(const std::string &command)
{
    // Implement logic to transition to the next state based on the command
    // Example:
    setGameState(transitions->find(command)->second);
    notify();
}

void GameEngine::reinforcementPhase()
{
    vector<pair<Player *, Continent *>> cplayers;
    for (auto iter = this->map->continents->begin(); iter != this->map->continents->end(); iter++)
    {
        bool owns = false;
        Player *player = &(*iter)->territories->at(0)->getPlayer();
        for (auto titer = (*iter)->territories->begin(); titer != (*iter)->territories->end(); titer++)
            if (!(player->getName() == (*titer)->getPlayer().getName()))
            {
                owns = false;
                break;
            }
            else
                owns = true;
        if (owns)
            cplayers.push_back(pair<Player *, Continent *>(player, *iter));
    }
    for (auto piter = this->playerList->begin(); piter != this->playerList->end(); piter++)
    {
        int armies = (*piter)->getTerritoryList().size() / 3;
        if (armies < 3)
            armies = 3;
        int armiesPool = (*piter)->getArmiesPool();
        for (auto piter2 = cplayers.begin(); piter2 != cplayers.end(); piter2++)
        {
            if (piter2->first->getName() == (*piter)->getName())
                armies += piter2->second->getBonus();
        }
        (*piter)->addToPoolOfArmies(armies);
    }
}

void GameEngine::issueOrdersPhase()
{
    cout << "\nOrder Issuing Phase started\n\n";
    for (auto player : *this->playerList)
    {
        cout << player->getName() << " turn to enter orders\n\n";
        player->issueOrder();
    }
}
Player *GameEngine::mostOrders()
{
    int highest = 0;
    Player *p = nullptr;
    for (auto player : *playerList)
    {
        int size = player->getOrdersList()->Size();
        if (size > highest)
        {
            highest = size;
            p = player;
        }
    }
    return p;
}
void GameEngine::executeOrdersPhase()
{
    childrenTypes orderState = childrenTypes::DeployOrder;
    Player *numOrders = mostOrders();
    while (numOrders->getOrdersList()->Size() > 0)
    {
        int num = 0;
        for (auto player : *playerList)
        {
            if (player->getOrdersList()->Size() > 0)
            {
                Order *ord = player->getOrdersList()->getOrder(1);
                if (ord->getType() == orderState)
                {
                    ord->execute();
                    player->getOrdersList()->removeOrder(1);
                }
                else
                {
                    num++;
                }
            }
            else
            {
                num++;
            }
        }
        if (num == playerList->size())
            ++orderState;
    }
    cout << "Execute orders phase completed.\n";
}
void GameEngine::removePlayerNotControllingATerritory()
{
    for (int i = 0; i < playerList->size(); i++)
    {
        if (playerList->at(i)->getTerritoryList().size() == 0)
        {
            Player *temp = playerList->at(i);
            playerList->erase(playerList->begin() + i);
            cout << temp->getName() << " removed from the game.\n";
            temp->~Player();
        }
    }
}
Player *GameEngine::checkWinner()
{
    for (auto player : *playerList)
        if (player->getTerritoryList().size() == map->territories->size())
            return player;
    return nullptr;
}

//TournamentMode methods

TournamentMode::TournamentMode(string* maps, string* computerPlayers, int* numberOfGames, int* numberOfTurns) : maps(maps), computerPlayers(computerPlayers), numberOfGames(numberOfGames), numberOfTurns(numberOfTurns)
{
   this->mapCounter = new int(0);
   this->gameCounter = new int(1);
   this->turnCounter = new int(1);

   //Compute the number of maps to be loaded
   int temp = 0;
   for(int i = 0; i < SIZE1; i++)
   {
        if(maps[i] != "")
        {
            temp++;
        }
   }

   this->numberOfMaps = new int (temp);

   //Computer the number of players to be loaded
   temp = 0;

   for(int i = 0; i < SIZE2; i++)
   {
        if(computerPlayers[i] != "")
        {
            temp++;
        }
   }

   this->numberOfPlayers = new int (temp);

}

// Destructor
TournamentMode::~TournamentMode()
{
    delete mapCounter;
    delete gameCounter;
    delete[] maps;
    for(int i = 0; i < SIZE1; i++)
    {
        delete[] winners[i];
    }
    delete[] winners;
    delete numberOfGames;
    delete numberOfTurns;
    delete type;
}

//Getters

modeTypes TournamentMode::getType() const
{
    return *(this->type);
}

const string* TournamentMode::getComputerPlayers() const 
{
    return computerPlayers;
}

int TournamentMode::getNumberOfTurns() const 
{
    return *numberOfTurns;
}

int TournamentMode::getNumberOfGames() const 
{
    return *numberOfGames;
}

const int TournamentMode::getNumberOfMaps() const
{
    return *this->numberOfMaps;
}

const int TournamentMode::getNumberOfPlayers() const
{
    return *this->numberOfPlayers;
}

const string* TournamentMode::getMaps() const 
{
    return maps;
}

int TournamentMode::getMapCounter() const 
{
    return *mapCounter;
}

int TournamentMode::getGameCounter() const 
{
    return *gameCounter;
}

int TournamentMode::getTurnCounter() const
{
    return *turnCounter;
}

string** TournamentMode::getWinners() const
{
    return this->winners;
}

//Setter
void TournamentMode::setWinner(int mapIndex, int gameIndex, string winner) 
{
    winners[mapIndex][gameIndex] = winner;
}

//Miscellaneous methods

void TournamentMode::resetgameCounter() 
{
    *gameCounter = 1;
}

void TournamentMode::resetTurnCounter()
{
    *turnCounter = 1;
}

void TournamentMode::incrementMapCounter() 
{
    (*mapCounter)++;
}

void TournamentMode::incrementGameCounter() 
{
    (*gameCounter)++;
}

void TournamentMode::incrementTurnCounter()
{
    *(turnCounter)++;
}
Player *GameEngine::mainGameLoop()
{
    Player *winner = nullptr;
    if(this->mode == nullptr)
    {
        while(winner == nullptr)
        {
            cout << "Assigning reinforcements to players.\n";
            reinforcementPhase();
            cout << "Reinforcement phase completed.\n";
            transitionToNextState("issueorder");
            issueOrdersPhase();
            transitionToNextState("exeorders");
            for (auto player : *playerList)
            {
                player->setHasCapturedATerritory(false);
            }
            executeOrdersPhase();
            transitionToNextState("endexeorders"); 
            removePlayerNotControllingATerritory();
            winner = checkWinner();
            for (auto player : *playerList)
            {
                player->setArmiesPool(0);
            }
            transitionToNextState("assigncountries");
        }
        cout << winner->getName() << " won the game!\n";
        transitionToNextState("win");
    }
    else if(this->mode->getType() == modeTypes::TOURNAMENTMODE)
    {
        TournamentMode* tm = dynamic_cast<TournamentMode*>(this->mode);
        while(winner == nullptr)
        {
            cout << "Assigning reinforcements to players.\n";
            reinforcementPhase();
            cout << "Reinforcement phase completed.\n";
            transitionToNextState("issueorder");
            issueOrdersPhase();
            transitionToNextState("exeorders");
            for (auto player : *playerList)
            {
                player->setHasCapturedATerritory(false);
            }
            executeOrdersPhase();
            transitionToNextState("endexeorders"); 
            removePlayerNotControllingATerritory();
            winner = checkWinner();
            for (auto player : *playerList)
            {
                player->setArmiesPool(0);
            }
            transitionToNextState("assigncountries");

            //Check if the game is a draw
            if(tm->getTurnCounter() == tm->getNumberOfTurns())
            {
                winner = new Player();
                winner->setName(new string("Draw"));
            }

            //Increment turn counter
            tm->incrementTurnCounter();
        }
    }
    
    return winner;
}
string GameEngine::stringToLog() const
{
    return "Game state transitioned to " + this->stateToString();
}
void GameEngine::gameLoop()
{
    while (true)
    {
        //Enter startup phase
        this->startupPhase();
        //Check if mode changed after startup phase
        if(this->mode == nullptr) //Normal game mode
        {
            this->mainGameLoop();
            
            Command *re = comProc->getCommand();
            if (re->getCommandString() == "play")
            {
                cout << "Restarting the game...\n";
                //Reset GameEngine attributes
                delete GameEngine::map;
                GameEngine::map = nullptr;
                GameEngine::playerList->clear();
                delete GameEngine::deck;
                GameEngine::deck = new Deck(100);
                setGameState(GameState::START);
            }
            else
            {
                cout << "Exiting the game...\n";
                //Delete GameEngine attributes
                delete GameEngine::map;
                GameEngine::playerList->clear();
                delete GameEngine::playerList;
                delete GameEngine::deck;
                exit(0);
            }
        }
        else if(this->mode->getType() == modeTypes::TOURNAMENTMODE)
        {
            TournamentMode* tm = dynamic_cast<TournamentMode*>(this->mode);
            Player *winner = this->mainGameLoop();
            
            tm->setWinner(tm->getMapCounter(),tm->getGameCounter(),winner->getName());

            //Delete dummy player if game was a draw
            if(winner->getName() == "Draw")
            {
                delete winner;
            }

            //Check if tournament mode has ended
            if(tm->getGameCounter() == tm->getNumberOfGames() && tm->getMapCounter() == tm->getNumberOfMaps() - 1)
            {
                //Print out the results of the tournament mode
                cout << "Results:" << endl;

                string**winners = tm->getWinners();

                const string* maps = tm->getMaps();

                for(int i = 0; i < tm->getNumberOfMaps(); i++)
                {
                    cout << maps[i] << " ";

                    for(int j = 0; j < tm->getNumberOfGames(); j++)
                    {
                        cout << winners[i][j] << " ";
                    }

                    cout << endl;
                }
                //Terminate the mode
                delete this->mode;
                this->mode = nullptr;
            }
            //Otherwise update the counters
            else
            {
                //Check if map counter needs to be updated
                if(tm->getGameCounter() == tm->getNumberOfGames())
                {
                    tm->incrementMapCounter();
                    tm->resetgameCounter();
                    tm->resetTurnCounter();
                }
                else
                {
                    //Update game counter & reset turn counter
                    tm->incrementGameCounter();
                    tm->resetTurnCounter();
                }
            }

            //Reset all GameEngine attributes
            delete GameEngine::map;
            GameEngine::map = nullptr;
            GameEngine::playerList->clear();
            delete GameEngine::deck;
            GameEngine::deck = new Deck(100);

            //Set game state
            setGameState(GameState::START);
        }
    }
}