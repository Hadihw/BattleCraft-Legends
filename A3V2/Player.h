#ifndef PLAYER_H
#define PLAYER_H

#include "CommandProcessing.h"
#include "Orders.h"
#include "Cards.h"
#include "Territories.h"
#include "Map.h"
#include "PlayerStrategies.h"

#include <string>

class PlayerStrategy;
class OrdersList;

using std::vector;
using std::string;

class Command;

class Player
{

public:
    Player();                                                                                                                                                                                                                           // default constructor
    Player(string name, int playerId, vector<Territories *> *t, Hand *h, OrdersList *o, int *armiesPool, Player *playerInTreaty, int *turnsUntilEndOfTreaty, bool *hasCapturedATerritory, bool *gotAttacked, PlayerStrategy *strategy); // constructor
    Player(const Player &player);                                                                                                                                                                                                       // copy constructor
    ~Player();                                                                                                                                                                                                                          // destructor
    virtual vector<Territories *> &getTerritoryList();
    void setTerritoryList(vector<Territories *> *territorylist);
    string getName();
    void setName(string *name);
    int getArmiesPool();
    void setArmiesPool(int armies);
    int getPlayerId();
    void setPlayerId(int id);
    Player *getPlayerInTreaty();
    void setPlayerInTreaty(Player *player, int *numberOfTurns);
    int getTurnsUntilEndOfTreaty();
    void addToPoolOfArmies(int newArmies);
    vector<Command> *toAttack();
    vector<Command> *toDefend();
    void issueOrder();
    OrdersList *getOrdersList();
    bool getHasCapturedATerritory();
    void setHasCapturedATerritory(bool b);
    bool getGotAttacked();
    void setGotAttacked(bool b);
    Hand *getHand();
    void setHand(Hand *hand);
    PlayerStrategy *getPlayerStrategy();
    void setPlayerStrategy(PlayerStrategy *strategy);
    bool setPlayerStrategy(string strategy);
    void printTerritories();
    bool operator==(const Player &rhs);
    friend ostream &operator<<(ostream &out, const Player &p);

private:
    string *name;
    int *playerId;
    vector<Territories *> *territoryList;
    Hand *hand;
    OrdersList *ordersList;
    bool *hasCapturedATerritory;
    bool *gotAttacked;
    int *armiesPool;
    Player *playerInTreaty = nullptr;
    int *turnsUntilEndOfTreaty;
    PlayerStrategy *strategy;
};
#endif
