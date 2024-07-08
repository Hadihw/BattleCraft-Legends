#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include <iostream>
#include <string>
#include <map>
#include <vector>

#define SIZE1 5 //Size of the tournament mode's maps array
#define SIZE2 4 //Size of the tournament mode's computerStrats array

#include "Player.h"
#include "Map.h"
#include "CommandProcessing.h"
#include "Continent.h"
#include "LoggingObserver.h"
#include "Cards.h"

using std::vector;
using std::string;

class CommandProcessor;
class Player;
class Map;

enum class modeTypes : char 
{
    MODE = 0, 
    TOURNAMENTMODE
};

class Mode
{   
    public:
        virtual modeTypes getType() const = 0;

        virtual ~Mode(){};
    ;

    private:
        modeTypes* type;
    ;
};

class TournamentMode : public Mode
{
    public:
    //Constructor/Destructor
    TournamentMode(string* maps, string* computerPlayers, int* numberOfGames, int* numberOfTurns);

    ~TournamentMode();

    //Getters
    modeTypes getType() const;
    const string* getComputerPlayers() const;
    int getNumberOfGames() const;
    int getNumberOfTurns() const;
    const string* getMaps() const;
    int getMapCounter() const;
    int getGameCounter() const;
    int getTurnCounter() const;
    const int getNumberOfMaps() const;
    const int getNumberOfPlayers() const;
    string** getWinners() const;

    //Setter
    void setWinner(int mapIndex, int gameIndex, string winner);


    //Miscellaneous methods
    void resetgameCounter();
    void resetTurnCounter();

    void incrementMapCounter();
    void incrementGameCounter();
    void incrementTurnCounter();

    private:
        //Attributes
        int* mapCounter;
        int* gameCounter;
        int* turnCounter;
        const int* numberOfMaps;
        const int* numberOfPlayers;
        const string* const maps;
        const string* const computerPlayers;
        string** const winners =  new string*[SIZE1];
        const int* const numberOfGames;
        const int* const numberOfTurns;
        const modeTypes* const type = new modeTypes(modeTypes::TOURNAMENTMODE);
    ;
};

class GameEngine : public Subject, public ILoggable
{
public:
    // Enumeration representing different game states
    enum class GameState : char
    {
        //Insert a new tournament GameState
        START,
        MAP_LOADED,
        MAP_VALIDATED,
        PLAYERS_ADDED,
        ASSIGN_REINFORCEMENT,
        ISSUE_ORDERS,
        EXECUTE_ORDERS,
        WIN
    };
    // Constructor for initializing the game engine
    GameEngine();
    GameEngine(GameState, vector<Player *> *playerList, Map *map, CommandProcessor *comProc, Deck *deck);
    GameEngine(const GameEngine &);
    GameEngine &operator=(const GameEngine &);
    // Function to set the game state
    static void setGameState(GameState newState); // same as transitionToNextState but with enum type? shouldnt this be private?
    // Function to get the current game state
    static GameState getGameState();
    static vector<Player *> *getPlayerList();
    static void setPlayerList(vector<Player *> *playerList);
    static Player *getPlayerByName(string name);
    static Deck *getDeck();
    static void setDeck(Deck *deck);
    static Map *getMap();
    static void setMap(Map *map);
    static CommandProcessor *getComProc();
    static void setComProc(CommandProcessor *comProc);
    // Function to execute a state change based on a command
    void executeStateChange(const std::string &command); 
    // Function to convert the game state to a string for display
    string stateToString() const;
    virtual string stringToLog() const override;
    Player *mainGameLoop();
    void startupPhase();
    static Player *nPlayer;
    void gameLoop();

private:
    std::map<std::string, GameState> *transitions = new std::map<std::string, GameState>{
        {"play", GameState::START},
        {"loadmap", GameState::MAP_LOADED},
        {"validatemap", GameState::MAP_VALIDATED},
        {"addplayer", GameState::PLAYERS_ADDED},
        {"assigncountries", GameState::ASSIGN_REINFORCEMENT},
        {"issueorder", GameState::ISSUE_ORDERS},
        {"endissueorders", GameState::EXECUTE_ORDERS},
        {"exeorder", GameState::EXECUTE_ORDERS},
        {"endexeorders", GameState::ASSIGN_REINFORCEMENT},
        {"win", GameState::WIN}
    };
    // The current game state
    Mode* mode; // variable that stores mode of game
    static Deck *deck;
    static GameState *state;
    static std::vector<Player *> *playerList; // To initialize look at the example https://stackoverflow.com/questions/671423/c-stl-vectors-get-iterator-from-index
    // and use that info to initialize in this manner setTerritoryList(new vector(Map->territories->begin()+i,Map->territories()+i+j))
    static Map *map;
    static CommandProcessor *comProc;
    // Function to check if a given command leads to a valid state transition
    bool isValidTransition(const std::string &command);
    // Function to transition to the next state based on a command
    void transitionToNextState(const std::string &command);
    Player *mostOrders();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    void removePlayerNotControllingATerritory();
    Player *checkWinner();
};

#endif